#include "cgi.hpp"

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