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
#include <cstring>
#include <cstdlib>
#include <vector>

class MemberRepository;

class My_socket {
	private:
    	MemberRepository memberRepository;
	    int 				serv_sock;
	    std::vector<int> 	clnt_socks;
	    struct sockaddr_in serv_addr;
	    struct sockaddr_in clnt_addr;
	    int result;

	public:
    	My_socket(char **argv);
    	virtual ~My_socket();
    	std::string getPath(int clnt_sock);
    	int getResult(void) const;
    	void execve_serv();
};

#endif