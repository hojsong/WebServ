#ifndef MY_SOCKET_HPP
#define MY_SOCKET_HPP

#include "my_function.hpp"
#include "MemberRepository.hpp"
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <map>

class MemberRepository;

class My_socket {
	private:
    	MemberRepository 					*memberRepository;
	    int 								serv_sock;
	    int				 					clnt_sock;
	    struct sockaddr_in				 	serv_addr;
	    struct sockaddr_in 					clnt_addr;
	    int 								result;
		std::string							dir;
		std::map<std::string, std::string> 	file;
	public:
    	My_socket(const char *argv, MemberRepository *mr);
		void dir_indexing(const char *str);
    	virtual ~My_socket();
		int	getServSock();
    	std::string getPath(char *buf);
    	int getResult(void) const;
    	int acceptClient();
		void handleClientEvent();
};

#endif