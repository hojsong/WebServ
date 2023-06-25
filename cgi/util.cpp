#include "cgi.hpp"

bool readFile(const std::string& filename, MemberRepository *mr) {
    std::ifstream file(filename);
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
			char* charPtr = new char[line.length() + 1];
    		std::strcpy(charPtr, line.c_str());
			std::string id = getValue(charPtr,"id=");
			std::string password = getValue(charPtr,"password=");
			std::string name = getValue(charPtr,"name=");
			std::string grade = getValue(charPtr,"grade=");
			Member mem(id, password, name);
			mem.setGrade(std::atoi(grade.c_str()));
			mr->pushback(mem);
			delete[] charPtr;
        }
        file.close();
		return true;
    }
	return false;
}

void readMR(MemberRepository *mr){
	Member mem;
	for (size_t i = 0; i < mr->getMemberSize(); i++){
		mem = mr->getNumMember(i);
		std::cout << "mem" << i+1 << std::endl;
		std::cout << "id = " << mem.getById() << std::endl;
		std::cout << "pw = " << mem.getPassword() << std::endl;
		std::cout << "name = " << mem.getName() << std::endl;
		std::cout << "grade = " << mem.getGrade() << std::endl;
	}
}
// buf = recv();
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

// buf = recv() , value = "id=" || "password=" ...
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

// str = html.str , findstr = "~~~~", value = getvalue
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
	const char* button4 = std::strstr(buf, "Withdrawal=");
	if (button4 != NULL){
		return (4);
	}
	return (0);
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

bool execveWithdrawal(char *buf, MemberRepository *mr){
	std::string id = getValue(buf, "id=");
	std::string password = getValue(buf, "password=");

	return (mr->login(id, password));
}

bool GetBool(char *buf,MemberRepository *mr){
	bool exe = true;
	int button = getButton(buf);
	
	if (button == 2){
		exe = execveSAVE(buf, mr);
	}
	else if (button == 3){
		exe = execveLogin(buf, mr);
	}
	else if (button == 4){
		exe = execveWithdrawal(buf, mr);
	}
	return exe;
}

bool GetComplete(char *buf,MemberRepository *mr){
	bool exe = false;
	int button = getButton(buf);
	
	if (button == 2){
		exe = execveSAVE(buf, mr);
	}
	else if (button == 3){
		exe = execveLogin(buf, mr);
	}
	else if (button == 4){
		exe = execveWithdrawal(buf, mr);
	}
	return exe;
}

std::string getHtml(std::istream& file) {
	std::string content;
	std::string line;

	while (std::getline(file, line)) {
		content += line;
		content += '\n';
	}
	return content;
}

std::string getFile(char *buf, MemberRepository *mr){
	std::string filename = "";
	std::string url;
	int button = getButton(buf);

	url = GetUrl(buf);
	if (button == 0 || button == 1)
		;
	else if (GetBool(buf, mr) == false)
		return "";
	if (url == "/members/new" && button == 1)
		filename = "./html/members/new/createMemberForm.html";
	// else if (url == "/members/new")
	// 	filename = "./html/members/new/home.html";
	else if (url == "/members")
		filename = "./html/members/memberList.html";	
	// else if (url == "/members/logins")
	// 	filename = "./html/members/logins/home.html";
	// else
	// 	return "";
	std::string result;
	std::ifstream file(filename);
	result = getHtml(file);
	return result;
}

void 	exe_member_list(std::string &str, MemberRepository *mr){
	size_t i;
	Member dest;
	std::string memberValues = "";
	for (i = 0; i < mr->getMemberSize(); i++){
		dest = mr->getNumMember(i);
		std::string grade;
		int g = dest.getGrade();
		if (g == 0)
			grade = "user";
		else if (g == 1)
			grade = "vip";
		else if (g == 2)
			grade = "admin";
		memberValues += "<tr> <td>" + dest.getById() + "</td> <td>" + dest.getPassword() + "</td> <td>" + dest.getName() + "</td><td>" + grade + "</td></tr>";
	}
	text_change(str, "%MEMBER_ROWS%", memberValues);
}