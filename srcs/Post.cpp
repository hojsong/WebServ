#include "../includes/My_socket.hpp"

int	getButton(char *buf){
	const char* button1 = std::strstr(buf, "id_chk=");
	if (button1 != NULL){
		return (1);
	}
	const char* button2 = std::strstr(buf, "save=");
	if (button2 != NULL){
		return (2);
	}
	const char* button3 = std::strstr(buf, "login=");
	if (button3 != NULL){
		return (3);
	}
	return (0);
}

std::string getValue(char *buf, const char *value){
	const char* rvalue = std::strstr(buf, value);
	std::string findvalue;
	if (rvalue != NULL)
    {
        const char* valueStart = rvalue + std::strlen(value);
        const char* valueEnd = std::strstr(valueStart, "&");
		if (valueStart == valueEnd)
			return "";
        if (valueEnd != NULL)
            findvalue = std::string(valueStart, valueEnd - valueStart);
        else{
			return "";
		}
    }
    else{
		return "";
	}
	return findvalue;
}

bool	execveID_CHK(char *buf, MemberRepository *mr){
	Member find;
	std::string findId = getValue(buf, "id=");
	find = mr->findById(findId);
	if (find.getById().length() == 0)
		return (true);
	else
		return (false);
}

bool	execveSAVE(char *buf, MemberRepository *mr){
	Member find;
	std::string findId = getValue(buf, "id=");
	std::string findPassword = getValue(buf, "password=");
	std::string findName = getValue(buf, "name=");
	find = mr->findById(findId);
	if (find.getById().length() != 0 || findPassword.length() < 4 ||
		findId.length() < 3 || findName.length() == 0)
		return false;
	mr->addMember(findId, findPassword, findName);
	return true;
}

bool	execveLogin(char *buf, MemberRepository *mr){
	Member find;
	std::string findId = getValue(buf, "login_id=");
	std::string findPassword = getValue(buf, "login_password=");
	find = mr->findById(findId);
	if (find.getById().length() == 0)
		return (false);
	if (findId.length() == 0)
		return (false);
	return (mr->login(findId, findPassword));
}
 
void	text_change(std::string& str, const std::string findstr, const std::string value){
    std::string::size_type pos = str.find(findstr);
    if (pos != std::string::npos) {
        str.replace(pos, findstr.length(), value);
    }
}

void	create_changer(char *buf, std::string &str, bool set){
	std::string findId = getValue(buf, "id=");
	std::string findPassword = getValue(buf, "password=");
	std::string findName = getValue(buf, "name=");
	if (findId.length() != 0){
		text_change(str, "th:value=\"${id}\"", "value=\""+findId+"\"");
	}
	if (findPassword.length() != 0){
		text_change(str, "th:value=\"${password}\"", "value=\""+findPassword+"\"");
	}
	if (findName.length() != 0){
		text_change(str, "th:value=\"${name}\"", "value=\""+findName+"\"");
	}
	if (set == true && findId.length() >= 3)
		text_change(str, "th:text=\"${message}\">", ">사용 가능한 ID입니다.");
	else
		text_change(str, "th:text=\"${message}\">", ">사용 불가능한 ID입니다.");
}

std::string getPOSTPath(char *buf, int &rehead, MemberRepository *mr){
	const char* refererStart = std::strstr(buf, "POST ");
	std::string result = "";
	std::string url;
	int	button;
	bool exe = false;

    if (refererStart != NULL)
    {
        const char* urlStart = refererStart + std::strlen("POST ");
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
	button = getButton(buf);
	if (button == 0)
		return (0);
	else if (button == 1){
		exe = execveID_CHK(buf, mr);
	}
	else if (button == 2){
		exe = execveSAVE(buf, mr);
	}
	else if (button == 3){
		exe = execveLogin(buf, mr);
	}
	if (button == 3){
		if (exe==true){
			std::ifstream file("./html/Home.html");
			if (!file.is_open()){
	        	file.close();
				rehead = 101;
				return "";
			}
			result = getHtml(file);
			rehead = 200;
			file.close();
		}
		else {
			std::ifstream file("./html/loginError.html");
			result = getHtml(file);
			rehead = 400;
			file.close();
		}
	}
	else if (button == 2){
		if (exe == true) {
			std::ifstream file("./html/Home.html");
			if (!file.is_open()){
	        	file.close();
				rehead = 102;
				return "";
			}
			result = getHtml(file);
			rehead = 201;
			file.close();
		}
		else{
			std::ifstream file("./html/createMemberError.html");
			result = getHtml(file);
			rehead = 400;
			file.close();
		}
	}
	else if (button == 1){
		std::ifstream file("./html/createMemberForm.html");
		result = getHtml(file);
		file.close();
		rehead = 200;
		create_changer(buf, result, exe);
	}
    return (result);
}
