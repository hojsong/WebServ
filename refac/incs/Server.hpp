#ifndef SERVER_HPP
#define SERVER_HPP

#include "library.hpp"

static std::string serverParams[] = {"server_name", "listen", "root", "index", "allow_methods", "client_body_buffer_size"};

class Location;
class Connection;
class Request;
class   MemberRepository;

class Server {
    private:
        std::vector<Location>   locations;

        // std::queue<Request>       requests;
        //setter, getter
        MemberRepository            *memberRepository;
        std::string                 server_name;
        std::string                 root;
        std::string                 index;
        uint16_t                    port;
        int                         clnt_fd;
        in_addr_t                   host;
        unsigned long               client_max_body_size;
        bool                        autoindex;
        std::map<int, std::string>  error_pages;
        std::map<int, std::string>  error_pages_header;
        uintptr_t                   server_socket; // listen_fd를 조금 더 직관적으로 변경
        struct sockaddr_in          server_address; // 서버 별 run을 위한 인자 추가
        sockaddr_in                 server_addr; // 서버 별 run을 위한 인자 추가
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
        void    setServerSocket(std::string param);
        void    setTryFiles(std::vector<std::string> param);
        void    setLocation(std::vector<Location> params);
        void    setServerAddress(void);
        void    setClntFd(int fd);
        // void    setConnection(int idx, Connection param);
        void    setRequest(Request param);
        void    setMemberRepository(MemberRepository *memberRepository);

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
        uintptr_t                       getServerSocket(void);
        std::vector<std::string>        getTryFiles(void);
        std::vector<Location>           getLocations(void);
        struct sockaddr_in*             getServerAddress(void);
        // std::map<int, Connection>       getConnect(void);
        std::queue<Request>             getRequests(void);
        int                             getClntFd(void);
        MemberRepository                *getMemberRepository(void);


        void    run(void);
        void    initErrorPages(void);
        void    initErrorPagesHeader(void);
        void    print(void);
};

#endif