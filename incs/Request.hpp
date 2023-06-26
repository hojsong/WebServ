#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "library.hpp"

#define HEADER_READ 0
#define BODY_READ 1
#define READ_FINISH 2

class   Request {
    private:
        uintptr_t   serv_fd;
        int         state;
        std::string headers;
        std::string method;
        std::string path;
        std::string type;
        std::string host;
        std::map<std::string, std::string> header_data;
        std::string connection;
        std::string body;
        struct timeval *requestStart;

    public:
        Request(void);
        ~Request();

        //setter
        void    setMethod(std::string param);
        void    setPath(std::string param);
        void    setType(std::string param);
        void    setHost(std::string param);
        void    setHeaderData(std::map<std::string, std::string> header_data);
        void    setConnection(std::string param);
        void    setBody(std::string param);
        void    setState(int state);
        void    setServerFd(uintptr_t serv_fd);
        void    setTime(void);

        //getter
        std::string                         getMethod(void);
        std::string                         getPath(void);
        std::string                         getType(void);
        std::string                         getHost(void);
        std::map<std::string, std::string>  getHeaderData(void);
        std::string                         getConnection(void);
        std::string                         getHeaders(void);
        std::string                         getBody(void);
        int                                 getState(void);
        uintptr_t                           getServerFd(void);
        struct timeval                      *getTime(void);

        std::string     makeHeader(void);
        void            appendHeader(std::string data);
        void            appendBody(std::string data);
        void            clearAll(void);
};

#endif