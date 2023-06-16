#ifndef SERVERMANAGE_HPP
#define SERVERMANAGE_HPP

#include "library.hpp"

#define HEADER_READ 0
#define BODY_READ 1
#define READ_FINISH 2
#define WRITE_READY 3
#define WRITE_FINISH 4

class ParseConfig;
class Server;
class Response;
class Request;

class ServerManage {
    private:
        std::vector<Server> servers;
        std::map<int, Request> connects;
        std::map<int, Response> responses;
        std::vector<struct kevent> changeList;
        std::map<size_t, uintptr_t>clients;
        // int max_fd;
    
    public:
        ServerManage();
        ~ServerManage();

        void    setupServer(ParseConfig param);
        void    runServer(void);
        void    change_events(uintptr_t const & ident, int16_t const & filter, uint16_t const & flags);
        void    testEchoServer(void);
        void    testServer(void);
        size_t  checkContenLength(std::string headers);
        bool    checkServerIndex(struct kevent *curr_event);
};

#endif