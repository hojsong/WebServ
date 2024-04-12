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

## 과제 구조
<img width="1235" alt="스크린샷 2024-02-19 오후 3 13 42" src="https://github.com/dnjsals45/Webserv/assets/44596433/56fa2b13-840c-4ccb-821e-a0b76c4d8232">

## 과제 상세 구조
> <img width="1246" alt="스크린샷 2024-02-19 오후 3 23 26" src="https://github.com/dnjsals45/Webserv/assets/44596433/76e3adc5-1bdd-4e73-9164-5661aa6f0d78">

> config 파일 예시

> <img width="672" alt="스크린샷 2024-02-19 오후 3 24 15" src="https://github.com/dnjsals45/Webserv/assets/44596433/089e7203-a27f-4404-86b4-3155be6904ad">

> 포트, 서버 이름, index 페이지 설정
> Location 블록 별로 허용 메소드 및 autoindex 기능 설정

### Main 로직
<img width="471" alt="스크린샷 2024-02-19 오후 3 25 43" src="https://github.com/dnjsals45/Webserv/assets/44596433/38a53aed-7c59-44fa-874c-4423d890c66f">

- 인자로 파일 위치를 받을 경우, 그 파일을 설정 파일로 사용
- 인자가 존재하지 않을 시 deault 설정 파일 세팅

### 서버 설정
<img width="445" alt="스크린샷 2024-02-19 오후 3 27 57" src="https://github.com/dnjsals45/Webserv/assets/44596433/0a6a0ccf-7fa2-4617-a63c-079b5a0b35f2">

- setter 함수를 이용하여 객체 내부 설정

### 서버 실행
<img width="896" alt="스크린샷 2024-02-19 오후 3 28 36" src="https://github.com/dnjsals45/Webserv/assets/44596433/3d960cb6-1914-4995-a7e6-e5faaa3d83df">

- non-blocking 처리를 위해 fcntl 함수에서 O_NONBLOCK 옵션 사용

### 메인 로직
- Kqueue를 이용하여 이벤트 수신 처리
- 서버 소켓으로 첫 요청이 올 경우, accept 이후 client 소켓을 std::map으로 관리
- Read 이벤트 발생 시, Request 요청 처리
- read 데이터가 0일 경우 클라이언트와의 연결이 끊긴 것으로 판단
- 서버에서 한 번에 수신할 수 있는 데이터보다 요청 데이터가 많을 경우, std::vector<char>을 이용하여 나누어 받을 수 있도록 설정
- Write 이벤트 발생 시, Location의 허용 Method 파악 후 응답 생성
- Cgi 처리가 필요할 경우 fork() 함수를 이용하여 멀티 프로세스 환경에서 동적 응답 생성

## 결과 화면
- AutoIndex 설정하지 않았을 경우
<img width="557" alt="스크린샷 2024-02-19 오후 3 38 00" src="https://github.com/dnjsals45/Webserv/assets/44596433/24692ada-1fc3-4c09-b489-107206ac80f3">

- AutoIndex 설정한 경우
<img width="501" alt="스크린샷 2024-02-19 오후 3 38 24" src="https://github.com/dnjsals45/Webserv/assets/44596433/0b4003bc-4d5f-4015-b35c-29c0704499b0">

- 이미지 파일 업로드(python cgi 프로그램)
<img width="460" alt="스크린샷 2024-02-19 오후 4 26 16" src="https://github.com/dnjsals45/Webserv/assets/44596433/4c283ba7-8df3-474f-bb8b-ebc7b065ccce">

- 이미지 출력 결과
<img width="376" alt="스크린샷 2024-02-19 오후 4 26 29" src="https://github.com/dnjsals45/Webserv/assets/44596433/65ae67d9-31db-4f80-8617-2e63db9eab89">

- 멤버 기능 페이지 기본 화면
<img width="734" alt="스크린샷 2024-02-19 오후 4 27 14" src="https://github.com/dnjsals45/Webserv/assets/44596433/f86bfba0-a9c2-4e47-acad-19a1e12b46ad">

- 로그인 성공 시 기본 화면
<img width="787" alt="스크린샷 2024-02-19 오후 4 27 47" src="https://github.com/dnjsals45/Webserv/assets/44596433/9e7a6562-4b04-489a-8717-d7c8f72eaf49">

