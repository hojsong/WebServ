#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include "library.hpp"

class Connection
{
private:
    int             fd;
    int             client_port;
    std::string     client_ip;
    struct timeval  last_request_time;
    
public:
    Connection(int fd, int port, std::string ip);
    ~Connection();

    //getter
    int             getFd(void);
    int             getClientPort(void);
    std::string     getClientIp(void);
    struct timeval  getLastRequestTime(void);

    //setter
    void    setLastRequestTime(struct timeval param);
};

#endif