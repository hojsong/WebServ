#ifndef MEMBER_HPP
#define MEMBER_HPP

#include "Head.hpp"

class Member{
    private :
        std::string memberID;
        std::string memberPW;
        std::string memberName;
        int         grade;
    public :
        Member(std::string id, std::string pw, std::string name);
        std::string getID(void) const;
        std::string getPW(void) const;
        std::string getName(void) const;
        int getGrade(void) const;
        void setGrade(int grade);
        virtual ~Member();
}

#endif