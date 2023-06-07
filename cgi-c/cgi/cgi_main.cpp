#include "cgi.hpp"

int main(int argc, char **argv){
	MemberRepository mr;
	std::string url;
	

	readFile("memberdb.txt", &mr);
	// readMR(&mr);
	// std::cout << "MemberReposytort Get End" << std::endl;
	if (argc != 2)
		return (0);
	int button = getButton(argv[1]);
	url = GetUrl(argv[1]);
	if (GetBool(argv[1], &mr) == false)
		exe_Error_page(argv[1], button, &mr);
	else if (url == "/members/new")
		member_id_chk(argv[1], &mr);
	else if (url == "/members")
		member_list(argv[1], &mr);
	return (0);
}