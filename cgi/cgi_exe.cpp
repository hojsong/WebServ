#include "cgi.hpp"

void	exe_Error_page(char *buf, int button, MemberRepository *mr){
	std::ifstream file("./html/Error/cgi_Error.html");
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
		head_plus(errorpage, 400, buf, mr);
	}
	if (button == 3){
		text_change(errorpage, "Message1", "로그인 실패");
		text_change(errorpage, "Message2", "ID 또는 Password 를 확인해주세요");
		head_plus(errorpage, 401, buf, mr);
	}
	if (button == 4){
		text_change(errorpage, "Message1", "회원 탈퇴 실패");
		text_change(errorpage, "Message2", "ID 또는 Password 를 확인해주세요");
		head_plus(errorpage, 400, buf, mr);
	}
	std::cout << errorpage << std::endl;
	errorpage.clear();
}

void	exe_Complete_page(char *buf, int button, MemberRepository *mr){
	std::ifstream file("./html/members/complete/complete.html");
	std::string errorpage = getHtml(file);
	std::string findId = getValue(buf, "id=");
	if (button == 2){
		text_change(errorpage, "Message1", "생성 성공");
		text_change(errorpage, "Message2", findId + "님 회원 가입이 완료되었습니다.");
		head_plus(errorpage, 201, buf, mr);
	}
	if (button == 3){
		text_change(errorpage, "Message1", "로그인 성공");
		text_change(errorpage, "Message2", findId + "님 로그인이 완료되었습니다.");
		head_plus(errorpage, 200, buf, mr);
	}
	if (button == 4){
		text_change(errorpage, "Message1", "회원 탈퇴 성공");
		text_change(errorpage, "Message2", findId + "님 회원 탈퇴가 완료되었습니다.");
		head_plus(errorpage, 200, buf, mr);
	}
	std::cout << errorpage << std::endl;
	errorpage.clear();
}

void	member_id_chk(char *buf, MemberRepository *mr){
	std::string str = getFile(buf, mr);
	int button = getButton(buf);
	
	if (button == 1){
		bool set = execveID_CHK(buf, mr);
		create_changer(buf, str, set);
		head_plus(str, 200, buf, mr);
		std::cout << str << std::endl;
	}
}
	
void	member_list(char *buf, MemberRepository *mr){
	Member mb = get_cookie(buf, mr);
	std::string str;
	if (mb.getGrade() != 2){
		std::ifstream file("./html/Error/cgi_Error.html");
		str = getHtml(file);
		text_change(str, "Message1", "접근 실패");
		text_change(str, "Message2", "관리자가 아닙니다.");
		head_plus(str, 400, buf, mr);
	}
	else{
		str = getFile(buf, mr);
		exe_member_list(str, mr);
		head_plus(str, 200, buf, mr);
	}
	std::cout << str << std::endl;
}

void	logined_home(char *buf, MemberRepository *mr){
	std::string str;
	Member mb = get_cookie(buf, mr);
	if (mb.getById().length() != 0)
	{
		std::ifstream file("./html/home.html");
		str = getHtml(file);
		text_change(str, "로그인", "로그 아웃");
		text_change(str, "th:text=\"${message}\">", ">" + mb.getName() + "님 환영합니다.");
		head_plus(str, 200, buf, mr);
		
	}
	std::cout << str << std::endl;
}

void	cookie_del(std::string url, char *buf, MemberRepository *mr){
	std::string filename = "";
	std::string str;
	if(url == "/members/logins")
		filename = "./html/members/logins/loginMemberForm.html";
	else if (url == "/members/new")
		filename = "./html/members/new/createMemberForm.html";
	if (filename.length() == 0)
		return ;
	std::ifstream file(filename);
	str = getHtml(file);
	head_plus(str, 200, buf, mr);
	std::cout << str << std::endl;
}