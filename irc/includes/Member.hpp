#ifndef MEMBER_HPP
#define MEMBER_HPP

#include "mainhead.hpp"

class Member{
	private :
		std::string			password;
		std::string 		name;
		std::string 		roomName;
	    int				 	clnt_sock;
	public :
		Member (int fd);
		void setPass(std::string password);
		void setName(std::string name);
		void setRoom(std::string roomName);
		int	getFD() const;
		std::string getPassword() const;
		std::string getName() const;
		std::string getRoom() const;
};

#endif