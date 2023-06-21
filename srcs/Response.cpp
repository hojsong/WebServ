#include "library.hpp"

Response::Response(void) {
    version = "";
    status_code = 200;
    content_type = "";
    content_length = 0;
    connection = "";
    cgi_str = "";
    return_value = "";
}

Response::~Response(void) {}

void    Response::setVersion(std::string param) {
    version = param;
}

void    Response::setContentType(std::string param) {
    content_type = param;
}

void    Response::setConnection(std::string param) {
    connection = param;
}

void    Response::setStatusCode(int param) {
    status_code = param;
}

void    Response::setContentLength(int param) {
    content_length = param;
}

void    Response::setCgiStr(std::string cgi_str){
    this->cgi_str = cgi_str;
}

void    Response::setReturnValue(std::string param) {
    this->return_value = param;
}

std::string Response::getVersion(void) {
    return version;
}

std::string Response::getContentType(void) {
    return content_type;
}

std::string Response::getConnection(void) {
    return connection;
}

int         Response::getStatusCode(void) {
    return status_code;
}

int         Response::getContentLength(void) {
    return content_length;
}

std::string Response::getCgiStr(void){
    return cgi_str;
}

std::string Response::getReturnValue(void) {
    return return_value;
}

std::string Response::makeHeader(void) {
    return "";
}

void    Response::clearAll(void) {
    version = "";
    status_code = 200;
    content_type = "";
    content_length = 0;
    connection = "";
    cgi_str = "";
    return_value = "";
}