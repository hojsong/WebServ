#include "library.hpp"

// file save;
void saveToFile(MemberRepository *mr, const std::string& filename) {
    std::ofstream file(filename);
    if (file.is_open()) {
        for (size_t i = 0; i < mr->getMemberSize(); i++){
			Member mem = mr->getNumMember(i);
			file << "id=" << mem.getById();
			file << "&password=" << mem.getPassword();
			file << "&name=" << mem.getName();
			file << "&grade=" << mem.getGrade();
			file << "&\n";
		}
        file.close();
	}
}

// file delete;
void deleteFile(const std::string& filename) {
    unlink(filename.c_str());
}

std::string GetUrl(char *buf){
	const char* refererStart = std::strstr(buf, "GET ");
	std::string url = "";

	if (refererStart != NULL)
    {
        const char* urlStart = refererStart + std::strlen("GET ");
        const char* urlEnd = std::strstr(urlStart, " HTTP/");

        if (urlEnd != NULL)
            url = std::string(urlStart, urlEnd - urlStart);
    }
	else {
		refererStart = std::strstr(buf, "POST ");
		const char* urlStart = refererStart + std::strlen("POST ");
		const char* urlEnd = std::strstr(urlStart, " HTTP/");

        if (urlEnd != NULL)
            url = std::string(urlStart, urlEnd - urlStart);
	}
	
	return url;
}

int	ft_strncmp(char *str1, const char *str2, int n){
	int i = 0;

	if (str1 == NULL)
		return (1);
	while (i < n){
		if (str1[i] != str2[i])
			return (1);
		i++;
	}
	return (0);
}

int method_cgi_differentiation(char *buf){
    if (ft_strncmp(buf, "GET ", 4) == 0)
        return (1);
    else if (ft_strncmp(buf, "POST ", 5) == 0)
        return (2);
    else if (ft_strncmp(buf, "DELETE ", 7) == 0)
        return (3);
    return (0);
}

std::string cgi_differentiation(MemberRepository *mr, Request req){
    std::string result;
    std::string str = req.getHeaders() + "\r\n\r\n" + req.getBody();
    char *buf = const_cast<char *>(str.c_str());

    int method = method_cgi_differentiation(buf);
    if (method == 0)
        return "";
    std::string url = GetUrl(buf);
    if (url.length() == 0)
        return "";
    if (method == 1 && url == "/members")
        result = hj_cgi_execve(buf, mr);
    else if (method == 1 && url == "/home")
        result = hj_cgi_execve(buf, mr);
    else if (url == "/members/new")
        result = hj_cgi_execve(buf, mr);
    else if (url == "/members/del")
        result = hj_cgi_execve(buf, mr);
    else if (url == "/members/logins") 
        result = hj_cgi_execve(buf, mr);
    else if (method == 2 && url == "/upload/file")
        result = hj_cgi_execve(buf, mr);
    else if (method == 2) {
        result = handle_cgi("cgi-bin/image_cgi.py", req);
    }
    return (result);
}

// buf == read() || recv() , mr = MemberRepository Class
std::string hj_cgi_execve(char *buf, MemberRepository *mr){
	deleteFile("memberdb.txt");
	saveToFile(mr, "memberdb.txt");
    std::string cgi_output;
    int cgiPipe[2];
    pid_t pid;
    int status;
    if (pipe(cgiPipe) < 0) {
        perror("pipe error");
        return "";
    }
    if ((pid = fork()) < 0) {
        perror("fork error");
        return "";
    }
    if (pid == 0) {
        dup2(cgiPipe[1], 1);
        close(cgiPipe[0]);
        char *execPath = const_cast<char*>("cgi.exe");
    	char *args[] = {execPath, buf, NULL};
        execve(execPath, args, NULL);
        exit(0);
    }
    else {
        close(cgiPipe[1]);
        char buf[1024];
        ssize_t bytesRead;
        while ((bytesRead = read(cgiPipe[0], buf, sizeof(buf))) > 0) {
            cgi_output.append(buf, bytesRead);
        }
        close(cgiPipe[0]);
        waitpid(pid, &status, 0);
    }
	return cgi_output;
}

long long time_diff(const timeval* clientTime){
    timeval end;
    gettimeofday(&end, NULL);

    long long diff = (end.tv_sec - clientTime->tv_sec) * 1000000;  // 초 단위 차이를 마이크로초 단위로 변환

    diff += end.tv_usec - clientTime->tv_usec;

    // if (diff > 60000000LL) {
    //     diff = 60000000LL;
    // }

    return diff;
}