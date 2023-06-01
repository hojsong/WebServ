#include "library.hpp"

Connection::Connection(int fd, int port, std::string ip) : fd(fd), client_port(port), client_ip(ip) {}

Connection::~Connection() {}

void    Connection::setLastRequestTime(struct timeval param) {
    last_request_time = param;
}

int Connection::getFd(void) {
    return fd;
}

int Connection::getClientPort(void) {
    return client_port;
}

std::string Connection::getClientIp(void) {
    return client_ip;
}
