#include "../includes/Serv.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
int ErrorPrint(const char *str);
int init_fail(My_socket my_so);

int main(){
	Serv server;
	server.exe_Serv();
	return (0);
}

int init_fail(My_socket my_so){
	const char *printError;
	if (my_so.getResult() == 1)
		printError = "socket error\n";
	else if (my_so.getResult() == 2)
		printError = "bind error\n";
	else if (my_so.getResult() == 3)
		printError = "listen fail\n";
	else if (my_so.getResult() == 3)
		printError = "accept fail\n";
	else
		return (0);
	return (ErrorPrint(printError));
}

int ErrorPrint(const char *str){
	size_t i = 0;
	
	if (str == NULL)
		return (1);
	while (str[i]){
		write(2, &str[i], 1);
		i++;
	}
	return (1);
}