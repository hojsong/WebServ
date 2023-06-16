#include "library.hpp"

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

void	post_save(char* buf, MemberRepository *mr){
	std::string findId = getValue(buf, "id=");
	std::string findPassword = getValue(buf, "password=");
	std::string findName = getValue(buf, "name=");
	mr->addMember(findId, findPassword, findName);
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

bool GetComplete(char *buf, MemberRepository *mr){
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

void	delete_member_true(char *buf, MemberRepository *mr){
	if (std::strstr(buf, "_method=delete") && getButton(buf) == 4) {
		std::string id = getValue(buf, "id=");
		std::string password = getValue(buf, "password=");
		if(mr->memberWithdrawal(id, password) == true){
			std::cout << "Success" << std::endl;
		}
	}
}

void	save_true(char *buf, MemberRepository *mr){
	int button = getButton(buf);
	if (button == 2){
		post_save(buf, mr);
		// (void)mr;
	}
}

