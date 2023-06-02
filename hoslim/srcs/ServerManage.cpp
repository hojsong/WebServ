#include "library.hpp"

ServerManage::ServerManage() {}

ServerManage::~ServerManage() {}

void    ServerManage::setupServer(ParseConfig param) {
    serv = param.getServers();
}

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

std::string makeBody(Server server, std::string request_path, Location loc, int& error_flag) {
    std::map<int, std::string> errors = server.getErrorPages();
    if (error_flag != 200)
        return errors.find(error_flag)->second;

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

void    acceptClient(int sockfd, struct sockaddr_in &client_addr, std::vector<int> &server_list) {
    socklen_t addr_size = sizeof(client_addr);
    int client_fd = accept(sockfd, (sockaddr*)&client_addr, &addr_size);
    if (client_fd == -1) {
        std::cout << "accept error" << std::endl;
        return;
    }
    if (fcntl(client_fd, F_SETFL, 0) == -1) {
        perror("fcntl error");
        close(client_fd);
        return;
    }
    server_list.push_back(client_fd);
}

ssize_t readData(int client_fd, char *buffer, size_t buffer_size) {
    ssize_t len = read(client_fd, buffer, buffer_size);
    if (len == 0) {
        std::cout << "client disconnected" << std::endl;
    } else if (len > 0) {
        return len;
    }
    return -1;
}

Request processRequest(const char *buffer, ssize_t len) {
    std::istringstream is(std::string(buffer, len));
    std::string line;
    std::ostringstream os;

    // std::cout << "----------------------------------------" << std::endl;
    while (std::getline(is, line)) {
        // std::cout << line << std::endl;
        os << line;
    }

    std::vector<std::string> buf = splitArgs(os.str(), " ");
    Request req(buf);
    return req;
}

void    sendResponse(int client_fd, Server &server, std::string req_path, int error_flag) {
    std::vector<Location>  locs = server.getLocations();
    std::string body;
    std::string response;
    bool    is_404 = true;

    if (error_flag > 400) { // 원래는 200
        body = makeBody(server, req_path, locs[0], error_flag);
        response = buildResponse(body, locs[0], server, error_flag);
        write(client_fd, response.c_str(), response.size());
        return ;
    }
    size_t i;
    for (i = 0; i < locs.size(); i++) {
        if (locs[i].getPath() == req_path) {
            Location    target = locs[i];
            if (target.getReturnValue().size() == 2) {
                std::vector<std::string> buf = target.getReturnValue();

                int head_number = atoi(buf[0].c_str());
                std::string return_path = buf[1];
                response = buildResponse(body, target, server, head_number);
                std::cout << response << std::endl;
                std::cout << return_path << std::endl;
                // 303 처리에 대한 문제점 : 한 fd당 한번의 send만 가능하나
                // 303 send 후 다시 리다이렉션시 소켓이 끊김
                // write(client_fd, response.c_str(), response.size());

                sendResponse(client_fd, server, return_path, 200);
                return ;
            }
            body = makeBody(server, req_path, target, error_flag);
            response = buildResponse(body, target, server, error_flag);
            is_404 = false;
            break;
        }  else if (locs[i].getAutoindex() == true && isDirect(server, locs[i], "." + req_path) == true) {
            body = makeBody(server, req_path, locs[i], error_flag);
            response = buildResponse(body, locs[i], server, error_flag);
            is_404 = false;
            break;
        }
    }
    if (is_404 == true) {
        error_flag = 404;
        body = makeBody(server, req_path, locs[0], error_flag);
        response = buildResponse(body, locs[0], server, error_flag);
    }
    std::cout << response << std::endl;
    write(client_fd, response.c_str(), response.size());
}


void    executeMethodPut(char buffer[], ssize_t len, std::vector<int> server_list, Server serv, std::string req_path) {
    std::string content(buffer + len, BUFFER_SIZE - len);
    std::fstream file(req_path, std::ios::out);
    if (file.is_open()) {
        file.write(content.c_str(), content.size());
        file.close();
        sendResponse(server_list.back(), serv, req_path, 200);
    } else {
        sendResponse(server_list.back(), serv, req_path, 500);
    }
}

void    executeMethodDelete(std::vector<int> server_list, Server serv, std::string req_path) {
    std::ifstream ifs(req_path.c_str());

    if (!ifs.good()) {
        sendResponse(server_list.back(), serv, req_path, 404);
        return ;
    }
    ifs.close();

    if (unlink(req_path.c_str()) == 0) {
        sendResponse(server_list.back(), serv, req_path, 200);
    } else {
        sendResponse(server_list.back(), serv, req_path, 500);
    }

}

void    executeMethodHead(int client_fd, Server &server, int error_flag) {
    std::vector<Location>   locs = server.getLocations();
    std::string body;
    std::string response;

    body = "";
    response = buildResponse(body, locs[0], server, error_flag);

    write(client_fd, response.c_str(), response.size());
}

void handle_cgi(int sockfd, Request req) {
    // 입력을 모두 읽어들입니다.
    std::string buffer;
    // char buf[1024];
    // ssize_t nread;
    // while ((nread = read(sockfd, buf, sizeof(buf))) > 0) {
    //     buffer += std::string(buf, nread);
    // }
    // if (nread == -1) {
    //     perror("read");
    //     exit(EXIT_FAILURE);
    // }

    // 요청 및 응답 헤더를 읽어들입니다.
    std::string header =
        "HTTP/1.1 200 OK\r\n"
        // "Content-Type: text/html;charset=UTF-8\r\n"
        "\r\n";

    // CGI 스크립트를 실행합니다.
    std::string cgi_path = "." + req.getPath();

    pid_t       pid;
    std::string output;

    int pipefd[2];

    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(1);
    }

    pid = fork();
    if (pid == 0) {
        char*   command[2];

        command[0] = const_cast<char*>(cgi_path.c_str());
        command[1] = NULL;

        char*   env[] = {NULL};

        close(pipefd[0]);

        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
        
        if (execve(command[0], command, env) < 0) {
            perror("execve");
            exit(1);
        }
        exit(0);
    } else {
        close(pipefd[1]);

        int status;

        waitpid(pid, &status, 0);
        
        if (WIFEXITED(status)) {
            char    buf[4096];
            ssize_t bytesRead = read(pipefd[0], buf, 4096);
            buffer[bytesRead] = '\0';

            // 응답 헤더를 출력
            std::cout << "Content-Type: text/html" << std::endl;
            std::cout << "Status: 200 OK" << std::endl << std::endl;
        } else {
            // 오류 발생 시 응답 헤더를 출력
            std::cout << "Content-Type: text/plain" << std::endl;
            std::cout << "Status: 500 Internal Server Error" << std::endl << std::endl;
            std::cout << "Error executing CGI script" << std::endl;
        }

        close(pipefd[0]); // 읽기 파이프를 닫음
    }

    // 출력 데이터를 클라이언트로 보냅니다.
    std::string response = header + output;
    // std::cout << response << std::endl;
    if (write(sockfd, response.c_str(), response.size()) == -1) {
        perror("send");
        exit(EXIT_FAILURE);
    }
}

// void    executeMethodPost(int client_fd, Server &server, Request req, char buffer[], int len) {
//     std::vector<Location>   locs = server.getLocations();
//     // std::string body;
//     // std::string response;

//     // body = readFilePath(req.getPath(), server.getErrorPages());
//     // response = buildResponse(body, locs[0], server, 200);
//     handleCGI()
//     std::cout << "----------------------------------------" << std::endl;
//     // write(client_fd, response.c_str(), response.size());
// }

void    ServerManage::runServer(void) {
    int server_count = serv.size();

    std::vector<int>    server_list;
    struct kevent   change_list[server_count], event_list[server_count];
    
    std::cout << "Setting Server" << std::endl;
    int idx = 0;
    std::vector<Server>::iterator iter;
    for (iter = serv.begin(); iter != serv.end(); iter++) {
        int fd = ::socket(AF_INET, SOCK_STREAM, 0);
        if (fd == -1)
            throw ErrorException("socket error");
        
        if (::fcntl(fd, F_SETFL, O_NONBLOCK) == -1)
            throw ErrorException("fcntl error");

        int option = 1;
        if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &option, (socklen_t)(sizeof(option))) < 0) {
            perror("setsockopt");
            exit(1);
        }
        
        if (::bind(fd, (struct sockaddr*)(*iter).getServerAddress(), sizeof(struct sockaddr)) < 0) {
            perror("bind error: ");
            throw ErrorException("bind error");
        }
        
        if (::listen(fd, SOMAXCONN) == -1)
            throw ErrorException("listen error");

        EV_SET(&change_list[idx++], fd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
        
        server_list.push_back(fd);
    }

    int kq = ::kqueue();
    if (kq == -1)
        throw ErrorException("failed to kqueue");

    // struct timespec timeout_specs;
    // timeout_specs.tv_sec = TIMEOUT_SEC;
    // timeout_specs.tv_nsec = 0;

    std::cout << "Running Server!" << std::endl;
    while (1) {
        int event_count = ::kevent(kq, change_list, server_count, event_list, server_count, NULL);
        if (event_count < 0) {
            throw ErrorException("failed to kevent");
        }
        else if (event_count == 0) {
            // time-out
            continue;
        }

        for (int i = 0; i < event_count; i++) {
            struct sockaddr_in client_addr;
            int sockfd = event_list[i].ident;
            int event_flags = event_list[i].flags;
            std::vector<int>::iterator iter = std::find(server_list.begin(), server_list.end(), sockfd);
            if (iter != server_list.end()) {
                int index = std::distance(server_list.begin(), iter);
                std::map<int, std::string>  header_list = serv[index].getErrorPagesHeader();
                if (event_list[i].filter == EVFILT_READ) {
                    acceptClient(sockfd, client_addr, server_list);

                    if (event_flags & EV_EOF) {
                        std::cout << "Client disconnected" << std::endl;
                    } else if (event_flags & EV_ERROR) {
                        std::cout << "Error occurred" << std::endl;
                    }
                    
                    char buffer[BUFFER_SIZE];
                    ssize_t len = readData(server_list.back(), buffer, BUFFER_SIZE);
                    if (len > 0) {
                        Request     req = processRequest(buffer, len);
                        std::string req_path = req.getPath();
                        std::string req_method = req.getMethod();
                        // if (req_method == "POST") {
                        //     std::string post_buffer(buffer, len);
                        //     size_t  pos = post_buffer.find("\r\n\r\n");
                        //     if (pos != std::string::npos) {
                        //         std::string post_body = post_buffer.substr(pos + 4);
                        //         req.setBody(post_body);
                        //     }
                        // }

                        if (req_method == "GET")
                            sendResponse(server_list.back(), serv[index], req_path, 200);
                        else if (req_method == "PUT")
                            executeMethodPut(buffer, len, server_list, serv[index], "." + req_path);
                        else if (req_method == "DELETE")
                            executeMethodDelete(server_list, serv[index], "." + req_path);
                        // else if (req_method == "POST")
                        //     handle_cgi(server_list.back(), req);
                            // executeMethodPost(server_list.back(), serv[index], req, buffer, len);
                        else if (req_method == "HEAD")
                            executeMethodHead(server_list.back(), serv[index], 200);
                        else
                            sendResponse(server_list.back(), serv[index], req_path, 405);
                    }
                } else if (event_list[i].filter == EVFILT_WRITE) {
                    char    buffer[BUFFER_SIZE];
                    ssize_t len = write(sockfd, buffer, BUFFER_SIZE);

                    if (len > 0) {
                        std::cout << "Sent " << len << "bytes to client" << std::endl;
                    } else {
                        EV_SET(&change_list[i], sockfd, EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
                        if (kevent(kq, &change_list[i], 1, NULL, 0, NULL) == -1)
                            throw ErrorException("failed to delete event");
                    }
                    close(sockfd);
                    std::cout << "Connection closed with client " << std::endl;
                }

                close(server_list.back());
                server_list.pop_back();
            }
        }
        std::cout << "Success!" << std::endl;
    }
}
