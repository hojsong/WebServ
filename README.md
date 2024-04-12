# 42Seoul Webserv 과제

### 과제 요약
- C++98 을 이용한 HTTP 서버 구현
- config 파일 작성 및 이를 이용한 서버 설정
- poll, epoll, kqueue 중 1개를 이용하여 non-blocking 서버 작성
- 클라이언트는 파일을 업로드 할 수 있어야 함
- GET, POST, DELETE 요청에 대해 필수적으로 응답할 수 있어야 함
- python, php 등을 이용하여 CGI 프로그램 작성

### 과제 인원
3명(seongmpa, hoslim, hojsong), 기여도(33%, 서버 설정 및 HTTP 요청 수신과 응답 생성. 이미지 파일 CGI 프로그램 적용)

## config 파일 예시
> 포트, 서버 이름, index 페이지 설정
> Location 블록 별로 허용 메소드 및 autoindex 기능 설정

### Main 로직

- 인자로 파일 위치를 받을 경우, 그 파일을 설정 파일로 사용
- 인자가 존재하지 않을 시 deault 설정 파일 세팅

### 서버 설정

- setter 함수를 이용하여 객체 내부 설정

### 서버 실행

- non-blocking 처리를 위해 fcntl 함수에서 O_NONBLOCK 옵션 사용

### 메인 로직
- Kqueue를 이용하여 이벤트 수신 처리
- 서버 소켓으로 첫 요청이 올 경우, accept 이후 client 소켓을 std::map으로 관리
- Read 이벤트 발생 시, Request 요청 처리
- read 데이터가 0일 경우 클라이언트와의 연결이 끊긴 것으로 판단
- 서버에서 한 번에 수신할 수 있는 데이터보다 요청 데이터가 많을 경우, std::vector<char>을 이용하여 나누어 받을 수 있도록 설정
- Write 이벤트 발생 시, Location의 허용 Method 파악 후 응답 생성
- Cgi 처리가 필요할 경우 fork() 함수를 이용하여 멀티 프로세스 환경에서 동적 응답 생성
