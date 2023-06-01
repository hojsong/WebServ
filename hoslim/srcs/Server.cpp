#include "library.hpp"

Server::Server() {
    server_name = "";
    root = "";
    index = "";
    port = 0;
    host = 0;
    client_max_body_size = 2147483647;
    autoindex = false;
    listen_fd = 0;
    initErrorPages();
    initErrorPagesHeader();
}

Server::~Server() {}

//setter
void    Server::setServerName(std::string param) {
    size_t pos = param.find(';');
    if (pos != std::string::npos)
        param = param.substr(0, pos);
    server_name = param;
}

void    Server::setRoot(std::string param) {
    size_t pos = param.find(';');
    if (pos != std::string::npos)
        param = param.substr(0, pos);
    root = param;
}

void    Server::setIndex(std::string param) {
    size_t pos = param.find(';');
    if (pos != std::string::npos)
        param = param.substr(0, pos);
    index = param;
}

void    Server::setPort(std::string param) {
    size_t pos = param.find(';');
    if (pos != std::string::npos)
        param = param.substr(0, pos);
    port = std::atol(param.c_str());
}

void    Server::setHost(std::string param) {
    size_t pos = param.find(';');
    if (pos != std::string::npos)
        param = param.substr(0, pos);
    host = std::atol(param.c_str());
}

void    Server::setClntMaxBodySize(std::string param) {
    size_t pos = param.find(';');
    if (pos != std::string::npos)
        param = param.substr(0, pos);
    char* end = NULL;
    client_max_body_size = std::strtoul(param.c_str(), &end, 10);
}

void    Server::setAutoindex(std::string param) {
    size_t pos = param.find(';');
    if (pos != std::string::npos)
        param = param.substr(0, pos);
    if (param.compare("true"))
        autoindex = true;
    else
        autoindex = false;
}


void    Server::setErrorPages(std::vector<std::string> param) {
    std::map<int, std::string> tmp;
    int idx;
    std::string path;
    std::string error_message_path;
    std::string error_massege;

    if (param.size() == 0)
        return ;
    for (size_t i = 0; i < param.size(); i++) {
        idx = std::atoi(param[i++].c_str());
        if (i == param.size())
            throw ErrorException("error page isn't pair");
        size_t pos = param[i].find(';');
        if (pos != std::string::npos)
            param[i] = param[i].substr(0, pos);
        path = param[i];
        if (path[0] != '/')
            path = "/" + path;
        error_message_path = "./" + root + path;

        std::ifstream file(error_message_path);
        std::ostringstream  os;
        std::string line;
        while (std::getline(file, line))
            os << line;
        tmp.insert(std::pair<int, std::string>(idx, os.str()));
    }
    error_pages = tmp;
}

void    Server::setListenFd(std::string param) {
    size_t pos = param.find(';');
    if (pos != std::string::npos)
        param = param.substr(0, pos);
    listen_fd = std::atoi(param.c_str());
}

void    Server::setLocation(std::vector<Location> locs) {
    if (locs.size() == 0)
        return ;
    locations = locs;
}

void    Server::setServerAddress(void) {
    std::memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(port);
}

//getter
std::string    Server::getServerName(void) {
    return server_name;
}

std::string    Server::getRoot(void) {
    return root;
}

std::string    Server::getIndex(void) {
    return index;
}

uint16_t    Server::getPort(void) {
    return port;
}

in_addr_t    Server::getHost(void) {
    return host;
}


unsigned long    Server::getClntMaxBodySize(void) {
    return client_max_body_size;
}

bool    Server::getAutoindex(void) {
    return autoindex;
}

std::map<int, std::string>    Server::getErrorPages(void) {
    return error_pages;
}

std::map<int, std::string>    Server::getErrorPagesHeader(void) {
    return error_pages_header;
}


int    Server::getListenFd(void) {
    return listen_fd;
}

std::vector<Location>   Server::getLocations(void) {
    return locations;
}

struct sockaddr_in*  Server::getServerAddress(void) {
    return &server_address;
}

void    Server::initErrorPages(void)
{
    error_pages[200] = "";
	error_pages[301] = "";
	error_pages[302] = "";
	error_pages[400] = "";
	error_pages[401] = "";
	error_pages[402] = "";
	error_pages[403] = "";
	error_pages[404] = "";
	error_pages[405] = "";
	error_pages[406] = "";
	error_pages[500] = "";
	error_pages[501] = "";
	error_pages[502] = "";
	error_pages[503] = "";
	error_pages[505] = "";
	error_pages[505] = "";
}

void    Server::initErrorPagesHeader(void)
{
	error_pages_header[200] = "HTTP/1.1 200 OK";
	error_pages_header[301] = "HTTP/1.1 301 Moved Permanently";
	error_pages_header[302] = "HTTP/1.1 302 Found";
	error_pages_header[400] = "HTTP/1.1 400 Bad Request";
	error_pages_header[401] = "HTTP/1.1 401 Unauthorized";
	error_pages_header[402] = "HTTP/1.1 402 Payment Required";
	error_pages_header[403] = "HTTP/1.1 403 Forbidden";
	error_pages_header[404] = "HTTP/1.1 404 Not Found";
	error_pages_header[405] = "HTTP/1.1 405 Method Not Allowed";
	error_pages_header[406] = "HTTP/1.1 406 Not Acceptable";
	error_pages_header[500] = "HTTP/1.1 500 Internal Server Error";
	error_pages_header[501] = "HTTP/1.1 501 Not Implemented";
	error_pages_header[502] = "HTTP/1.1 502 Bad Gateway";
	error_pages_header[503] = "HTTP/1.1 503 Service Unavailable";
	error_pages_header[505] = "HTTP/1.1 504 Gateway Timeout";
	error_pages_header[505] = "HTTP/1.1 505 HTTP Version Not Supported";
}

void    Server::print(void) {
    std::cout << "server name :" << server_name << std::endl;
    std::cout << "root :" << root << std::endl;
    std::cout << "index :" << index << std::endl;
    std::cout << "port :" << port << std::endl;
    std::cout << "host :" << host << std::endl;
    std::cout << "max body size :" << client_max_body_size << std::endl;
    std::cout << "autoindex :" << autoindex << std::endl;
    std::cout << "listen fd :" << listen_fd << std::endl;
    std::cout << "server addr :"  << std::endl;
    std::cout << "{"  << std::endl;
    std::cout << "\tsin_family" << server_address.sin_family  << std::endl;
    std::cout << "\tsin_port" << server_address.sin_port  << std::endl;
    std::cout << "\ts_addr" << server_address.sin_addr.s_addr  << std::endl;
    std::cout << "}"  << std::endl;
    std::cout << "error pages :"  << std::endl;
    std::cout << "{"  << std::endl;
    for (std::map<int, std::string>::iterator iter = error_pages.begin(); iter != error_pages.end(); iter++)
        std::cout << "\t" << (*iter).first << " " << (*iter).second << std::endl;
    std::cout << "}"  << std::endl;

    int idx = 0;
    for (std::vector<Location>::iterator iter = locations.begin(); iter != locations.end(); iter++) {
        std::cout << idx++; 
        (*iter).print();
    }

}