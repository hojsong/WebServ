#include "../includes/mainhead.hpp"

int main(int argc, char **argv){
	if (argc != 3)
		return (1);
	Serv serv(argv[1], argv[2]);
	serv.exe_Serv();
	return (0);
}