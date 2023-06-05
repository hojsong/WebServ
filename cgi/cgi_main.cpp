#include "cgi.hpp"

int main(int argc, char **argv){
	MemberRepository mr;
	std::string url;
	if (argc != 2)
		return (0);
		
	readFile("memberdb.txt", &mr);
	// readMR(&mr);
	// std::cout << "MemberReposytort Get End" << std::endl;
	url = GetUrl(argv[1]);
	if (url == "/members/new")
		member_id_chk(argv[1], &mr);
	else if (url == "/members")
		member_list(argv[1], &mr);
	return (0);
}