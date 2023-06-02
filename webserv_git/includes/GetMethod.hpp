#ifndef __GETMETHOD_HPP__
#define __GETMETHOD_HPP__

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>

/* Status Meaning */
// 1xx(정보) : 요청을 받았으며 프로세스를 계속 진행함
// 2xx(성공) : 요청을 성공적으로 받았으며 인식했고 수용했음
// 3xx(리다이렉션) : 요청 완료를 위해 추가 작업 조치가 필요함
// 4xx(클라이언트 오류) : 요청의 문법이 잘못되었거나 요청을 처리할 수 없음
// 5xx(서버 오류) : 서버가 명백히 유효한 요청에 대해 충족을 실패함

enum Status {
    Continue = 100, // 진행 중
    OK = 200, // 요청이 성공
    Created = 201, // 요청이 성공했으며 그 결과로 새로운 리소스 생성
    Accepted = 202, // 요청을 수신하였지만 응답 불가
    Non_Authoritative_Information = 203, // ?
    No_Content = 204, // 요청에 대해서 보내줄 수 있는 콘텐츠는 없지만 헤더는 있음
    Reset_Content = 205, // 요청을 완료한 이후 이 요청을 보낸 문서 뷰를 리셋하라고 알려줌
    Partial_Content = 206, // ?
    Multiple_Choices = 300, // 요청에 대해서 하나 이상의 응답이 가능함. 클라이언트는 그 중 하나를 반드시 선택
    Moved_Permanently = 301, // 요청한 리소스의 URI가 변경. 새로운 URI 응답
    Found = 302, // 요청한 리소스의 URI가 일시적으로 변경. 새로운 URI는 나중에 만들어질 수 있음
    See_Other = 303, // 클라이언트가 요청한 리소스를 다른 URI에서 GET 요청을 통해 얻어야할 때, 서버가 클라이언트로 직접 보내는 응답
    Not_Modified = 304, // 캐시를 목적으로 사용
    Use_Proxy = 305, // 요청한 응답은 반드시 프록시를 통해서 접속해야 함
    Temporary_Redirect = 307, // ?
    Bad_Request = 400, // 잘못된 요청
    Unauthorized = 401, // 요청한 응답을 받기 위해 인증이 필요함
    Forbidden = 403, // 클라이언트가 콘텐츠에 접근할 권리를 가지고 있지 않음. 401과 다른 점은 서버가 클라이언트가 누구인지 알고 있음
    Not_Found = 404, // 요청받은 리소스를 찾을 수 없음
    Method_Not_Allowed = 405, // 없는 메소드를 요청했을 때
    Not_Acceptable = 406, // ?
    Proxy_Authentication_Required = 407, // 401과 비슷하지만 프록시에 의해 완료된 인증이 필요
    Request_Timeout = 408, // Timeout 시 발생
    Conflict = 409, // 요청이 현재 서버의 상태와 충돌될 때
    Gone = 410, // 요청한 컨텐츠가 서버에서 영구적으로 삭제되었으며, 전달해 줄 수 있는 주소 역시 존재하지 않음
    Length_Required = 411, // 서버에서 필요로 하는 Content-Length 헤더 필드가 정의되지 않은 요청이 들어와 거절
    Precondition_Failed = 412, // 클라이언트 헤더가 서버의 전제조건에 적절하지 않음
    Request_Entity_Too_Large = 413, // 요청 엔티티가 서버에서 정의한 하계보다 큼. 연결을 끊거나 Retry-After 헤더 필드로 응답
    Request_URI_Too_Long = 414, // 클라이언트가 요청한 URI가 서버에서 처리하지 않기로 한 길이보다 김
    Unsupported_Media_Type = 415, // 요청한 미디어 포맷이 서버에서 지원하지않음. 거절
    Requested_Range_Not_Satisfiable = 416, // Range 헤더 필드에 요청한 지정 범위를 만족시킬 수 없음.
    Expectation_Failed = 417, // Except요청 헤더 필드로 요청한 예상이 서버에서는 적당하지 않음
    Internal_Server_Error = 500, // 웹 사이트 서버에 문제가 있음. 정확한 문제에 대한 설명 X
    Not_Implemented = 501, // 서버가 요청을 이행하는 데 필요한 기능을 지원하지 않음
    Bad_Gateway = 502, // 서버가 게이트웨이로부터 잘못된 응답을 수신. 인터넷 상의 서버가 다른 서버로부터 유효하지 않은 응답을 받은 경우
    Service_Unavailable = 503, // 서버가 유지보수를 위해 작동이 중단되거나, 과부하가 걸려 요청 처리할 준비가 되지 않은 경우
    Gateway_Timeout = 504, // 웹 페이지를 로드하거나 브라우저에서 다른 요청을 채우려는 동안 한 서버가 엑세스하고 있는 다른 서버에서 적시에 응답을 받지 못했음
    HTTP_Version_Not_Supported = 505 // 서버에서 지원되지 않는 HTTP 버전을 요청받음
};

class GetMethod
{
	private:
		std::stringstream _status;
        std::string _uri;
		std::string _headers;
		std::string _body;
		std::string _response;
		std::string _filePath;
	public:
		GetMethod();
		GetMethod(std::string filePath);
		~GetMethod();
		void setStatus(int code);
		void makeBody();
		std::string getResponse() const;
};

#endif