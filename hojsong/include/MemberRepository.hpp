#ifndef MEMBERREPOSITORY_HPP
#define MEMBERREPOSITORY_HPP

#include "Head.hpp"

class MemberRepository{
    private :
        std::vector<Member> member;
        int idx;
    public :
        MemberRepository();
        void   addMember(std::string id, std::string pw, std::string name);
        member findID(std::string ID);
        class NullPointException : public std::exception
		{
			public:
				NullPointException();
		};
}

#endif