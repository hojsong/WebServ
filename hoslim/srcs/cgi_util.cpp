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

std::string cgi_differentiation(char *buf, MemberRepository *mr){
    std::string result;

    int method = method_cgi_differentiation(buf);
    std::string url = GetUrl(buf);
    if (method == 1 && url == "/members")
        result = hj_cgi_execve(buf, mr);
    else if (method == 2 && url == "/members/new"){
        result = hj_cgi_execve(buf, mr);
        execveSAVE(buf, mr);
    }
    else if ((method == 2 || method == 3 ) && url == "/members/del"){
        result = hj_cgi_execve(buf, mr);
        execveSAVE(buf, mr);
    }
    else if (method == 2 && url == "/members/logins")
        result = hj_cgi_execve(buf, mr);
    else if (method == 2 && url == "/upload/file")
        result = hj_cgi_execve(buf, mr);
    else
        ;
    return (result);
}

// buf == read() || recv() , mr = MemberRepository Class
std::string hj_cgi_execve(char *buf, MemberRepository *mr){
	deleteFile("memberdb.txt");
	saveToFile(mr, "memberdb.txt");
    std::string cgi_output;
	int cgiInput[2], cgiOutput[2];
    pid_t pid;
    int status;
    if (pipe(cgiInput) < 0 || pipe(cgiOutput) < 0) {
        perror("pipe error");
        return "";
    }
    if ((pid = fork()) < 0) {
        perror("fork error");
        return "";
    }
    if (pid == 0) {
        dup2(cgiInput[0], 0);
        dup2(cgiOutput[1], 1);
        close(cgiInput[1]);
        close(cgiOutput[0]);
        char *execPath = const_cast<char*>("cgi.exe");
    	char *args[] = {execPath, buf, NULL};
        execve(execPath, args, NULL);
        exit(0);
    }
    else {
        close(cgiInput[0]);
        close(cgiOutput[1]);
        // write(cgiInput[1], req.getBody().c_str(), req.getBody().size());
        close(cgiInput[1]);
        char buf[1024];
        ssize_t bytesRead;
        while ((bytesRead = read(cgiOutput[0], buf, sizeof(buf))) > 0) {
            cgi_output.append(buf, bytesRead);
        }
        close(cgiOutput[0]);
        waitpid(pid, &status, 0);
        // std::cout << "-------------CGI OUTPUT-------------" << std::endl;
        // std::cout << cgi_output << std::endl;
        // std::cout << "------------------------------------" << std::endl;
    }
	return cgi_output;
}

