#include "library.hpp"

Connection::Connection() {}

Connection::Connection(uintptr_t fd, uint16_t port) : fd(fd), client_port(port) {}

Connection::~Connection() {}

void    Connection::setLastRequestTime(struct timeval param) {
    last_request_time = param;
}

uintptr_t Connection::getFd(void) {
    return fd;
}

uint16_t Connection::getClientPort(void) {
    return client_port;
}

