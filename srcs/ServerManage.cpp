#include "library.hpp"

ServerManage::ServerManage() {}

ServerManage::~ServerManage() {}

std::string makeAutoindex(Location loc, Server server) {
    std::string rootpath = "./" + server.getRoot() + loc.getPath() + "/";
    DIR*    dir;
    struct dirent*  dp;
    
    dir = opendir(rootpath.c_str());
    if (dir == NULL) {
        throw ErrorException("invalid folder path");
    }

    std::ostringstream  os;

    os << "<html>";
    os << "<h1>Index of " << loc.getPath() <<"</h1>";
    os << "<table>";
    os << "<thead>";
    os << "<tr><th>Name</th><th>Description</th></tr>";
    os << "</thead>";
    os << "<tbody>";
    while ((dp = readdir(dir)) != NULL) {
        std::string dir_name(dp->d_name);
        os << "<tr>";
        os << "<td><a href=" << rootpath + dir_name << ">" << dir_name << "</td>";
        os << "<td>-</td>";
        os << "</tr>";
    }
    closedir(dir);
    os << "</tbody>";
    os << "</table>";
    return os.str();
}

std::string readFilePath(std::string file_path, std::map<int, std::string> errors) {
    std::ifstream   file(file_path);
    if (file.is_open()) {
        std::ostringstream os;
        std::string line;
        while (std::getline(file, line))
            os << line;
        file.close();
        return os.str();
    } else {
        file.close();
        return errors.find(500)->second;
    }
}

std::string makeBodyPath(Location loc, Server server) {
    std::string file_path;
    std::string root;
    std::string buf;
    std::string path = loc.getPath();

    if (loc.getRoot() == "")
        buf = server.getRoot();
    else
        buf = loc.getRoot();

    root = "./" + buf;

    if (path == "/")
        file_path = root + "/" + server.getIndex();
    else {
        std::string index = loc.getIndex();
        file_path = server.getRoot() + loc.getPath() + "/" + loc.getIndex();
    }
    return file_path;
}

bool    isDirect(Server server, Location loc, std::string request_path) {
    DIR*    dir;
    struct dirent* dp;

    std::string buffer = server.getRoot() + loc.getPath();
    dir = opendir(buffer.c_str());
    if (dir == NULL)
        return false;
    dp = readdir(dir);
    while (dp != NULL) {
        std::string dir_name(dp->d_name);
        std::string rootpath = "./" + server.getRoot() + loc.getPath() + "/";
        if (request_path == rootpath + dir_name) {
            closedir(dir);
            return true;
        }
        dp = readdir(dir);
    }
    closedir(dir);
    return false;
}

std::string makeBody(Server server, std::string request_path, Location loc, Response res) {
    std::map<int, std::string> errors = server.getErrorPages();
    if (res.getStatusCode() != 200)
        return errors.find(res.getStatusCode())->second;

    std::string body;
    if (isDirect(server, loc, "." + request_path) == true) {
        std::string file_path = "." + request_path;
        body = readFilePath(file_path, errors);
        return body;
    } else if (loc.getAutoindex() == true) {
        std::string auto_index = makeAutoindex(loc, server);
        return auto_index;
    } else {
        std::string file_path = makeBodyPath(loc, server);
        body = readFilePath(file_path, errors);
        return body;
    }
    return "";
}

std::string buildResponse(const std::string& body, Location loc, Server server, int error_flag) {
    std::map<int, std::string>  header_list = server.getErrorPagesHeader();
    std::string header_message = header_list.find(error_flag)->second;
    std::ostringstream os;
    os << header_message << "\r\n";
    os << "Content-Type: text/html; charset=utf-8\r\n";
    if (error_flag == 405) {
        os << "Allow: GET,";
        std::vector<int>    methods = loc.getMethods();
        bool    is_first = true;
        if (methods[1] == 1) {
            os << "POST";
            is_first = false;
        } if (methods[2] == 1) {
            if (is_first == false)
                os << ",";
            os << "DELETE";
        } if (methods[3] == 1) {
            if (is_first == false)
                os << ",";
            os << "PUT";
        } if (methods[4] == 1) {
            if (is_first == false)
                os << ",";
            os << "HEAD";
        }
        os.str().pop_back();
        os << "\r\n";
    }
    os << "Content-Length: " << body.size() << "\r\n";
    os << "\r\n";
    if (body != "")
        os << body;
    return os.str();
}

ssize_t readData(int fd, char *buffer, size_t buffer_size) {
    ssize_t len = recv(fd, buffer, buffer_size, 0);
    if (len == 0) {
        //std::cout << "client disconnected" << std::endl;
        return (0);
    } else if (len > 0) {
        return len;
    }
    return -1;
}

void    processRequest(Request& req) {
    std::vector<std::string> buf = splitArgs(req.getHeaders(), "\r\n");
    std::vector<std::string> first_line = split(buf[0].c_str(), ' ');
    std::map<std::string, std::string> tmpData;

    if (first_line.size() != 3) {
        // 에러 처리
    }
    if (first_line[0] == "GET" || first_line[0] == "POST" || first_line[0] == "DELETE")
        req.setMethod(first_line[0]);
    req.setPath(first_line[1]);
    req.setType(first_line[2]);

    for (size_t i = 1; i < buf.size(); ++i) {
        size_t pos = buf[i].find(":");
        tmpData.insert(std::pair<std::string, std::string>(buf[i].substr(0, pos), buf[i].substr(pos + 2)));
    }
    req.setHeaderData(tmpData);
}

// Request processRequest(const char *buffer, ssize_t len) {
//     std::istringstream is(std::string(buffer, len));
//     std::string line;
//     std::ostringstream os;

//     // std::cout << "----------------------------------------" << std::endl;
//     while (std::getline(is, line)) {
//         // std::cout << line << std::endl;
//         os << line;
//     }

//     std::vector<std::string> buf = splitArgs(os.str(), " ");
//     Request req(buf);
//     return req;
// }

size_t  sendResponse(int client_fd, Server &server, std::string req_path, Response res) {
    std::vector<Location>  locs = server.getLocations();
    std::string body;
    std::string response;
    bool    is_404 = true;

    if (res.getCgiStr().length() != 0){
        std::cout << "1123" << std::endl;
        write(client_fd, res.getCgiStr().c_str(), res.getCgiStr().length());
        return WRITE_FINISH;
    }
    else {
        if (res.getStatusCode() == 204) {
            response = "HTTP/1.1 204 No Content\r\n\r\n";
            write(client_fd, response.c_str(), response.size());
            return WRITE_FINISH;
        }
        else if (res.getStatusCode() == 403) {
            response = "HTTP/1.1 403 Forbidden\r\nContent-Length: 19\r\nContent-Type: text/plain; charset=utf-8\r\n\r\nAccess denied: 403\r\n";
            write(client_fd, response.c_str(), response.size());
            return WRITE_FINISH;
        }
        else if (res.getStatusCode() > 400) { // 원래는 200
            body = makeBody(server, req_path, locs[0], res);
            response = buildResponse(body, locs[0], server, res.getStatusCode());
            size_t len = write(client_fd, response.c_str(), response.size());
            if (len == response.size()) {
                return WRITE_FINISH;
            }
        }
        size_t i;
        for (i = 0; i < locs.size(); i++) {
            if (locs[i].getPath() == req_path) {
                Location    target = locs[i];
                if (target.getReturnValue().size() == 2) {
                    std::string return_path = target.getReturnValue();

                    // int head_number = atoi(buf[0].c_str());
                    // std::string return_path = buf[1];
                    // response = buildResponse(body, target, server, head_number);
                    // std::cout << response << std::endl;
                    // std::cout << return_path << std::endl;
                    // 303 처리에 대한 문제점 : 한 fd당 한번의 send만 가능하나
                    // 303 send 후 다시 리다이렉션시 소켓이 끊김
                    // write(client_fd, response.c_str(), response.size());

                    sendResponse(client_fd, server, return_path, res);
                    return WRITE_READY;
                }
                body = makeBody(server, req_path, target, res);
                response = buildResponse(body, target, server, res.getStatusCode());
                is_404 = false;
                break;
            }  else if (locs[i].getAutoindex() == true && isDirect(server, locs[i], "." + req_path) == true) {
                body = makeBody(server, req_path, locs[i], res);
                response = buildResponse(body, locs[i], server, res.getStatusCode());
                is_404 = false;
                break;
            }
        }
        if (is_404 == true) {
            res.setStatusCode(404);
            body = makeBody(server, req_path, locs[0], res);
            response = buildResponse(body, locs[0], server, res.getStatusCode());
        }
        size_t len = write(client_fd, response.c_str(), response.size());
        if (len == response.size()) {
            return WRITE_FINISH;
        }
    }
    return WRITE_READY;
}

 void    executeMethodDelete(std::string req_path, Response& res) {
    std::ifstream ifs(req_path.c_str());

    std::cout << "path: " << req_path << std::endl;

    if (!ifs.good()) {
        res.setStatusCode(404);
        return ;
    }
    ifs.close();
    if (unlink(req_path.c_str()) != 0) {
        res.setStatusCode(403);
    }
    else
        res.setStatusCode(204);

 }

std::string handle_cgi(std::string cgiPath, Request req) {
    int cgiInput[2], cgiOutput[2];
	pid_t pid;
	int status;

	if (pipe(cgiInput) < 0 || pipe(cgiOutput) < 0) {
		perror("pipe error");
		return "";
	}

	if ((pid = fork()) < 0) {
		perror("fork error");
		return "";
	}

    std::string image = req.getBody();

    size_t pos = image.find("\r\n\r\n");
    std::string image_data = image.substr(pos + 4);
    size_t pos2 = image_data.find("------");
    std::string real_data = image_data.substr(0, pos2);
    //std::cout << "real data: " << real_data << std::endl;
	if (pid == 0) {
		dup2(cgiInput[0], 0);
		dup2(cgiOutput[1], 1);
		close(cgiInput[1]);
		close(cgiOutput[0]);


		std::string contentLength = "CONTENT_LENGTH=" + std::to_string(req.getBody().size());
        std::string pythonPath = "/Users/seongmpa/.brew/lib/python3.11/site-packages";  // PIL 모듈이 설치된 디렉토리 경로로 변경해야 합니다.
        std::string pythonPathEnv = "PYTHONPATH=" + pythonPath;
        char* pythonPathEnvPtr = new char[pythonPathEnv.size() + 1];
        std::strcpy(pythonPathEnvPtr, pythonPathEnv.c_str());
        pythonPathEnvPtr[pythonPathEnv.size()] = '\0';

		char* cl_env = new char[contentLength.size() + 1];
		strncpy(cl_env, contentLength.c_str(), contentLength.size());
		cl_env[contentLength.size()] = '\0';

        //char* envp[] = {cl_env, NULL};
        char* envp[] = {cl_env, pythonPathEnvPtr, NULL};

		char* path = new char[cgiPath.size() + 1];
		strncpy(path, cgiPath.c_str(), cgiPath.size());
		path[cgiPath.size()] = '\0';


		char* const argv[] = {path, NULL};
		if (execve(path, argv, envp) == -1) {

            delete[] pythonPathEnvPtr;
			delete[] cl_env;
			delete[] path;
		}
		exit(0);
	}
	else {
		close(cgiInput[0]);
		close(cgiOutput[1]);

		write(cgiInput[1], real_data.c_str(), real_data.size());
		close(cgiInput[1]);

		std::string cgi_output;
		char buf[1024];
		ssize_t bytesRead;
		while ((bytesRead = read(cgiOutput[0], buf, sizeof(buf))) > 0) {
			cgi_output.append(buf, bytesRead);
		}

		close(cgiOutput[0]);
		waitpid(pid, &status, 0);

		std::string http_response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: " + std::to_string(cgi_output.length()) + "\r\n\r\n" + cgi_output;
        return http_response;
	}
}

void    ServerManage::setupServer(ParseConfig param) {
    this->servers = param.getServers();
}

void
ServerManage::change_events(uintptr_t const & ident, int16_t const & filter, uint16_t const & flags)
{
	struct kevent temp_event;

	EV_SET(&temp_event, ident, filter, flags, 0, 0, NULL);
	changeList.push_back(temp_event);
}

size_t
ServerManage::checkContenLength(std::string headers) {
    size_t len = 0;
    std::string length = "";

    std::vector<std::string> tmp = splitArgs(headers, "\r\n");
    for (size_t i = 0; i < tmp.size(); i++) {
        if (tmp[i].find("Content-Length: ") != std::string::npos) {
            length = tmp[i].substr(tmp[i].find(':') + 2);
        }
    }
    if (length != "")
        len = std::stoi(length);
    return len;
}

bool ServerManage::checkServerIndex(struct kevent *curr_event) {

    for (size_t i = 0; i < servers.size(); ++i) {
        if (curr_event->ident == servers[i].getServerSocket()) {
            return true;
        }
    }
    return false;
}

void    urlSearch(Request &req, std::vector<Location> locations){
    std::string str = "";
    std::string file = "";
    size_t      slen = 0;
    size_t      i;
    size_t      pos;

    for (i = 0; i < locations.size(); i++) {
        pos = req.getPath().find(locations[i].getPath());
        if (pos != std::string::npos && locations[i].getPath().size() > slen){
            str = locations[i].getPath();
            slen = locations[i].getPath().size();
            file = req.getPath().substr(pos + locations[i].getPath().size());
        }
    }
    std::cout << str << std::endl;
    std::cout << file << std::endl;

    req.setPath(str);
    req.setBody(file);
}

void    ServerManage::runServer(void) {
    int server_size = this->servers.size();
    
    struct kevent eventList[1024];
    struct kevent *curr_event;

    //socket open
    std::vector<Server>::iterator it;
    for (it = this->servers.begin(); it != this->servers.end(); ++it) {
        it->run();
    }
    //kqueue open
    int kq = kqueue();
	if (kq == -1) {
		throw ErrorException("kqueue error");
	}
    for (int i = 0; i < server_size; ++i) {
        change_events(this->servers[i].getServerSocket(), EVFILT_READ, EV_ADD | EV_ENABLE);
    }
    std::cout << "Server Start!" << std::endl;
    //main loop
    while(true) {
        int event_count = kevent(kq, &changeList[0], changeList.size(), eventList, 1024, 0);

        changeList.clear();
        if (event_count == -1) {
            // fd 모두 닫기
            throw ErrorException("kevent error");
        }
        for (int i = 0; i < event_count; ++i) { // 이벤트 개수만큼 루프 순회
            curr_event = &eventList[i];
            if (curr_event->flags & EV_ERROR) { // 에러 발생 시
                //perror("kevent");
                continue;
                // 에러 처리 필요
            }
            if (checkServerIndex(curr_event)) { // 서버 소켓일 경우, 서버 인덱스 값
                size_t      index;

                for (index = 0; index < servers.size(); ++index) {
                    if (curr_event->ident == servers[index].getServerSocket()) {
                        break;
                    }
                }
                int clnt_fd = accept(servers[index].getServerSocket(), NULL, NULL);
                if (clnt_fd == -1) {
                    // 에러 처리
                    continue;
                }
                if (fcntl(clnt_fd, F_SETFL, O_NONBLOCK) == -1) {
                        // 에러 처리
                }
                // 클라이언트 소켓 이벤트 등록(READ와 WRITE 모두 등록하지만 READ부터 해야하기때문에 ENABLE, DISABLE로 구분)
                change_events(clnt_fd, EVFILT_READ, EV_ADD | EV_ENABLE);
                change_events(clnt_fd, EVFILT_WRITE, EV_ADD | EV_DISABLE);
                // 클라이언트 : 서버소켓 으로 다수 클라이언트 관리
                clients[clnt_fd] = servers[index].getServerSocket(); // 추후 클라이언트 이벤트 발생 시, curr_event->ident로 접근 가능
                // Connection 객체 생성 -> 응답 시간 체크 및 request 정보 포함
                Request req;
                connects[clnt_fd] = req;
                // 새로운 client 생성
                std::cout << "Accept new client: " << clnt_fd << std::endl;
            }
            else { // false일 경우 클라이언트 소켓 값
                if (curr_event->filter == EVFILT_READ) {
                    std::cout << "Read Event: " << curr_event->ident << std::endl;
                // 클라이언트 소켓일 경우 Reqeust를 읽어야 하기 때문에 아래 else if 문으로 접근(else문의 curr_event->ident는 모두 클라이언트 소켓임)
                    //char buffer[BUFFER_SIZE + 1];
                    //std::memset(buffer, 0, sizeof(buffer));
                    std::vector<char> buffer(BUFFER_SIZE);
                    ssize_t len = readData(curr_event->ident, buffer.data(), BUFFER_SIZE);
                    // len > 0 : 읽을 데이터 있음, len == -1 : 아직 데이터 수신을 완료하지 못했을 수 있으므로 다시 접근, len == 0 : 클라이언트와의 접근이 끊김(close)
                    if (len > 0) {
                        buffer.resize(len);
                        if (connects[curr_event->ident].getState() == HEADER_READ) { // State 초기 설정값은 HEADER_READ임
                            std::string temp_data(buffer.begin(), buffer.end());
                            size_t pos = temp_data.find("\r\n\r\n"); // 헤더와 본문을 구문하는 구분자는 \r\n\r\n
                            if (pos == std::string::npos) { // pos가 npos라는 뜻은 본문이 존재하지 않는 요청
                                connects[curr_event->ident].appendHeader(temp_data);
                            }
                            else { // 본문을 읽는데
                                connects[curr_event->ident].appendHeader(temp_data.substr(0, pos)); // 헤더만 저장
                                size_t body_size = checkContenLength(this->connects[curr_event->ident].getHeaders()); // Content-Length 헤더의 값 추출
                                std::string temp_body = temp_data.substr(pos + 4); // 본문 내용 저장(없을 경우 "")
                                connects[curr_event->ident].appendBody(temp_body);
                                if (body_size == temp_body.size()) { // 본문을 다 읽었을 경우
                                    connects[curr_event->ident].setState(READ_FINISH);
                                }
                                else { // 본문을 다 읽지 못했을 경우
                                    connects[curr_event->ident].setState(BODY_READ);
                                }
                            }
                        }
                        else if (connects[curr_event->ident].getState() == BODY_READ) {
                            size_t body_size = checkContenLength(this->connects[curr_event->ident].getHeaders());
                            std::string temp_body(buffer.begin(), buffer.end());
                            connects[curr_event->ident].appendBody(temp_body);
                            if (body_size == connects[curr_event->ident].getBody().size()) {
                                connects[curr_event->ident].setState(READ_FINISH);
                            }
                        }
                    }
                    else if (len == 0) { // 클라이언트와의 연결 종료(읽을 데이터가 없을 경우 클라이언트에게서는 0이 아닌 -1 값을 받아옴. 연결이 끊겼을 때(close)만 0 출력됨
                        // std::cout << "Client " << curr_event->ident << " disconnected." << std::endl;
                        // std::string disc = "Set-Cookie: expires=Thu, 01-Jan-1970 00:00:01; GMT path=/";
                        // if (write(curr_event->ident, disc.c_str(), disc.length()) == -1)
                            // std::cout << "Cookie reset Fail; "<< std::endl;
                        close(curr_event->ident);
                    }
                    else {
                        continue ; // 추후 다시 접근
                    }
                    if (connects[curr_event->ident].getState() == READ_FINISH) { // 데이터를 모두 읽었을 경우 본문 응답 생성
                        uintptr_t   serv_fd;
                        Response    res;
                        std::vector<int> is_ok;
                        size_t  i;
                        size_t  index;

                        serv_fd = clients[curr_event->ident]; // 서버 객체에 접근할 서버 소켓값 추출
                        for (index = 0; index < servers.size(); ++index) {
                            if (serv_fd == servers[index].getServerSocket()) {
                                break;
                            }
                        }
                        processRequest(this->connects[curr_event->ident]);
                        if (this->connects[curr_event->ident].getMethod() == "DELETE"){
                            urlSearch(this->connects[curr_event->ident], servers[index].getLocations());
                        }
                        for (i = 0; i < servers[index].getLocations().size(); i++) {
                            if (connects[curr_event->ident].getPath() == servers[index].getLocations()[i].getPath())
                                break;
                        }
                        // std::cout << "Header: " << connects[curr_event->ident].getHeaders() << std::endl;
                        // std::cout << "Body: " << connects[curr_event->ident].getBody() << std::endl;
                        if (i == servers[index].getLocations().size()){
                           res.setStatusCode(404);
                        }
                        else {
                           is_ok = servers[index].getLocations()[i].getMethods(); // is_ok의 경우 각 Method 권한을 가지고 있음. 0일 경우 접근 불가, 1일 경우 접근 가능
                        }
                        std::cout << "isok : " << is_ok.size() << std::endl;
                        if (is_ok.size() > 0) { // 접근할 수 있는 Method가 있을 경우
                            // 각 메소드 및 권한을 파악하여 응답 생성
                            if (this->connects[curr_event->ident].getMethod() == "GET") {
                                std::cout << "==========GET========" << std::endl;
                                std::string cgi_str = cgi_differentiation(servers[index].getMemberRepository(), connects[curr_event->ident]);
                                responses[curr_event->ident].setCgiStr(cgi_str);
                                std::string body = makeBody(servers[index], connects[curr_event->ident].getPath(), servers[index].getLocations()[index], res);
                                std::string send_message = buildResponse(body, servers[index].getLocations()[index], servers[index], res.getStatusCode());
                            }
                            else if (this->connects[curr_event->ident].getMethod() == "POST") {
                                std::cout << "==========POST========" << std::endl;
                                std::string cgi_str = cgi_differentiation(servers[index].getMemberRepository(), connects[curr_event->ident]);
                                std::string str = connects[curr_event->ident].getHeaders() + "\r\n\r\n" + connects[curr_event->ident].getBody();
                                char *buf = const_cast<char *>(str.c_str());
                                if (GetComplete(buf, servers[index].getMemberRepository())){
                                   if (is_ok[3] > 0 && std::strstr(buf, "_method=delete"))
                                       delete_member_true(buf, servers[index].getMemberRepository());
                                   else 
                                       save_true(buf, servers[index].getMemberRepository());
                                }
                                responses[curr_event->ident].setCgiStr(cgi_str);
                            }
                            else if (this->connects[curr_event->ident].getMethod() == "DELETE") {
                                std::cout << "==========DELETE========" << std::endl;
                                std::string path = "./" + servers[index].getRoot() + connects[curr_event->ident].getPath() + connects[curr_event->ident].getBody();
                                executeMethodDelete(path, responses[curr_event->ident]);
                            }
                        }
                         if (this->connects[curr_event->ident].getMethod() == "") {
                            std::cout << "없는 method" << std::endl;
                             // 없는 method error 처리
                         }
                        // 응답 생성까지 끝났으므로
                        change_events(curr_event->ident, EVFILT_READ, EV_DISABLE); // 클라이언트 READ 이벤트 비활성화
                        change_events(curr_event->ident, EVFILT_WRITE, EV_ENABLE); // 클라이언트 소켓 WRITE 이벤트 활성화
                    }  
                }
                else if (curr_event->filter == EVFILT_WRITE) { // 클라이언트 소켓이고, WRITE 이벤트가 생겼을 경우
                    uintptr_t   serv_fd;
                    size_t      index;

                    serv_fd = clients[curr_event->ident];
                    for (index = 0; index < servers.size(); ++index) {
                        if (serv_fd == servers[index].getServerSocket()) {
                            break;
                        }
                    }
                    size_t res = sendResponse(curr_event->ident, servers[index], connects[curr_event->ident].getPath(), responses[curr_event->ident]);
                    if (res == WRITE_FINISH) {
                        change_events(curr_event->ident, EVFILT_READ, EV_ENABLE); // 클라이언트 READ 이벤트 활성화
                        change_events(curr_event->ident, EVFILT_WRITE, EV_DISABLE); // 클아이언트 WRITE 이벤트 비활성화
                        connects[curr_event->ident].clearAll(); // 응답을 보냈으므로 안쪽 내용 초기화
                        responses[curr_event->ident].clearAll();
                        std::cout << "response ok" << std::endl;
                    }
                }
            }
        }
        curr_event = NULL;
    }
}
