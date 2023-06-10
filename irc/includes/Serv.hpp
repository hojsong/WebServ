#ifndef SERV_HPP
#define SERV_HPP

#include "mainhead.hpp"

class Member;

class Serv{
	private :
		std::vector<Member> clnts;
	    int				 	serv_sock;
	    struct sockaddr_in 	serv_addr;
		std::string			password;
	public :
		Serv(char *argv, char *password);
		int		FindByFd(int fd);
		void	exe_Serv();
		void	acceptClient();
		bool	hasIncomingConnection();
		void	handleClientEvent();
		void	sendMassage(Member sender, std::string message);
};

#endif