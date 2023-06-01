#ifndef SERV_HPP
#define SERV_HPP

#include "./My_socket.hpp"
#include <map>
#define MAX_EVENTS 100

class MemberRepository;

class Serv{
	private :
    	MemberRepository memberRepository;
		std::vector<My_socket> servs;
		int					kq;
	public :
		Serv();
		void exe_Serv();
};

#endif