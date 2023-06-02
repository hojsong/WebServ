#include "../includes/ConfigParse.hpp"

/* Main Part */
void
ConfigParse::getData(std::string file) {
	if (file != "default") {
		std::ifstream fin;

		fin.open(file);
		if (fin.fail()) {
			perror("file open error!");
			return ;
		}
		fin.close();
	}
	this->setData("config/default.conf");
	//printParseData();
}

/* Setting Functions */
void
ConfigParse::setData(std::string filePath) {
	std::ifstream fin(filePath);
	std::string fileContents;
	std::stringstream buf;

	buf << fin.rdbuf();
	fileContents = buf.str();
	this->_blocks = splitServerBlocks(fileContents);
	std::vector<std::string>::iterator it;
	for (it = this->_blocks.begin(); it != this->_blocks.end(); ++it)
	{
		setServerBlock(it);
	}
}

void
ConfigParse::setServerBlock(std::vector<std::string>::iterator it) {
	if (checkVaildate(it) == true) {
		Server newServer;

		newServer.setPortNumber(this->_portNumber);
		newServer.setServerName(this->_serverName);
		newServer.setLocations(this->_locations);
		this->_servers.push_back(newServer);
		this->_locations.clear();
	}
	else {
		return ;
	}
}

/* Validate Check Part */
bool
ConfigParse::checkVaildate(std::vector<std::string>::iterator it) {
	std::vector<std::string> vec;
	bool listenFlag = false;
	bool servernameFlag = false;
	bool locationFlag = false;

	this->_contents = split(*it, '\n');
	std::vector<std::string>::iterator contentsIt = this->_contents.begin();
	while (contentsIt != this->_contents.end() && *contentsIt != "{") {
		++contentsIt;
	}

	if (contentsIt == this->_contents.end())
		return false;
	
	++contentsIt;

	while (contentsIt != this->_contents.end() && *contentsIt != "}") {
		if (contentsIt->find("listen") != std::string::npos) {
			listenFlag = true;
			contentsIt = isValidPort(contentsIt);
			if (contentsIt == this->_contents.end()) {
				std::cerr << "Invalid Port Number." << std::endl;
				return false;
			}
		}
		else if (contentsIt->find("server_name") != std::string::npos) {
			servernameFlag = true;
			contentsIt = isValidHostName(contentsIt);
			if (contentsIt == this->_contents.end()) {
				std::cerr << "Invalid ServerName." << std::endl;
				return false;
			}
		}
		else if (contentsIt->find("location") != std::string::npos) {
			Location location;
			locationFlag = true;

			contentsIt = isValidLocation(contentsIt, location);
			if (contentsIt == this->_contents.end()) {
				return false;
			}
			this->_locations.push_back(location);
		}
		++contentsIt;
	}

	if (listenFlag && servernameFlag && locationFlag)
		return true;
	else
		return false;
}

std::vector<std::string>::iterator
ConfigParse::isValidPort(std::vector<std::string>::iterator it) {

	std::vector<std::string> vec;
	
	while (it != this->_contents.end()) {
		vec = split(*it, ' ');
		vec = splitData(vec);
		if (vec.size() != 2)
			return this->_contents.end();
		std::string port = vec.back();
		int num = myStoi(port);
		if (num < 1024 || 0 > 65535)
			return this->_contents.end();
		if (it->find(";") != std::string::npos) {
			this->_portNumber = num;
			break ;
		}
		++it;
	}

	return it;
}

std::vector<std::string>::iterator
ConfigParse::isValidHostName(std::vector<std::string>::iterator it) {
	std::vector<std::string> vec;
	
	while (it != this->_contents.end()) {
		vec = split(*it, ' ');
		vec = splitData(vec);
		if (vec.size() != 2)
			return this->_contents.end();
		std::string hostname = vec.back();
		hostname.pop_back();
		if (hostname.length() > 255) {
			return this->_contents.end();
		}
		for (size_t i = 0; i < hostname.length(); i++) {
			char c = hostname[i];

			if (!std::isalnum(c) && c != '-') {
				return this->_contents.end();
			}
		}
		if (it->find(";") != std::string::npos) {
			this->_serverName = hostname;
			break ;
		}
		++it;
	}

	return it;
}

std::vector<std::string>::iterator
ConfigParse::isValidLocation(std::vector<std::string>::iterator it, Location& location) {
	std::vector<std::string> vec;
	
	while (it != this->_contents.end()) {
		vec = split(*it, ' ');
		vec = splitData(vec);
		if (vec.front() == "location") {
			if (vec.size() != 3 || vec[2] != "{") {
				return this->_contents.end();
			}
			vec[1] = this->_uriPath;
		}
		else if (vec.front() != "}" && vec.back().find(";") != std::string::npos) {
			if (vec.size() != 2) {
				return this->_contents.end();
			}
			location.addLocation(vec);
		}
		else if (vec.front() != "}" && vec.back().find(";") == std::string::npos) {
			return this->_contents.end();
		}
		else if (vec.front() == "}") {
			break ;
		}
		
		++it;
	}

	return it;
}

/* Getter Part */
std::vector<Server>
ConfigParse::getServers() const {
	return this->_servers;
}

std::vector<Location>
ConfigParse::getLocations() const {
	return this->_locations;
}

/* Library Functions */
int
ConfigParse::myStoi(const std::string& str) {
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

int
ConfigParse::countChar(const std::string& str, char ch) {
    int count = 0;
	
	for (size_t i = 0; i < str.length(); i++) {
		if (ch == str[i])
			count++;
	}
    return count;
}

std::vector<std::string>
ConfigParse::split(std::string input, char delimiter) {
	std::vector<std::string> result;
	std::stringstream ss(input);
	std::string temp;

	while (std::getline(ss, temp, delimiter)) {
		result.push_back(temp);
	}
	return result;
}

std::vector<std::string>
ConfigParse::splitServerBlocks(std::string input) {
    std::vector<std::string> result;
    std::stringstream ss(input);
    std::string temp, currentBlock;
    bool inServerBlock = false;

    while (std::getline(ss, temp)) {
		if (temp.empty())
			continue;
        if (temp == "server") {
            if (inServerBlock) {
				currentBlock.pop_back();
                result.push_back(currentBlock);
            }
            currentBlock = temp + '\n';
            inServerBlock = true;
        }
        else if (inServerBlock) {
            	currentBlock += temp + '\n';
        }
    }

    if (inServerBlock) {
		currentBlock.pop_back();
        result.push_back(currentBlock);
    }

    return result;
}

std::vector<std::string>
ConfigParse::splitData(std::vector<std::string> buffer) {
	std::vector<std::string> result;

	for (size_t i = 0; i < buffer.size(); i++) {
		std::string line = buffer[i];
		std::string lineBuf;
		for (size_t j = 0; j < line.length(); j++) {
			char ch = line[j];
			if (ch != ' ' && ch != '\n' && ch != '\t') {
				lineBuf += ch;
			}
			else {
				if (!lineBuf.empty()) {
					result.push_back(lineBuf);
					lineBuf.clear();
				}
			}
		}
		if (!lineBuf.empty()) {
            result.push_back(lineBuf);
            lineBuf.clear();
        }
	}
	return (result);
}

void
ConfigParse::printParseData(void) const {
	std::cout << "----------------Print----------------" << std::endl;
	std::cout << "Config File Information" << std::endl;
	for (size_t i = 0; i < this->_blocks.size(); ++i) {
		std::cout << "Server Block" << i << std::endl;
		std::cout << this->_blocks[i] << std::endl;
	}
	for (size_t j = 0; j < this->_servers.size(); ++j) {
		this->_servers[j].printServerInfo();
	}
	std::cout << "-------------------------------------" << std::endl;
}