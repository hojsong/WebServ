#ifndef __SERVER_HPP__
#define __SERVER_HPP__

#include <iostream>
#include <string>
#include <vector>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#include <fcntl.h>
#include <unistd.h>
#include <fstream>

#include <pthread.h>

#include "Location.hpp"
#include "Request.hpp"
#include "GetMethod.hpp"
#include "PostMethod.hpp"

#define MAX_BUFFER_SIZE 8192

class Server
{
	private:
		std::vector<Location> _locations;
		std::string _serverName;
		std::string _receivedData;
		std::string _headerData;
		std::string _bodyData;
		size_t _bodySize;
		int _portNum;
		int _serverSocket;
		int _clientSocket;
		sockaddr_in _serverAddr;
		sockaddr_in _clientAddr;
		socklen_t _clientAddrSize;
	public:
		Server();
		~Server();
		void startServer();
		int acceptClient();
		void handleClient();
		void setPortNumber(const int port);
		void setServerName(const std::string name);
		void setLocations(const std::vector<Location> locations);
		void sendResponse(Request request);
		std::string getServerName() const;
		int getPortNumber() const;
		int getServerSocket() const;
		size_t checkBodysize(std::string header);
		std::vector<std::string> split(std::string input, char delimiter);
		int myStoi(const std::string& str);
		void printServerInfo() const;
};

#endif