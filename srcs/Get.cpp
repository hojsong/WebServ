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
	if (getValue(buf, "Cookie: login_id=").length() != 0)
		me = get_cookie(buf, mr);
	if (url == "/" || url == "/home")
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
	else if (url == "/upload")
		result = "./html/upload.html";
    return (result);
}

std::string  getFile(char *buf, int &rehead, MemberRepository *mr) {
	Member me;
	std::string content;
	rehead = 200;
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
