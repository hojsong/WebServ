#include "../includes/My_socket.hpp"

My_socket::My_socket(char **argv){
	this->serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if (this->serv_sock == -1){
		this->result = 1;
		return ;
	}
	memset (&this->serv_addr, 0, sizeof(this->serv_addr));
	this->serv_addr.sin_family = AF_INET;
	this->serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	this->serv_addr.sin_port=htons(atoi(argv[1]));
	if (bind(this->serv_sock, (struct sockaddr*) &this->serv_addr, sizeof(this->serv_addr)) == -1){
		this->result = 2;
		return ;
	}
	if (listen(this->serv_sock, 5) == -1) {
    	this->result = 3;
    	return;
	}
	int flag = fcntl(this->serv_sock, F_SETFL, 0);
	flag |= O_NONBLOCK;
	fcntl(this->serv_sock, F_SETFL, flag);
	this->result = 0;
}

My_socket::~My_socket(){
	// close(this->serv_sock);
}

void My_socket::execve_serv() {
	fd_set readfds;
    FD_ZERO(&readfds);
    int max_fd = 0;

    FD_SET(this->serv_sock, &readfds);
    max_fd = this->serv_sock;

    for (size_t i = 0; i < this->clnt_socks.size(); ++i) {
        int clnt_sock = this->clnt_socks[i];
        FD_SET(clnt_sock, &readfds);
        if (clnt_sock > max_fd) {
            max_fd = clnt_sock;
        }
    }

    struct timeval timeout;
    timeout.tv_sec = 5;
    timeout.tv_usec = 0;

    int ready = select(max_fd + 1, &readfds, NULL, NULL, &timeout);
    if (ready == -1) {
        this->result = 5;
        return;
    } else if (ready == 0) {
        this->result = 6;
		
    }

    if (FD_ISSET(this->serv_sock, &readfds)) {
        socklen_t clnt_addr_size = sizeof(this->clnt_addr);
        int clnt_sock = accept(this->serv_sock, (struct sockaddr*) &this->clnt_addr, &clnt_addr_size);
        if (clnt_sock == -1) {
            std::cerr << "Accept failed" << std::endl;
        }        
        this->clnt_socks.push_back(clnt_sock);
    }

    for (size_t i = 0; i < this->clnt_socks.size(); ++i) {
	    int clnt_sock = this->clnt_socks[i];
    	if (FD_ISSET(clnt_sock, &readfds)) {
        	std::string response = getPath(clnt_sock);
        	int send_result = send(clnt_sock, response.c_str(), response.length() + 1, 0);
        	if (send_result == -1) {
	            std::cerr << "Send failed" << std::endl;
        	} else {
	            std::cout << "Send successful" << std::endl;
        	}

        	close(clnt_sock);
        	this->clnt_socks.erase(this->clnt_socks.begin() + i);
		}
    }
}


int	ft_strncmp(char *str1, const char *str2, int n){
	int i = 0;

	if (str1 == NULL)
		return (1);
	while (i < n){
		if (str1[i] != str2[i])
			return (1);
		i++;
	}
	return (0);
}

std::string My_socket::getPath(int clnt_sock) {
	int	rehead;
    std::string content = getFile(clnt_sock, rehead, this->memberRepository);
	head_plus(content, rehead);
	// std::cout << content << std::endl;
    return content;
}

int My_socket::getResult(void) const{
	return (this->result);
}
