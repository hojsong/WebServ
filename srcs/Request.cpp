#include "library.hpp"
Request::Request(void) {
    state = HEADER_READ;
    headers = "";
    method = "";
    path = "";
    type = "";
    host = "";
    connection = "";
    body = "";
    requestStart = NULL;
}

Request::~Request(void) {}

//setter
void    Request::setMethod(std::string param) {
    method = param;
}

void    Request::setPath(std::string param) {
    path = param;
}

void    Request::setType(std::string param) {
    type = param;
}

void    Request::setHost(std::string param) {
    host = param;
}

void    Request::setHeaderData(std::map<std::string, std::string> src_data) {
    header_data = src_data;
}

void    Request::setConnection(std::string param) {
    connection = param;
}

void    Request::setBody(std::string param) {
    body = param;
}

void    Request::setState(int state) {
    this->state = state;
}

void    Request::setServerFd(uintptr_t serv_fd) {
    this->serv_fd = serv_fd;
}

void    Request::setTime(void){
    struct timeval start;

     if (requestStart == NULL) {
        gettimeofday(&start, NULL);
        requestStart = &start;
    }
}

//getter
std::string Request::getMethod(void) {
    return method;
}

std::string Request::getPath(void) {
    return path;
}

std::string Request::getType(void) {
    return type;
}

std::string Request::getHost(void) {
    return host;
}

std::map<std::string, std::string> Request::getHeaderData(void) {
    return header_data;
}

std::string Request::getConnection(void) {
    return connection;
}

std::string Request::getBody(void) {
    return body;
}

std::string Request::getHeaders(void) {
    return headers;
}

int Request::getState(void) {
    return state;
}

std::string Request::makeHeader(void) {
    return "";
}

uintptr_t    Request::getServerFd(void) {
    return serv_fd;
}

struct timeval  *Request::getTime(void){
    return (requestStart);
}

void    Request::appendHeader(std::string data) {
    this->headers.append(data);
}

void    Request::appendBody(std::string data) {
    this->body.append(data);
}

void    Request::clearAll(void) {
    state = HEADER_READ;
    headers = "";
    method = "";
    path = "";
    type = "";
    host = "";
    connection = "";
    body = "";
    requestStart = NULL;
}
