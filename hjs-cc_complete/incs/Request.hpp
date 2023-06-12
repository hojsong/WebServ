#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "library.hpp"

class   Request {
    private:
        std::string method;
        std::string path;
        std::string type;
        std::string host;
        std::vector<std::string>    accept;
        std::string connection;
        std::string body;
    public:
        Request(void);
        Request(std::vector<std::string> param);
        ~Request();

        //setter
        void    setMethod(std::string param);
        void    setPath(std::string param);
        void    setType(std::string param);
        void    setHost(std::string param);
        void    setAccept(std::vector<std::string> param);
        void    setConnection(std::string param);
        void    setBody(std::string param);

        //getter
        std::string                 getMethod(void);
        std::string                 getPath(void);
        std::string                 getType(void);
        std::string                 getHost(void);
        std::vector<std::string>    getAccept(void);
        std::string                 getConnection(void);
        std::string                 getBody(void);

        std::string     makeHeader(void);

};

#endif