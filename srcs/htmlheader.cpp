#include "../includes/My_socket.hpp"

std::string get_header(int status, std::string &content){
	std::string result;
	std::string con_size = std::to_string(content.length());

	if (status == 404){
		std::ifstream file("./html/404.html");
		std::string str = getHtml(file);
		std::string str_size = std::to_string(str.length());
		result = "HTTP/1.1 404 Not Found\r\nContent-Type: text/html; charset=utf-8\r\nContent-Length: " + str_size + "\r\n";
		content = str;
	}
	else if (status == 200){
		result = "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=utf-8\r\nContent-Length: "+ con_size + "\r\n";
	}
	else if (status == 101){
		result = "HTTP/1.1 101 Switching Protocols\r\nUpgrade: websocket\r\nConnection: Upgrade\r\n";
	}
	else if (status == 400){
		result = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/html; charset=utf-8\r\nContent-Length: "+ con_size +"\r\n";
	}
	else if (status == 102){
		result = "HTTP/1.1 102 Processing\r\n";
	}
	else if (status == 201){
		result = "HTTP/1.1 201 Created\r\nContent-Type: text/html; charset=utf-8\r\nContent-Length: " + con_size +"\r\n";
	}
	else if (status == 500){
		std::ifstream file("./html/500.html");
		std::string str = getHtml(file);
		std::string str_size = std::to_string(str.length());
		result = "HTTP/1.1 500 internal Server Error\r\nContent-Type: text/html; charset=utf-8\r\nContent-Length: " + str_size + "\r\n"; 
		content = str;
	}
	return(result);
}

void 		head_plus (std::string &content, int status, char *buf, MemberRepository *mr){
	std::string cookie = cookie_add(buf, mr);
    std::string response_header = get_header(status, content);
	if (cookie.length() == 0)
    	content = response_header+ "\r\n" + content;
	else{
		content = response_header + cookie + "\r\n\r\n" + content;
	}
}

std::string cookie_add(char *buf, MemberRepository *mr){
	std::string result = "";
	
	if (execveLogin(buf, mr) == true){
		result = "Set-Cookie: login_id=" + getValue(buf, "login_id=") + "&\r\n";
		std::cout<<result<<std::endl;
	}
	return result;
}

Member get_cookie(char *buf, MemberRepository *mr){
	Member member;
	
	std::string findId = getValue(buf, "login_id=");
	if (getValue(buf, "Cookie:").length() != 0){
		member = mr->findById(findId);
		std::cout<< "Get_cookie:"<< findId<<std::endl;
	}
	return member;
}