#ifndef __SERVERMANAGE_HPP__
#define __SERVERMANAGE_HPP__

#include "ConfigParse.hpp"
#include "Server.hpp"
#include <fstream>

#define MAX_EVENT_NUM 1024 // 임의로 설정한 값

class ServerManage
{
	private:
		std::string _file; // .conf 파일을 받아들임
		std::vector<Server> _servers; // 서버 클래스를 저장하는 Vector
		int _kqfd; // Kqueue를 관리
		struct kevent _events[MAX_EVENT_NUM];
	public:
		void serverRun(const char *file); // server객체를 받아 실제로 실행은 여기서 진행됨
		void makeKqueue(); // Kqueue 생성
};

#endif