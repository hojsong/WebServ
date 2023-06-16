#ifndef LIBRARY_HPP
#define LIBRARY_HPP

//macro

#define BUFFER_SIZE 300000
#define TIMEOUT_SEC 60

//C++
#include <iostream>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <exception>

//Container
#include <vector>
#include <map>
#include <queue>

//C
#include <unistd.h>
#include <netdb.h>
#include <fcntl.h>
#include <signal.h>
#include <dirent.h>

#include <sys/socket.h>
#include <sys/select.h>
#include <sys/poll.h>
#include <sys/event.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <sys/time.h>

//class
#include "Member.hpp"
#include "MemberRepository.hpp"
#include "ServerManage.hpp"
#include "ParseConfig.hpp"
#include "Server.hpp"
#include "Location.hpp"
#include "Response.hpp"
#include "Request.hpp"
#include "Connection.hpp"
#include "utilFunc.hpp"

class ErrorException : public std::exception
{
    private:
        std::string message;
    public:
        ErrorException(std::string mess) throw() {
            message = "Error: " + mess;
        }
        const char* what() const throw() {
            return message.c_str();
        }
        ~ErrorException() throw() {}
};

#endif