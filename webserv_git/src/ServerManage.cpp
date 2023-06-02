#include "../includes/ServerManage.hpp"

void ServerManage::serverRun(const char *file)
{
	ConfigParse parse;

	this->_file = file; // argv[1]이 없을 경우 file = "default"
	parse.getData(this->_file);
	this->_servers = parse.getServers();
	
	if (this->_servers.empty())
		return ;
	for (size_t i = 0; i < this->_servers.size(); ++i) {
		this->_servers[i].startServer();
	}
	makeKqueue();

	struct kevent changeList[MAX_EVENT_NUM];
	int changeListCount = 8;

	while (1) {
		int nev = kevent(this->_kqfd, this->_events, this->_servers.size(), changeList, changeListCount, NULL);
		if (nev == -1) {
			std::cerr << "kevent error." << std::endl;
			break;
		}
		for (int i = 0; i < nev; ++i) {
			int eventFd = changeList[i].ident;
			for (size_t j = 0; j < this->_servers.size(); ++j) {
				if (this->_servers[j].getServerSocket() == eventFd) {
					if (this->_events[i].filter == EVFILT_READ) {
						int fd = this->_servers[j].acceptClient();
						if (fd != -1)
							this->_servers[j].handleClient();
					}
				}
			}
		}
		if (changeListCount > MAX_EVENT_NUM) {
			changeListCount = 0;
		}
	}
}

void
ServerManage::makeKqueue() {
	this->_kqfd = kqueue();
	if (this->_kqfd == -1) {
		std::cerr << "kqueue error" << std::endl;
		exit (1);
	}
	for (size_t i = 0; i < this->_servers.size(); ++i) {
		EV_SET(&this->_events[i], this->_servers[i].getServerSocket(), EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
	}
}