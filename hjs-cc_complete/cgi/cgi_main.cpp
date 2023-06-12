#include "cgi.hpp"

int main(int argc, char **argv){
	MemberRepository mr;
	std::string url;

	readFile("memberdb.txt", &mr);
	if (argc != 2)
		return (0);	
	int button = getButton(argv[1]);
	url = GetUrl(argv[1]);
	if (GetBool(argv[1], &mr) == false)
		exe_Error_page(argv[1], button, &mr);
	if (GetComplete(argv[1], &mr) == true)
		exe_Complete_page(argv[1], button, &mr);
	if (url == "/home" && get_cookie(argv[1], &mr).getById().length() != 0)
		logined_home(argv[1], &mr);
	else if (url == "/members/new")
		member_id_chk(argv[1], &mr);
	else if (url == "/members")
		member_list(argv[1], &mr);
	else if (url == "/upload/file")
		POST_FILE(argv[1], &mr);
	return (0);
}