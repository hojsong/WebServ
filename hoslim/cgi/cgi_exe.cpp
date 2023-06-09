#include "cgi.hpp"

void	exe_Error_page(char *buf, int button, MemberRepository *mr){
	std::ifstream file("./html/Error.html");
	std::string errorpage = getHtml(file);
	Member find;
	if (button == 2){
		text_change(errorpage, "Message1", "생성 실패");
		std::string findId = getValue(buf, "id=");
		std::string findPassword = getValue(buf, "password=");
		std::string findName = getValue(buf, "name=");
		find = mr->findById(findId);
		if (find.getById().length() != 0 || findId.length() < 3)
			text_change(errorpage, "Message2", "ID 중복 되었거나 길이가 부족합니다.");
		else if (findPassword.length() < 4)
			text_change(errorpage, "Message2", "비밀번호의 길이가 적절하지 않습니다.");
		else if (findName.length() == 0)
			text_change(errorpage, "Message2", "이름을 입력하지 않으셨습니다.");
	}
	if (button == 3){
		text_change(errorpage, "Message1", "로그인 실패");
		text_change(errorpage, "Message2", "ID 또는 Password 를 확인해주세요");
	}
	std::cout << errorpage << std::endl;
}

void	member_id_chk(char *buf, MemberRepository *mr){
	std::string str = getFile(buf, mr);
	int button = getButton(buf);
	
	if (button == 1){
		bool set = execveID_CHK(buf, mr);
		create_changer(buf, str, set);
		std::cout << str << std::endl;
	}
}
	
void	member_list(char *buf, MemberRepository *mr){
	std::string str = getFile(buf, mr);
	exe_member_list(str, mr);
	std::cout << str << std::endl;
}