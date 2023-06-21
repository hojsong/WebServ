#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include "library.hpp"

class Connection
{
private:
    uintptr_t       fd;
    uint16_t        client_port;
    struct timeval  last_request_time;
    
public:
    Connection();
    Connection(uintptr_t fd, uint16_t port);
    ~Connection();

    //getter
    uintptr_t       getFd(void);
    uint16_t        getClientPort(void);
    struct timeval  getLastRequestTime(void);

    //setter
    void    setLastRequestTime(struct timeval param);
};

#endif