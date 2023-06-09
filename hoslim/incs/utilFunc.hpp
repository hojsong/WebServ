#ifndef UTILFUNC_HPP
#define UTILFUNC_HPP

#include "library.hpp"

std::vector<std::string>	splitArgs(std::string line, std::string sep);
void saveToFile(MemberRepository *mr, const std::string& filename);
void deleteFile(const std::string& filename);
std::string hj_cgi_execve(char *buf, MemberRepository *mr);
std::string cgi_differentiation(char *buf, MemberRepository *mr);

#endif