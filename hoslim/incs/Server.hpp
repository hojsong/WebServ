#ifndef SERVER_HPP
#define SERVER_HPP

#include "library.hpp"

static std::string serverParams[] = {"server_name", "listen", "root", "index", "allow_methods", "client_body_buffer_size"};

class Location;

class Server {
    private:
        // Connection connect;
        std::vector<Location>   locations;

        std::string                 server_name;
        std::string                 root;
        std::string                 index;
        uint16_t                    port;
        in_addr_t                   host;
        unsigned long               client_max_body_size;
        bool                        autoindex;
        std::map<int, std::string>  error_pages;
        std::map<int, std::string>  error_pages_header;
        struct sockaddr_in          server_address;
        int                         listen_fd;
    public:
        Server();
        ~Server();

        //setter
        void    setServerName(std::string param);
        void    setRoot(std::string param);
        void    setIndex(std::string param);
        void    setPort(std::string param);
        void    setHost(std::string param);
        void    setClntMaxBodySize(std::string param);
        void    setAutoindex(std::string param);
        void    setErrorPages(std::vector<std::string> param);
        void    setListenFd(std::string param);
        // void    setTryFiles(std::vector<std::string> param);
        void    setLocation(std::vector<Location> params);
        void    setServerAddress(void);

        //getter
        std::string                     getServerName(void);
        std::string                     getRoot(void);
        std::string                     getIndex(void);
        uint16_t                        getPort(void);
        in_addr_t                       getHost(void);
        unsigned long                   getClntMaxBodySize(void);
        bool                            getAutoindex(void);
        std::map<int, std::string>      getErrorPages(void);
        std::map<int, std::string>      getErrorPagesHeader(void);
        int                             getListenFd(void);
        // std::vector<std::string>        getTryFiles(void);
        std::vector<Location>           getLocations(void);
        struct sockaddr_in*             getServerAddress(void);

        void    initErrorPages(void);
        void    initErrorPagesHeader(void);
        void    print(void);
};

#endif