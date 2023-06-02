#include "../includes/Server.hpp"

Server::Server() {
	this->_bodySize = 0;
	this->_clientSocket = -1;
	this->_serverName = "";
	this->_serverSocket = -1;
}

void
Server::setPortNumber(const int port) {
	this->_portNum = port;
}

void
Server::setServerName(const std::string name) {
	this->_serverName = name;
}

void
Server::setLocations(const std::vector<Location> locations) {
	this->_locations = locations;
}

void
Server::startServer() {

	this->_serverSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (this->_serverSocket == -1) {
		std::cerr << "socket error" << std::endl;
		exit(1);
	}

	// 서버 소켓 Non-Blocking 처리
	// 소켓의 다른 플래그 정보도 저장하기 위해, flags 를 이용해 O_NONBLOCK과 OR 연산처리
	int flags = fcntl(this->_serverSocket, F_GETFL, 0);
	fcntl(this->_serverSocket, F_SETFL, flags | O_NONBLOCK);

	std::memset(&this->_serverAddr, 0, sizeof(this->_serverAddr));
	this->_serverAddr.sin_family = AF_INET;
	this->_serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	this->_serverAddr.sin_port = htons(this->_portNum);

	int option = 1;
	if (setsockopt(this->_serverSocket, SOL_SOCKET, SO_REUSEADDR, (const void *)&option, sizeof(option)) == -1) {
		std::cerr << "setsockopt error" << std::endl;
		exit(1);
	}

	if (bind(this->_serverSocket, reinterpret_cast<sockaddr*>(&this->_serverAddr), sizeof(this->_serverAddr)) == -1) {
		std::cerr << "bind error" << std::endl;
		exit(1);
	}

	if (listen(this->_serverSocket, 5) == -1) {
		std::cerr << "listen error" << std::endl;
		exit(1);
	}
}

int
Server::acceptClient() {
	if (this->_clientSocket == -1) {
		this->_clientAddrSize = sizeof(_clientAddr);
		std::memset(&this->_clientAddr, 0, this->_clientAddrSize);
		this->_clientSocket = accept(this->_serverSocket, reinterpret_cast<sockaddr*>(&this->_clientAddr), &this->_clientAddrSize);
		if (this->_clientSocket == -1) {
			std::cerr << "accept error: " << errno << std::endl;
			return true;
		}

		// 클라이언트 소켓 Non-Blocking 처리
		int flags = fcntl(this->_clientSocket, F_GETFL, 0);
		fcntl(this->_clientSocket, F_SETFL, flags | O_NONBLOCK);

		// Connect
		std::cout << "Client Connected: " << inet_ntoa(this->_clientAddr.sin_addr) << std::endl;
	}

	return (this->_clientSocket);
}

//void
//Server::handleClient() {
//	// Data read
//	char buffer[MAX_BUFFER_SIZE + 1];


//	ssize_t size = recv(this->_clientSocket, buffer, MAX_BUFFER_SIZE, 0);
//	//if (size != -1)
//	//	std::cout << "size: " << size << std::endl;
// 	if (size == -1) {
//		return ;
//	}
//	else if (size == 0) {
//		std::cout << "Connection Closed" << std::endl;
//		close(this->_clientSocket);
//		this->_clientSocket = -1;
//	}
//	else {
//		buffer[MAX_BUFFER_SIZE] = '\0';
//		if (this->_headerData.find("\r\n\r\n") == std::string::npos) {
//			std::string temp;

//			this->_headerData.append(buffer, size);
//		}
//		else if (this->_bodySize == 0) {
//			this->_bodySize = checkBodysize(this->_headerData);
//		}
//		else {
//			this->_bodyData.append(buffer, size);
//		}

//		std::cout << "headerData: " << std::endl << this->_headerData << std::endl;
//		std::cout << "bodySize: " << this->_bodySize << std::endl;
//		std::cout << "bodyData: " << std::endl << this->_bodyData << std::endl;
//		if (this->_bodySize) {
//			if ((this->_bodyData.find("\r\n\r\n") != std::string::npos) && (this->_bodyData.size() == this->_bodySize)) {
//				this->_receivedData = this->_headerData + this->_bodyData;
//				Request newRequest(this->_receivedData);

//				//std::cout << "----------------Received Data-------------------" << std::endl;
//				//std::cout << this->_receivedData << std::endl;
//				//std::cout << "------------------------------------------------" << std::endl;
//				sendResponse(newRequest);
//				this->_headerData.clear();
//				this->_bodyData.clear();
//				this->_receivedData.clear();
//			}
//		}
//		else {
//				if (this->_headerData.find("\r\n\r\n") != std::string::npos) {
//				this->_receivedData = this->_headerData + this->_bodyData;
//				Request newRequest(this->_receivedData);

//				//std::cout << "----------------Received Data-------------------" << std::endl;
//				//std::cout << this->_receivedData << std::endl;
//				//std::cout << "------------------------------------------------" << std::endl;
//				sendResponse(newRequest);
//				this->_headerData.clear();
//				this->_receivedData.clear();
//			}
//		}
//	}
//}

size_t
Server::checkBodysize(std::string header) {
	int size = 0;

	std::string::size_type contentLengthPos = header.find("Content-Length: ");
	if (contentLengthPos != std::string::npos) {
		std::string::size_type lengthStartPos = contentLengthPos + 16;
    	std::string::size_type lengthEndPos = header.find("\n", lengthStartPos);
    	std::string lengthStr = header.substr(lengthStartPos, lengthEndPos - lengthStartPos);
		size = myStoi(lengthStr);
    	std::cout << "Content-Length: " << lengthStr << std::endl;
	}
	return size;
}

void
Server::handleClient() {
	// Data read
	char buffer[MAX_BUFFER_SIZE + 1];
	std::string receivedData;


	ssize_t size = recv(this->_clientSocket, buffer, MAX_BUFFER_SIZE, 0);
	//if (size != -1)
		std::cout << "size: " << size << std::endl;
 	if (size == -1) {
		return ;
	}
	//else if (size == 0) {
	//	std::cout << "Connection Closed" << std::endl;
		
	//}
	else {
		buffer[MAX_BUFFER_SIZE] = '\0';
		this->_receivedData.append(buffer, size);
	
		if (this->_receivedData.find("\r\n\r\n") != std::string::npos) {
			Request newRequest(this->_receivedData);

			//std::cout << "----------------Received Data-------------------" << std::endl;
			//std::cout << this->_receivedData << std::endl;
			//std::cout << "------------------------------------------------" << std::endl;
			sendResponse(newRequest);
			this->_receivedData.clear();
			close(this->_clientSocket);
			this->_clientSocket = -1;
		}
	}
}

void
Server::sendResponse(Request request) {
	const std::string method = request.getMethod();
	std::string uri = request.getUri();
	const std::string version = request.getVersion();
	std::string filePath = this->_locations[0].getValue(uri);

	if (uri == "/") {
		filePath = "html/index.html";
	}
	else if (filePath != "") {
		filePath = this->_locations[0].getRootValue() + filePath;
	} 
	if (method == "GET") {
		GetMethod response(filePath);
		
		response.makeBody();
		send(this->_clientSocket, response.getResponse().c_str(), response.getResponse().length(), 0);
	}
	else if (method == "POST") {
		PostMethod response(request);

		const std::string cgiPath = "cgi-bin/my_cgi.py";
		//const std::string cgiPath = "cgi-bin/image_cgi.py";
		if (access(cgiPath.c_str(), X_OK) == -1) {
			perror("access error");
		}
		response.executeCgi(cgiPath, this->_clientSocket);
	}
	else if (method == "PUT") {

	}
	else if (method == "DELETE") {

	}
}

/* Getter Functions */
std::string
Server::getServerName() const {
	return this->_serverName;
}

int
Server::getPortNumber() const {
	return this->_portNum;
}

int
Server::getServerSocket() const {
	return this->_serverSocket;
}

void
Server::printServerInfo() const {
	std::cout << "-----------------------------" << std::endl;
	std::cout << "Servername: " << this->_serverName << std::endl;
	std::cout << "Port: " << this->_portNum << std::endl;
}

int
Server::myStoi(const std::string& str) {
	size_t index = 0;
	int sign = 1;
	int result = 0;

	if (str[0] == '-' || str[0] == '+')
	{
		if (str[0] == '-')
			sign = -1;
		index = 1;
	}
	while (index < str.length())
	{
		if ((str[index] < '0' || str[index] > '9') && str[index] != ';') {
			throw std::invalid_argument("invalid port number");
		}
		if (str[index] == ';')
			break ;
		result *= 10;
		result += (str[index] - '0');
		++index;
	}
    return (result * sign);
}

std::vector<std::string>
Server::split(std::string input, char delimiter) {
	std::vector<std::string> result;
	std::stringstream ss(input);
	std::string temp;

	while (std::getline(ss, temp, delimiter)) {
		result.push_back(temp);
	}
	return result;
}


Server::~Server() {}