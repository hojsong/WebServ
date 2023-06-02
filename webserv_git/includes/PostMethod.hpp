#ifndef __POSTMETHOD_HPP__
#define __POSTMETHOD_HPP__

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/socket.h>

#include "Request.hpp"

class PostMethod
{
	private:
		std::stringstream _status;
        std::string _request;
        std::string _uri;
        std::map<std::string, std::string> _headers;
		std::string _body;
		std::string _response;
		std::string _filePath;
	public:
		PostMethod();
		PostMethod(Request request);
		~PostMethod();
        int executeCgi(std::string cgiPath, int clientSock);
		std::string getResponse() const;
};

#endif