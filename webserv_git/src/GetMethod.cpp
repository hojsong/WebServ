#include "../includes/GetMethod.hpp"

GetMethod::GetMethod() {}

GetMethod::GetMethod(std::string filePath) {
	this->_filePath = filePath;
}

void
GetMethod::setStatus(int code) {
	this->_status << code;
}

void
GetMethod::makeBody() {
	std::ifstream file(this->_filePath);
	if (file.is_open()) {
		std::stringstream buffer;

		this->_status << OK;
		buffer << file.rdbuf();
		this->_body = buffer.str();

		this->_headers = "HTTP/1.1 " + this->_status.str() + " OK\r\n"
						"Content-Type: text/html\r\n"
						"Content-Length: " + std::to_string(this->_body.size()) + "\r\n"
						"Connection: close\r\n"
						"\r\n";
		this->_response = this->_headers + this->_body;
	}
	else {
		this->_status << Not_Found;
		this->_body = "<html><body><h1>404 Not Found</h1></body></html>";
		this->_headers = "HTTP/1.1 " + this->_status.str() + " Not Found\r\n"
						"Content-Type: text/html\r\n"
						"Content-Length: " + std::to_string(this->_body.size()) + "\r\n"
						"Connection: close\r\n"
						"\r\n";
		this->_response = this->_headers + this->_body;
	}
}

std::string
GetMethod::getResponse() const {
	return this->_response;
}

GetMethod::~GetMethod() {}