#include "library.hpp"
Request::Request(void) {
    method = "";
    path = "";
    type = "";
    host = "";
    connection = "";
    body = "";
}

Request::Request(std::vector<std::string> param) {
    if (param[0] == "GET" || param[0] == "POST" || param[0] == "DELETE" \
    || param[0] == "PUT" || param[0] == "HEAD")
        method = param[0];
    else
        method = "";

    path = param[1];

    type = param[2];
    
    for (size_t i = 3; i < param.size(); i++) {
        if (param[i] == "Host:")
            host = param[++i];
        else if (param[i] == "Connection:")
            connection = param[++i];
        else if (param[i] == "Accept") {
            std::vector<std::string>    accept_args = splitArgs(param[++i], ",");
            accept = accept_args;
        }
    }
}

Request::~Request(void) {}

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

void    Request::setAccept(std::vector<std::string> param) {
    accept = param;
}

void    Request::setConnection(std::string param) {
    connection = param;
}

void    Request::setBody(std::string param) {
    body = param;
}

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

std::vector<std::string>   Request::getAccept(void) {
    return accept;
}

std::string Request::getConnection(void) {
    return connection;
}

std::string Request::getBody(void) {
    return body;
}

std::string Request::makeHeader(void) {
    return "";
}

