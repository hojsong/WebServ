#include "../include/Member.hpp"

Member::Member(std::string id, std::string pw, std::string name){
    this->memberID = id;
    this->memberPW = pw;
    this->memberName = name;
    this->grade = 1;
}

Member::~Member(){

}

std::string Member::getID(void) const{
    return (this->memberID);
}

std::string Member::getPW(void) const{
    return (this->memberPW);
}

std::string Member::getName(void) const{
    return (this->memberName);
}

int Member::getGrade(void) const{
    return (this->grade);
}

void Member::setGrade(int grade){
    this->grade = grade;
}