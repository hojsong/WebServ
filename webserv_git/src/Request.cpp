#include "../includes/Request.hpp"

Request::Request(std::string& request) {
	this->_request = request;
	setRequest(request);
}

void
Request::setRequest(std::string request) {
	size_t separator = request.find("\r\n\r\n");

	std::string header;
	std::string body;
	if (separator != std::string::npos) {
		header = request.substr(0, separator);
		body = request.substr(separator + 4);
	}
	else {
		header = request;
	}

	//std::cout << "header: " << header << std::endl;
	//std::cout << "body: " << body << std::endl;
	std::vector<std::string> vec;

	vec = split(header, '\n');
	std::vector<std::string>::iterator headerIt;
	for (headerIt = vec.begin(); headerIt != vec.end(); ++headerIt) {
		if (headerIt == vec.begin()) {
			setRequestLine(headerIt);
		}
		else if (headerIt->empty()) {
			continue;
		}
		else {
			setHeader(headerIt);
		}
	}

	if (body != "") {
		setBody(body);
	}
}

void
Request::setRequestLine(std::vector<std::string>::iterator it) {
	std::vector<std::string> requestLine = split(*it, ' ');

	this->_method = requestLine[0];
	this->_uri = requestLine[1];
	this->_version = requestLine[2];
}

void
Request::setHeader(std::vector<std::string>::iterator it) {
	std::vector<std::string> headerVal;

	headerVal = split(*it, ':');
	if (headerVal.size() != 2)
		return ;
	this->_headers.insert(std::pair<std::string, std::string>(headerVal[0], headerVal[1]));
}

void
Request::setBody(std::string body) {
	this->_body = body;
}

/* Getter Functions */
std::string
Request::getMethod() const {
	return this->_method;
}

std::string
Request::getUri() const {
	return this->_uri;
}

std::string
Request::getVersion() const {
	return this->_version;
}

std::string
Request::getRequest() const {
	return this->_request;
}

std::string
Request::getBody() const {
	return this->_body;
}

std::map<std::string, std::string>
Request::getHeaders() const {
	return this->_headers;
}

/* Split Functions */
std::vector<std::string>
Request::split(std::string input, char delimiter) {
	std::vector<std::string> result;
	std::stringstream ss(input);
	std::string temp;

	while (std::getline(ss, temp, delimiter)) {
		result.push_back(temp);
	}
	return result;
}

std::vector<std::string>
Request::splitData(std::vector<std::string> buffer) {
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