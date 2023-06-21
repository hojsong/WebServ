#include "library.hpp"

Location::Location(void) {
    path = "";
    root = "";
    index = "";
    return_value = "";
    alias = "";
    client_max_body_size = 2147483647;
    autoindex = false;
    methods.resize(5);
    methods[0] = 1;
    methods[1] = 0;
    methods[2] = 0;
    methods[3] = 0;
    methods[4] = 0;
}

Location::~Location() {}

//setter

void	Location::setPath(std::string param) {
    size_t pos = param.find(';');
    if (pos != std::string::npos)
        param = param.substr(0, pos);
    if (param.find('/') == std::string::npos)
        throw ErrorException("wrong path format");
    path = param;
}

void	Location::setRoot(std::string param) {
    if (root != "")
        throw ErrorException("location root duplicated");
    size_t pos = param.find(';');
    if (pos != std::string::npos)
        param = param.substr(0, pos);
    root = param;
}

void	Location::setAutoindex(std::string param) {
    size_t pos = param.find(';');
    if (pos != std::string::npos)
        param = param.substr(0, pos);
    if (param.compare("on") == 0)
        autoindex = true;
    else
        autoindex = false;
}

void	Location::setIndex(std::string param) {
    size_t pos = param.find(';');
    if (pos != std::string::npos)
        param = param.substr(0, pos);
    index = param;
}

void	Location::setMethods(std::vector<std::string> param) {
    if (param.size() == 0)
        return ;
    std::vector<int>    buffer;

    buffer.resize(5);
    buffer[0] = 1;
    buffer[1] = 0;
    buffer[2] = 0;
    buffer[3] = 0;
    buffer[4] = 0;

    for (size_t i = 0; i < param.size(); i++) {
        std::size_t pos = param[i].find(';');
        if (pos != std::string::npos)
            param[i].erase(pos, 1);
        if (param[i] == "GET")
            buffer[0]++;
        else if (param[i] == "POST")
            buffer[1]++;
        else if (param[i] == "DELETE")
            buffer[2]++;
        else if (param[i] == "PUT")
            buffer[3]++;
        else if (param[i] == "HEAD")
            buffer[4]++;
        else
            throw ErrorException("wrong method argument");
    }
    methods = buffer;
}

void	Location::setReturnValue(std::string param) {
    if (return_value != "")
        throw ErrorException("location return_value duplicated");
    size_t pos = param.find(';');
    if (pos != std::string::npos)
        param = param.substr(0, pos);
    return_value = param;
}

void	Location::setAlias(std::string param) {
    alias = param;
}

void    Location::setTryFiles(std::vector<std::string>  param) {
    try_files = param;
}

void	Location::setCgiPath(std::vector<std::string> param) {
    cgi_path = param;
}

void	Location::setCgiText(std::vector<std::string> param) {
    cgi_text = param;
}

void	Location::setClntMaxBodySize(std::string param) {
    client_max_body_size = std::atoi(param.c_str());
}

//getter

std::string	Location::getPath(void) {
    return path;
}

std::string	Location::getRoot(void) {
    return root;
}

bool	Location::getAutoindex(void) {
    return autoindex;
}

std::string	Location::getIndex(void) {
    return index;
}

std::vector<int>	Location::getMethods(void) {
    return methods;
}

std::string	Location::getReturnValue(void) {
    return return_value;
}

std::string	Location::getAlias(void) {
    return alias;
}

std::vector<std::string>    Location::getTryFiles(void) {
    return try_files;
}

std::vector<std::string>	Location::getCgiPath(void) {
    return cgi_path;
}

std::vector<std::string>	Location::getCgiText(void) {
    return cgi_text;
}

unsigned long	Location::getClntMaxBodySize(void) {
    return client_max_body_size;
}

void    Location::print(void) {
    std::cout << "------------Location--------------" << std::endl;
    std::cout << "path: " << path << std::endl;
    std::cout << "root: " << root << std::endl;
    std::cout << "index: " << index << std::endl;
    std::cout << "return_value" << return_value << std::endl;
    std::cout << "alias: " << alias << std::endl;
    std::cout << "autoindex: " << autoindex << std::endl;
    std::cout << "max body size: " << client_max_body_size << std::endl;
    std::cout << "methods: " << std::endl;
    std::cout << "\tGET: " << methods[0] << std::endl;
    std::cout << "\tPOST: " << methods[1] << std::endl;
    std::cout << "\tDELETE: " << methods[2] << std::endl;
    std::cout << "\tPUT: " << methods[3] << std::endl;
    std::cout << "\tHEAD: " << methods[4] << std::endl;
    std::cout << "cgi_path" << std::endl;
    for (std::vector<std::string>::iterator iter = cgi_path.begin(); iter != cgi_path.end(); iter++)
        std::cout << "\t" << *iter << std::endl;
    std::cout << "cgi_text" << std::endl;
    for (std::vector<std::string>::iterator iter = cgi_text.begin(); iter != cgi_text.end(); iter++)
        std::cout << "\t" << *iter << std::endl;
    
}