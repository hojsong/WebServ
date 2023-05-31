#ifndef MY_FUNCTION_HPP
#define MY_FUNCTION_HPP

#include "./mainhead.hpp"

class My_socket;
class Member;
class MemberRepository;

int 		ErrorPrint(const char *str);
int 		init_fail(My_socket my_so);
int			ft_strncmp(char *str1, const char *str2, int n);
int			getButton(char *buf);
bool		execveSAVE(char *buf, MemberRepository *mr);
bool		execveLogin(char *buf, MemberRepository *mr);
bool		execveID_CHK(char *buf, MemberRepository *mr);
void		create_changer(char *buf, std::string &str, bool set);
void		text_change(std::string& str, const std::string findstr, const std::string value);
void		URl_execve(char *buf, std::string &str, MemberRepository *mr, Member me);
void		button_execve(char *buf, std::string &str, MemberRepository *mr);
const char* getFilePath(char* buf, MemberRepository *mr);
std::string getValue(char *buf, const char *value);
std::string GetUrl(char *buf);
std::string getHtml(std::istream& file);
std::string getFile(char *buf, int &rehead, MemberRepository *mr);
std::string getPOSTPath(char *buf, int &rehead, MemberRepository *mr);
Member 		membersLogin(char *buf, MemberRepository *mr);
void 		head_plus (std::string &content, int status, char *buf, MemberRepository *mr);
std::string get_header(int status, std::string &content, char *buf);
std::string cookie_add(char *buf, MemberRepository *mr);
Member get_cookie(char *buf, MemberRepository *mr);



#endif