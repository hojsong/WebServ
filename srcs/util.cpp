#include "../includes/My_socket.hpp"


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
	
	return url;
}

void button_execve(char *buf, std::string &str, MemberRepository &mr){
	int button = getButton(buf);
	bool exe = false;
	if (button == 1){
		exe = execveID_CHK(buf, mr);
		create_changer(buf, str, exe);
	}
}

Member membersLogin(char *buf, MemberRepository &mr){
	Member me;

	if (execveLogin(buf, mr)){
		std::string findId = getValue(buf, "login_id=");
		me = mr.findById(findId);
	}
	return me;
}

void URl_execve(char *buf, std::string &str, MemberRepository &mr, Member me){
	me = membersLogin(buf, mr);
	if (GetUrl(buf) == "/members"){
		if (me.getGrade() != 2){
			return ;
		}
		else if (me.getGrade() == 2){
			size_t i;
			Member dest;
			std::string memberValues = "";
			for (i = 0; i < mr.getMemberSize(); i++){
				dest = mr.getNumMember(i);
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
	}
	else if (GetUrl(buf) == "/" || GetUrl(buf) == "/index"){
		if (me.getById().length() != 0){
			std::string grade;
			int g = me.getGrade();
			if (g == 0)
				grade = "user";
			else if (g == 1)
				grade = "vip";
			else if (g == 2)
				grade = "admin";
			text_change(str, "${name}", me.getName());
			text_change(str, "${grade}", grade);
			std::cout << str << std::endl;
		}
	}
}