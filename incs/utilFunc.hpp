#ifndef UTILFUNC_HPP
#define UTILFUNC_HPP

#include "library.hpp"

std::vector<std::string>	splitArgs(std::string line, std::string sep);
std::vector<std::string>	split(const std::string& str, char delimiter);
void 		saveToFile(MemberRepository *mr, const std::string& filename);
void 		deleteFile(const std::string& filename);
std::string hj_cgi_execve(char *buf, MemberRepository *mr);
std::string cgi_differentiation(MemberRepository *mr, Request req);
std::string getPostValue(char *buf, const char *value);
bool		execveSAVE(char *buf, MemberRepository *mr);
void		delete_member_true(char *buf, MemberRepository *mr);
int			ft_strncmp(char *str1, const char *str2, int n);
void		save_true(char *buf, MemberRepository *mr);
std::string getValue(char *buf, const char *value);
int			getButton(char *buf);
bool 		GetComplete(char *buf, MemberRepository *mr);
std::string handle_cgi(std::string cgiPath, Request req);
long long	time_diff(const timeval* clientTime);

#endif