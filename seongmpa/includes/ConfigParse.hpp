#ifndef __CONFIGPARSE_HPP__
#define __CONFIGPARSE_HPP__

#include <iostream>
#include <fstream>
#include <exception>
#include <vector>
#include <sstream>

#include "Server.hpp"

class ConfigParse
{
	private:
		std::string _serverName;
		int _portNumber;
		std::string _uriPath;
		std::vector<std::string> _blocks;
		std::vector<std::string> _contents;
		std::vector<Server> _servers;
		std::vector<Location> _locations;
	public:
		void getData(std::string file);
		void setData(std::string filePath);
		std::vector<std::string> split(std::string input, char delimiter);
		std::vector<std::string> splitServerBlocks(std::string input);
		std::vector<std::string> splitData(std::vector<std::string> buffer);
		int myStoi(const std::string &str);
		int countChar(const std::string& str, char ch);
		size_t setLocation(std::vector<std::string> data, size_t index);
		void setServerBlock(std::vector<std::string>::iterator it);
		bool checkVaildate(std::vector<std::string>::iterator it);
		std::vector<std::string>::iterator isValidPort(std::vector<std::string>::iterator it);
		std::vector<std::string>::iterator isValidHostName(std::vector<std::string>::iterator it);
		std::vector<std::string>::iterator isValidLocation(std::vector<std::string>::iterator it, Location& location);
		std::vector<Server> getServers() const;
		std::vector<Location> getLocations() const;
		void printParseData(void) const;
};

#endif