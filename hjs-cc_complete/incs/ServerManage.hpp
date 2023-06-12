#ifndef SERVERMANAGE_HPP
#define SERVERMANAGE_HPP

#include "library.hpp"

class Server;
class ParseConfig;

class ServerManage {
    private:
        std::vector<Server> serv;
        // int max_fd;
    
    public:
        ServerManage();
        ~ServerManage();

        void    setupServer(ParseConfig param);
        void    runServer(void);
        void    testEchoServer(void);
        void    testServer(void);
};

#endif