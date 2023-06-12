#include "Member.hpp"

Member::Member (){
	this->id = "";
	this->password = "";
	this->name = "";
	this->grade = 0;
}

Member::Member (std::string id, std::string password, std::string name){
	this->id = id;
	this->password = password;
	this->name = name;
	this->grade = 0;
}

Member::~Member(){
	
}

Member& Member::operator=(Member const &obj){
	this->id = obj.getById();
	this->password = obj.getPassword();
	this->name = obj.getName();
	this->grade = obj.getGrade();
	return (*this);
}

std::string Member::getById() const{
	return (this->id);
}

std::string Member::getName() const{
	return (this->name);
}

std::string Member::getPassword() const{
	return (this->password);
}

int	Member::getGrade() const{
	return (this->grade);
}

void	Member::setPw(std::string pw){
	this->password = pw;
}

void	Member::setName(std::string name){
	this->name = name;
}

void	Member::setGrade(int X){
	this->grade = X;
}
