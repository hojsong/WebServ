#ifndef CGI_HPP
#define CGI_HPP

#include "Member.hpp"
#include "MemberRepository.hpp"
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <map>

std::string GetUrl(char *buf);
std::string getValue(char *buf, const char *value);
void		saveToFile(MemberRepository *mr, const std::string& filename);
void		text_change(std::string& str, const std::string findstr, const std::string value);
void		create_changer(char *buf, std::string &str, bool set);
int			getButton(char *buf);
bool		execveID_CHK(char *buf, MemberRepository *mr);
bool		execveSAVE(char *buf, MemberRepository *mr);
bool		execveLogin(char *buf, MemberRepository *mr);
bool		execveWithdrawal(char *buf, MemberRepository *mr);
void 		URl_execve(char *buf, std::string &str, MemberRepository *mr, Member me);
bool		readFile(const std::string& filename, MemberRepository *mr);
void		readMR(MemberRepository *mr);
void		member_list(char *buf, MemberRepository *mr);
void		member_id_chk(char *buf, MemberRepository *mr);
bool		GetBool(char *buf,MemberRepository *mr);
std::string getHtml(std::istream& file);
std::string getFile(char *buf, MemberRepository *mr);
void 		exe_member_list(std::string &str, MemberRepository *mr);
void		deleteFile(const std::string& filename);
void		exe_Error_page(char *buf, int button, MemberRepository *mr);
std::string getPostFileName(char *buf);
std::string bufsaveBody(char *buf);
void		POST_FILE(char *buf, MemberRepository *mr);
std::string cookie_add(char *buf, MemberRepository *mr);
void 		head_plus(std::string &content, int status, char *buf, MemberRepository *mr);
Member 		get_cookie(char *buf, MemberRepository *mr);
std::string get_header(int status, std::string &content);
void		exe_Complete_page(char *buf, int button, MemberRepository *mr);
bool	 	GetComplete(char *buf,MemberRepository *mr);
void		logined_home(char *buf, MemberRepository *mr);
void		cookie_del(std::string url, char *buf, MemberRepository *mr);

#endif