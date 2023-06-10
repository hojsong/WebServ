#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>

class IRCServer {
private:
    int serv_sock;
    std::vector<int> client_socks;

public:
    void start(int port) {
        serv_sock = socket(PF_INET, SOCK_STREAM, 0);
        if (serv_sock == -1) {
            std::cerr << "Failed to create socket" << std::endl;
            return;
        }

        struct sockaddr_in serv_addr;
        memset(&serv_addr, 0, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        serv_addr.sin_port = htons(port);
		int option = 1;
 	  	 if (setsockopt(this->serv_sock, SOL_SOCKET, SO_REUSEADDR, (const void *)&option, sizeof(option)) == -1) {
  		      std::cerr << "setsockopt error" << std::endl;
 		       exit(1);
	    }
        if (bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1) {
            std::cerr << "Failed to bind" << std::endl;
            return;
        }

        if (listen(serv_sock, 5) == -1) {
            std::cerr << "Failed to listen" << std::endl;
            return;
        }

        std::cout << "IRC server started on port " << port << std::endl;

        // 서버 소켓도 논블로킹으로 설정
        setNonBlocking(serv_sock);

        while (true) {
            if (hasIncomingConnection()) {
                int client_sock = acceptClient();
                if (client_sock != -1) {
                    // 클라이언트 소켓도 논블로킹으로 설정
                    setNonBlocking(client_sock);
                    client_socks.push_back(client_sock);
                    std::cout << "New client connected" << std::endl;
                }
            }

            handleClients();

            // 처리할 이벤트가 없을 때 일시적으로 대기 (CPU 자원을 절약하기 위해)
            usleep(1000);
        }
    }

    void setNonBlocking(int sock) {
        int flags = fcntl(sock, F_GETFL, 0);
        if (flags == -1) {
            std::cerr << "Failed to get socket flags" << std::endl;
            return;
        }
        if (fcntl(sock, F_SETFL, flags | O_NONBLOCK) == -1) {
            std::cerr << "Failed to set socket as non-blocking" << std::endl;
            return;
        }
    }

    bool hasIncomingConnection() {
        fd_set rfds;
        FD_ZERO(&rfds);
        FD_SET(serv_sock, &rfds);
        struct timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 0;
        int retval = select(serv_sock + 1, &rfds, nullptr, nullptr, &tv);
        if (retval == -1) {
            std::cerr << "Failed to check for incoming connections" << std::endl;
            return false;
        }
        return FD_ISSET(serv_sock, &rfds);
    }

    int acceptClient() {
        struct sockaddr_in client_addr;
        socklen_t client_addr_size = sizeof(client_addr);
        int client_sock = accept(serv_sock, (struct sockaddr*) &client_addr, &client_addr_size);
        if (client_sock == -1) {
            std::cerr << "Failed to accept client" << std::endl;
        }
        return client_sock;
    }

    void handleClients() {
        char buffer[1024];
		if (client_socks.size() == 0)
			return ;
        for (int client_sock : client_socks) {
            while (true) {
                memset(buffer, 0, sizeof(buffer));
                int recv_len = recv(client_sock, buffer, sizeof(buffer), 0);
                if (recv_len <= 0) {
					if (recv_len == 0){
						for (size_t i = 0; i<client_socks.size(); i++){
							if (client_sock == client_socks[i])
								client_socks.erase(client_socks.begin() + i);
						}
						close(client_sock);
						std::cout << "DisConnected : " << recv_len << std::endl;
					}
                    // 클라이언트 연결이 종료되었거나 오류가 발생한 경우
                    if (recv_len == -1 && errno != EWOULDBLOCK && errno != EAGAIN) {
                        std::cerr << "Failed to receive data from client" << std::endl;
                    }
                    break;
                }

                std::string message(buffer, recv_len);
                std::cout << "Received message: " << message << std::endl;

                // 받은 메시지 처리 로직을 구현하세요.

                // 예시: 받은 메시지를 모든 클라이언트에게 전달하는 코드
        		broadcastMessage(message, client_sock);
            }
        }
    }

    void broadcastMessage(const std::string& message, int fd) {
        for (int client_sock : client_socks) {
			if (fd != client_sock)
            	send(client_sock, message.c_str(), message.size(), 0);
        }
    }
};

int main() {
    IRCServer server;
    server.start(8080);

    return 0;
}