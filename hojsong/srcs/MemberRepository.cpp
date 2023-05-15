#include "../include/MemberRepository.hpp"

MemberRepository::MemberRepository(){
    this->idx = 0;
}

void   MemberRepository::addMember(std::string id, std::string pw, std::string name){
    if (id.length() == 0 || pw.length() == 0 || name.length() == 0)
        throw MemberRepository::NullPointException();
    Member addm(id, pw, name);
    
    this->member.push_back(addm);
    this->idx++;
}

member MemberRepository::findID(std::string ID){
    int i;
    
    for (i = 0; i < this->idx; i++){
        if (this->memeber.getID() == ID)
            break;
    }
    // 없는 아이디인 경우
    if (i == idx)
        throw MemberRepository::NullPointException();
    // 중복된 아이디인 경우
    else
        throw MemberRepository::DupIDException();
}       

MemberRepository::NullPointException::NullPointException(){

}

MemberRepository::DupIDException::DupIDException(){

}

