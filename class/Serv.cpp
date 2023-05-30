#include "../includes/Serv.hpp"
#include "../includes/My_socket.hpp"


Serv::Serv() {
    std::ifstream file("default.conf");
    std::vector<std::string> conf;
    std::string line;
    std::string save;

    while (std::getline(file, line)) {
        if (line.find("server {") != std::string::npos) {
            if (save.length() != 0) {
                conf.push_back(save);
                save = "";
            }
        }
        save += line + "\n";
    }
    conf.push_back(save);

    for (size_t i = 0; i < conf.size(); i++) {
        const char* listen_pos = strstr(conf[i].c_str(), "listen ");
        if (listen_pos) {
            const char* port_start = listen_pos + std::strlen("listen ");
            const char* port_end = strstr(port_start, ";");
            std::string port_str(port_start, port_end - port_start);
            My_socket socket(port_str.c_str(), &this->memberRepository);  // 필요한 인수를 전달하여 객체 생성

            const char* location_pos = strstr(conf[i].c_str(), "location / {");
            if (location_pos) {
                const char* location_start = location_pos + std::strlen("location / {");
                const char* location_end = strstr(location_start, "}");
                std::string location(location_start, location_end - location_start);
                socket.dir_indexing(location.c_str());
            }

            this->servs.push_back(socket);
        }
    }
}

void Serv::exe_Serv() {
    int max_fd = -1;
    int num_events = 0;
	this->kq = kqueue();
    struct kevent events[MAX_EVENTS];
	size_t it;
    for (it = 0; it < servs.size(); ++it) {
		EV_SET(&events[it], servs[it].getServSock(), EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
        int fd = servs[it].getServSock();
        if (fd > max_fd)
            max_fd = fd;
    }
	
	struct kevent changeList[MAX_EVENTS];
    int changeCount = 8;
	
    while (1) {
        num_events = kevent(this->kq, events, servs.size(), changeList, changeCount, 0);
        if (num_events == -1) {
			std::cout << "Error Break" << std::endl;
			break ;
        } 
        for (int i = 0; i < num_events; ++i) {
            int fd = changeList[i].ident;
            for (it = 0; it < servs.size(); ++it) {
                int serv_fd = servs[it].getServSock();
                if (fd == serv_fd) {
                    int at = servs[it].acceptClient();
                    if (at != -1)
                        servs[it].handleClientEvent();
                }
            }
        }

        if (changeCount > MAX_EVENTS) {
            changeCount = 0;
        }
    }
}