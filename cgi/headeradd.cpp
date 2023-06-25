#include "cgi.hpp"

std::string get_header(int status, std::string &content){
	std::string result;
	std::string con_size = std::to_string(content.length());

	if (status == 404){
		std::ifstream file("./html/404.html");
		std::string str = getHtml(file);
		std::string str_size = std::to_string(str.length());
		result = "HTTP/1.1 404 Not Found\r\nContent-Type: text/html; charset=utf-8\r\nContent-Length: " + str_size;
		content = str;
	}
	else if (status == 200){
		result = "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=utf-8\r\nContent-Length: "+ con_size;
	}
	else if (status == 101){
		result = "HTTP/1.1 101 Switching Protocols\r\nUpgrade: websocket\r\nConnection: Upgrade\r\n";
		content = "";
	}
	else if (status == 400){
		result = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/html; charset=utf-8\r\nContent-Length: "+ con_size;
	}
	else if (status == 401){
		result = "HTTP/1.1 401 Unauthorized\r\nContent-Type: text/html; charset=utf-8\r\nContent-Length: "+ con_size;
	}
	else if (status == 102){
		result = "HTTP/1.1 102 Processing\r\n";
	}
	else if (status == 201){
		result = "HTTP/1.1 201 Created\r\nContent-Type: text/html; charset=utf-8\r\nContent-Length: " + con_size;
	}
	else if (status == 500){
		std::ifstream file("./html/500.html");
		std::string str = getHtml(file);
		std::string str_size = std::to_string(str.length());
		result = "HTTP/1.1 500 internal Server Error\r\nContent-Type: text/html; charset=utf-8\r\nContent-Length: " + str_size; 
		content = str;
	}
	return(result);
}

void 		head_plus (std::string &content, int status, char *buf, MemberRepository *mr){
	std::string cookie;
	if ((GetUrl(buf) == "/members/logins" || GetUrl(buf) == "/members/new") && getButton(buf) == 0 && get_cookie(buf, mr).getById().length() != 0)
		cookie = "Set-Cookie: login_ided=; expires=Thu, 01 Jan 1970 00:00:00 GMT; path=/";
	else if (GetUrl(buf) == "/members/del" && execveWithdrawal(buf, mr) == true && get_cookie(buf, mr).getById() == getValue(buf, "id="))
		cookie = "Set-Cookie: login_ided=; expires=Thu, 01 Jan 1970 00:00:00 GMT; path=/";
	else  
		cookie = cookie_add(buf, mr);
    std::string response_header = get_header(status, content);
	if (cookie.length() == 0)
    	content = response_header+ "\r\n\r\n" + content;
	else{
		content = response_header + "\r\n" + cookie + "\r\n\r\n" + content;
	}
}

std::string cookie_add(char *buf, MemberRepository *mr){
	std::string result = "";
	
	if (getButton(buf) == 3 && execveLogin(buf, mr) == true){
		if (get_cookie(buf, mr).getById().length() == 0)
			result = "Set-Cookie: login_ided=" + getValue(buf, "login_id=") + "&; path=/";
	}
	return result;
}

Member get_cookie(char *buf, MemberRepository *mr){
	Member member;
	
	if (std::strstr(buf, "Cookie:") == NULL)
		return member;
	std::string findId = getValue(buf, "login_ided=");
	if (getValue(buf, "Cookie:").length() != 0){
		return(mr->findById(findId));
	}
	return member;
}