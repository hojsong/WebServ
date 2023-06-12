#include "../includes/Serv.hpp"

//argv = argv[1] password = argv[2]
Serv::Serv(char *argv, char *password){
	this->password = password;
	this->serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if (this->serv_sock == -1){
		return ;
	}
	memset (&this->serv_addr, 0, sizeof(this->serv_addr));
	this->serv_addr.sin_family = AF_INET;
	this->serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	this->serv_addr.sin_port=htons(atoi(argv));
	std::cout << "port : " << atoi(argv) << std::endl;
	int option = 1;
	if (setsockopt(this->serv_sock, SOL_SOCKET, SO_REUSEADDR, (const void *)&option, sizeof(option)) == -1) {
		std::cerr << "setsockopt error" << std::endl;
		exit(1);
	}
	if (bind(this->serv_sock, (struct sockaddr*) &this->serv_addr, sizeof(this->serv_addr)) == -1){
		exit(1);
	}
	if (listen(this->serv_sock, 5) == -1) {
		exit(1);
	}
	int flag = fcntl(this->serv_sock, F_SETFL, 0);
	flag |= O_NONBLOCK;
	fcntl(this->serv_sock, F_SETFL, flag);
}


void Serv::exe_Serv() {
	std::cout << "ServStart"<<std::endl;
	std::cout << "pass : " << this->password << std::endl;
		
	while (1) {
		if (hasIncomingConnection()) {
			acceptClient();
		}
		handleClientEvent();
	}
}

bool Serv::hasIncomingConnection() {
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

void Serv::acceptClient() {
	int				 	clnt_sock;
	struct sockaddr_in 	clnt_addr;
   	socklen_t clnt_addr_size = sizeof(clnt_addr);
   	clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_size);
	std::cout << "Accept start fd: " << clnt_sock << std::endl;
   	if (clnt_sock == -1) {
		std::cerr << "Accept failed" << std::endl;
   	}
	Member add(clnt_sock);
	this->clnts.push_back(add);
	int flag = fcntl(clnt_sock, F_SETFL, 0);
	flag |= O_NONBLOCK;
	fcntl(clnt_sock, F_SETFL, flag);
}

int Serv::FindByFd(int fd){
	size_t i;
	for (i = 0; i < clnts.size(); i++){
		if (fd == clnts[i].getFD())
			return static_cast<int>(i);
	}
	return (-1);
}

bool memberCheck(Member me){
	if (me.getPassword().length() == 0)
		return false;
	if (me.getRoom().length() == 0)
		return false;
	if (me.getName().length() == 0)
		return false;
	return true;
}

std::string getvalue(std::string str){
	size_t i;

	for(i=5; i<str.size(); i++){
		if (str[i] != ' ')
			break;
	}
	std::string result = str.substr(i);
	if (result[result.size() - 1] == '\n'){
		result.replace(result.size() - 1, 1, "");
	}
	return result;
}

void Serv::sendMassage(Member sender, std::string message){
	size_t i;

	for (i = 0; i < clnts.size(); i++){
		if (clnts[i].getFD() != sender.getFD()){
			if (clnts[i].getRoom() == sender.getRoom()){
				send(clnts[i].getFD(), message.c_str(), message.size() + 1, 0);
			}
		}
	}
}

// void	Serv::create_or_joinRoom(std::string data, Member member){

// }

void Serv::handleClientEvent(){
	char buf[4096];
	std::string receiveData;
	for (size_t i = 0; i < clnts.size(); i++){
		while (true){
			memset(buf, 0, 4096);
			buf[0] = 'a';
			int bys = recv(clnts[i].getFD(), buf, 4096, 0);
			receiveData = buf;
			if (bys == -1)
				break;
			if (bys == 0){
				int fd = clnts[i].getFD();
				std::cout << fd << " is Close." << std::endl;
				clnts.erase(clnts.begin() + i);
				close(fd);
			}
			buf[bys] = '\0';
			std::cout << "ReceiveData Client" << clnts[i].getFD() << " : " << buf << std::endl;
			if (memberCheck(clnts[i]) == false && receiveData.length() > 4){
				std::string data = getvalue(receiveData);
				std::cout << data << std::endl;
				if (receiveData.find("PASS ") == 0){
					clnts[i].setPass(data);
				}
				else if (receiveData.find("NAME ") == 0 && clnts[i].getPassword() == this->password)
					clnts[i].setName(data);
				else if (receiveData.find("JOIN ") == 0 && clnts[i].getPassword() == this->password && clnts[i].getName().length() != 0){
					// create_or_joinRoom(data, clnts[i].getFD);
					clnts[i].setRoom(data);
				}
				if (clnts[i].getPassword() != this->password){
					std::cout << "clnt pass : " << clnts[i].getPassword() << std::endl;
					std::cout << "serv pass : " << this->password << std::endl;
					std::cout << static_cast<int>(clnts[i].getPassword()[clnts[i].getPassword().length() - 1]) << std::endl;
					std::cout << static_cast<int>(this->password[this->password.length() - 1]) << std::endl;
					std::string message = "Passwords don't match\n";
					send(clnts[i].getFD(), message.c_str(), message.size() + 1, 0);
					clnts[i].setPass("");
				}
			}
			else if (memberCheck(clnts[i])){
				if (receiveData.find("EXIT") == 0 && receiveData.length() == 4){
					std::string roomExit = clnts[i].getName() + " is " + clnts[i].getRoom() + " EXIT.";
					sendMassage(clnts[i], roomExit);
					clnts[i].setRoom("");
				}
				else {
					sendMassage(clnts[i], buf);
				}
			}
		}
	}
}
