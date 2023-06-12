#ifndef MEMBER_HPP
#define MEMBER_HPP

#include <iostream>
#include <string>

// grade : 0=user 1=grade 2=admin;
class Member{
	private :
		std::string id;
		std::string password;
		std::string name;
		int			grade;
	public :
		Member ();
		Member (std::string id, std::string password, std::string name);
		virtual ~Member();
		Member& operator=(Member const &obj);
		std::string getById() const;
		std::string getName() const; 
		std::string getPassword() const;
		int			getGrade() const;
		void		setPw(std::string pw);
		void		setName(std::string name);
		void		setGrade(int X);
};

#endif