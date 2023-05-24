#include "../includes/My_socket.hpp"

void error_handling(char *message);

int main(int argc, char* argv[]){
	int serv_sock;
	int clnt_sock;

	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
	socklen_t clnt_addr_size;

	
		serv_sock = socket(PF_INET, SOCK_STREAM, 0);
		if (serv_sock == -1)
			error_handling("socket error");
		memset (&serv_addr, 0, sizeof(serv_addr));
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
		serv_addr.sin_port=htons(atoi(argv[1]));

		if (bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1)
			error_handling("bind error");
	while (1){

		if (listen(serv_sock, 5) == -1)
			error_handling("listen error");

		clnt_addr_size = sizeof(clnt_addr);
		clnt_sock = accept(serv_sock, (struct sockaddr*) &clnt_addr, &clnt_addr_size);
		if(clnt_sock == -1)
			error_handling("accept error");
		char buf[4096];
		int bys = recv(clnt_sock, buf, sizeof(buf), 0);
		if (bys == -1)
		{
    		std::cerr << "수신 오류" << std::endl;
    		// 오류 처리
		}
		else if (bys == 0)
		{
    		std::cout << "클라이언트 연결 종료" << std::endl;
    		// 클라이언트 연결 종료 처리
		}
		else
		{
			std::cout << buf << std::endl;
		}
		std::cout << "---------------------------------------------------------------------" << std::endl;
		char *result;
		std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body><h1>Hello World</h1></body></html>";
		send(clnt_sock, response.c_str(), response.length() + 1, 0);
		close (clnt_sock);
	}
	close (clnt_sock);
	close (serv_sock);
	return (0);
}

void error_handling(char *str){
	size_t i;

	i = 0;
	if (str != NULL){
		while (str[i])
			i++;
		write(2, str, i);
		write(2, "\n", 1);
	}
	exit(1);
}