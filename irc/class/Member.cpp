#include "../includes/Member.hpp"

Member::Member (int fd){
	this->clnt_sock = fd;
}

void 		Member::setPass(std::string password){
	this->password = password;
}

void 		Member::setName(std::string name){
	this->name = name;
}

void Member::setRoom(std::string roomName){
	this->roomName = roomName;
}

int			Member::getFD() const{
	return (this->clnt_sock);
}

std::string Member::getPassword() const{
	return (this->password);
}

std::string Member::getName() const{
	return (this->name);
}

std::string Member::getRoom() const{
	return (this->roomName);
}

