#include "../includes/My_socket.hpp"

My_socket::My_socket(const char *argv, MemberRepository *mr){
	this->clnt_sock = -1;
	this->memberRepository = mr;
	this->serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if (this->serv_sock == -1){
		this->result = 1;
		return ;
	}
	
	memset (&this->serv_addr, 0, sizeof(this->serv_addr));
	this->serv_addr.sin_family = AF_INET;
	this->serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	this->serv_addr.sin_port=htons(atoi(argv));
	int option = 1;
    if (setsockopt(this->serv_sock, SOL_SOCKET, SO_REUSEADDR, (const void *)&option, sizeof(option)) == -1) {
        std::cerr << "setsockopt error" << std::endl;
        exit(1);
    }
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

void My_socket::dir_indexing(const char *str) {
	std::string strCopy = str;
	std::istringstream iss(strCopy);
	std::string line;
	while (std::getline(iss, line)) {
    	std::istringstream lineStream(line);
    	std::string key, value;
    	lineStream >> key >> value;
		if (key.length() == 0 || value.length() == 0)
			;
		else {
    		if (key == "root")
	        	this->dir = value;
	    	else
	    	    this->file[key] = dir + value;
			std::cout << "key: " << key << ", value: " << value << std::endl;
		}
	}
}

int	My_socket::getServSock(){
	return this->serv_sock;
}

My_socket::~My_socket(){
	// close(this->serv_sock);
}

int My_socket::acceptClient() {
	if (this->clnt_sock == -1){
    	socklen_t clnt_addr_size = sizeof(this->clnt_addr);
    	this->clnt_sock = accept(this->serv_sock, (struct sockaddr *)&this->clnt_addr, &clnt_addr_size);
    	if (this->clnt_sock == -1) {
			std::cerr << "Accept failed" << std::endl;
    	}
    	int flag = fcntl(this->clnt_sock, F_SETFL, 0);
    	flag |= O_NONBLOCK;
    	fcntl(this->clnt_sock, F_SETFL, flag);
	}
	return this->clnt_sock;
}

void My_socket::handleClientEvent(){
	char buf[4096];
	char *temp;
	std::string receiveData;
    int bys = recv(this->clnt_sock, buf, 4096, 0);
	if (bys == -1){
	    return ;
	}
    else if (bys == 0){
		close(this->clnt_sock);
		this->clnt_sock = -1;
        return ;
	}
	else {
		buf[bys] = '\0';
		receiveData += buf;
		if (receiveData.find("\r\n\r\n")){
			temp = const_cast<char *>(receiveData.c_str());
			std::string response = getPath(temp);
			if (response.length() != 0){
				int send_result = send(this->clnt_sock, response.c_str(), response.length() + 1, 0);
				if (send_result == -1) {
					return ;
				} else {
				}
				receiveData.clear();
			}
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

std::string My_socket::getPath(char *buf) {
	int	rehead;
    std::string content = getFile(buf, rehead, this->memberRepository);
	if (content.length() == 0 || rehead <= 0)
		return "";
	head_plus(content, rehead);
    return content;
}

int My_socket::getResult(void) const{
	return (this->result);
}
