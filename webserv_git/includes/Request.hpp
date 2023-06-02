#ifndef __REQUEST_HPP__
#define __REQUEST_HPP__

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <sstream>

class Request
{
	private:
		std::vector<std::string> _contents;
		std::string _request;
		std::string _method;
		std::string _version;
		std::string _uri;
		std::string _serverName;
		std::string _port;
		std::map<std::string, std::string> _headers;
		std::string _body;
	public:
		Request(std::string& request);
		void setRequest(std::string request);
		void setRequestLine(std::vector<std::string>::iterator it);
		void setHeader(std::vector<std::string>::iterator it);
		void setBody(std::string body);
		std::string getMethod() const;
		std::string getUri() const;
		std::string getVersion() const;
		std::string getRequest() const;
		std::string getBody() const;
		std::map<std::string, std::string> getHeaders() const;
		std::vector<std::string> split(std::string input, char delimiter);
		std::vector<std::string> splitData(std::vector<std::string> buffer);
};

#endif