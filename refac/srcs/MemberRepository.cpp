#include "../incs/MemberRepository.hpp"

MemberRepository::MemberRepository(){
	Member admin("admin", "admin", "관리자");

	admin.setGrade(2);
	member.push_back(admin);
}

MemberRepository::~MemberRepository(){
	
}

void MemberRepository::addMember(std::string id, std::string password, std::string name){
	Member add(id, password, name);

	add.setGrade(0);
	member.push_back(add);
}

size_t MemberRepository::getMemberSize(){
	return this->member.size();
}

bool MemberRepository::memberWithdrawal(std::string id, std::string password){
	size_t i;

	for (i=0; i < this->member.size(); i++){
		if (member[i].getById() == id)
			break;
	}
	if (i == member.size())
		return false;
	if (member[i].getPassword() == password){
		member.erase(member.begin() + i);
		return true;
	}
	return false;
}

bool MemberRepository::login(std::string id, std::string password){
	Member loginmember;

	loginmember = findById(id);
	if (loginmember.getById().length() == 0)
		return (false);
	else if (loginmember.getPassword() != password)
		return (false);
	else
		return (true);
}

Member MemberRepository::findById(std::string id){
	Member result;
	size_t i = 0;
	while (i < member.size()){
		if (member[i].getById() == id)
		{
			return (member[i]);
		}
		i++;
	}
	return (result);
}

Member MemberRepository::getNumMember(size_t i){
	Member dest;
	if (i < this->member.size())
		return (member[i]);
	return dest;
}

void MemberRepository::changeMember(std::string id, std::string password, std::string name){
	size_t i = 0;
	while (i < member.size()){
		if (id == member[i].getById())
			break;
		i++;
	}
	if (password.length() != 0){
		member[i].setPw(password);
	}
	if (name.length() != 0){
		member[i].setName(name);
	}
}