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

const char* getFilePath(char* buf, MemberRepository &mr) {
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
	if (execveLogin(buf, mr) == true){
		std::string findId = getValue(buf, "id=");
		me = mr.findById(findId);
	}
	if (url == "/" || url == "/index")
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

std::string  getFile(int clnt_sock, int &rehead, MemberRepository &mr) {
	char buf[4096];
    int bys = recv(clnt_sock, buf, sizeof(buf), 0);
	// std::cout << buf << std::endl;
	// std::cout << "-------------------" << std::endl;
	std::string content;
	Member me;
	rehead = 200;
    if (bys == -1){
		rehead = 500;
        return "";
	}
    else if (bys == 0){
        return "";
	}
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
