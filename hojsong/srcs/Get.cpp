#include "../includes/My_socket.hpp"

std::string getHtml(std::istream& file) {
	std::string content;
	std::string line;

	while (std::getline(file, line)) {
		content += line;
		content += '\n';
	}
	return content;
}

std::string mapGet(std::string url, std::map<std::string, std::string> file){
	std::map<std::string, std::string>::iterator it = file.find(url);
	std::string value;
	if (it != file.end()) {
	    value = it->second;
	} else {
	    std::map<std::string, std::string>::iterator it = file.find("notFount");
	    value = it->second;
	}
	return value;
}

const char* getFilePath(char* buf, MemberRepository *mr) {
	const char* hostStart = std::strstr(buf, "Host: ");
	const char* refererStart = std::strstr(buf, "GET ");
	const char* result = NULL;  // 수정: 초기화
	std::string host;
	std::string url;
	Member		me;

	if (hostStart != NULL) {
		const char* host_url = hostStart + std::strlen("Host: ");
		const char* host_urlEnd = std::strstr(host_url, "\r\n");

		if (host_urlEnd != NULL)
			host = std::string(host_url, host_urlEnd - host_url);
		else
			return NULL;
	}
	else
		return NULL;
    if (refererStart != NULL)
    {
        const char* urlStart = refererStart + std::strlen("GET ");
        const char* urlEnd = std::strstr(urlStart, " HTTP/");
        
		if (urlStart == urlEnd)
			url = "";
        else if (urlEnd != NULL)
            url = std::string(urlStart, urlEnd - urlStart);
        else{
			return NULL;
		}
    }
    else{
		return NULL;
	}
	me = get_cookie(buf, mr);
	if (url == "/" || url == "/home" || url == "/favicon.ico")
		result = "./html/Home.html";
	else if (url == "/members/logins")
		result = "./html/loginMemberForm.html";
	else if (url == "/members/new")
		result = "./html/createMemberForm.html";
	else if (url == "/members" && me.getGrade() == 2)
		result = "./html/memberList.html";
	else if (url == "/members" && me.getGrade() != 2)
		result = "./html/adminError.html";
	else if (url == "/loginError")
		result = "./html/loginError.html";
	return (result);
}

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

void deleteFile(const std::string& filename) {
    unlink(filename.c_str());
}

std::string cgi_execve(char *buf, MemberRepository *mr){
	deleteFile("memberdb.txt");
	saveToFile(mr, "memberdb.txt");
	int cgiInput[2], cgiOutput[2];
    pid_t pid;
    std::string cgi_output = "";
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
        char *execPath = const_cast<char*>("a.out");
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
        std::cout << "-------------CGI OUTPUT-------------" << std::endl;
        std::cout << cgi_output << std::endl;
        std::cout << "------------------------------------" << std::endl;
    }
	return cgi_output;
}


std::string  getFile(char *buf, int &rehead, MemberRepository *mr) {
	Member me;
	std::string content;
	rehead = 200;
	// int pid;

	content = cgi_execve(buf, mr);
	saveToFile(mr, "memberdb.txt");

	if (content.length() != 0)
		return content;
	if (ft_strncmp(buf, "GET ", 4) == 0){
		std::ifstream file(getFilePath(buf, mr));
		if (!file.is_open()){
			rehead = 404;
			return "";
		}
		std::string str = getHtml(file);
		file.close();
		button_execve(buf, str, mr);
		URl_execve(buf, str, mr, me);
		content = str;
	}
	if (ft_strncmp(buf, "POST ", 4) == 0){
		std::string file(getPOSTPath(buf, rehead, mr));
		if (file.length() == 0)
		{
			write (2, "Error\n", 6);
			return "";
		}
		content = file;
	}
	return content;
}
