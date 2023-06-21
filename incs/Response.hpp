#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "library.hpp"

// HTTP 프로토콜 버전: HTTP 프로토콜의 버전을 명시해야 합니다. 
//                  가장 최근에 사용되고 있는 버전은 HTTP/1.1입니다.
// 상태 코드: 서버가 클라이언트에게 전송한 응답의 상태를 나타내는 코드입니다. 
//          가장 많이 사용되는 상태 코드는 200(OK)와 404(Not Found)입니다.
// Content-Type: 서버가 클라이언트에게 전송한 컨텐츠의 타입을 명시합니다. 
//              이 타입은 MIME 타입 형식으로 표기되며, 
//              text/html, application/json 등이 있습니다. 특히, HTTP 1.1부터는 이 필드가 반드시 존재해야 하며, 
//              컨텐츠 타입이 명시되지 않으면 서버에서 자동으로 인식할 수 있도록 charset=utf-8과 같은 기본값이 지정됩니다.
// Content-Length: 서버가 클라이언트에게 전송한 컨텐츠의 길이를 명시합니다. 
//              이 필드가 없는 경우는 전송되는 컨텐츠의 길이가 알려져 있지 않거나, 
//              chunked 인코딩 방식으로 전송될 때입니다.
// Connection: HTTP/1.1에서는 기본적으로 keep-alive 방식이 사용되는데, 
//           이 필드를 활용하여 서버가 클라이언트에게 연결을 끊을 것인지, 
//           아니면 계속 유지할 것인지를 지시할 수 있습니다.
// Date: 응답을 생성한 서버의 현재 시간을 나타냅니다. 이 값은 RFC1123 형식으로 지정하며, GMT 시간대를 사용해야 합니다.

class   Response {
    private:
        std::string version;
        int         status_code;
        std::string content_type;
        int         content_length;
        std::string connection;
        std::string cgi_str;
        std::string return_value;
    public:
        Response(void);
        ~Response();

        //setter;
        void    setVersion(std::string param);
        void    setContentType(std::string param);
        void    setConnection(std::string param);
        void    setStatusCode(int param);
        void    setContentLength(int param);
        void    setCgiStr(std::string cgi_str);
        void    setReturnValue(std::string param);

        //getter
        std::string getVersion(void);
        std::string getContentType(void);
        std::string getConnection(void);
        int         getStatusCode(void);
        int         getContentLength(void);
        std::string getCgiStr(void);
        std::string getReturnValue(void);

        std::string     makeHeader(void);
        void            clearAll(void);
};

#endif