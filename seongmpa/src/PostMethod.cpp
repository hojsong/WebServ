#include "../includes/PostMethod.hpp"

PostMethod::PostMethod() {}

PostMethod::PostMethod(Request request) {
	this->_headers = request.getHeaders();
	this->_body = request.getBody();
}

int
PostMethod::executeCgi(std::string cgiPath, int clientSock) {
	int cgiInput[2], cgiOutput[2];
	pid_t pid;
	int status;

	if (pipe(cgiInput) < 0 || pipe(cgiOutput) < 0) {
		perror("pipe error");
		return -1;
	}

	if ((pid = fork()) < 0) {
		perror("fork error");
		return -1;
	}

	if (pid == 0) {
		dup2(cgiInput[0], 0);
		dup2(cgiOutput[1], 1);
		close(cgiInput[1]);
		close(cgiOutput[0]);

		std::string contentLength = "CONTENT_LENGTH=" + std::to_string(this->_body.size());
		char* cl_env = new char[contentLength.size() + 1];
		strncpy(cl_env, contentLength.c_str(), contentLength.size());
		cl_env[contentLength.size()] = '\0';
		//std::copy(contentLength.begin(), contentLength.end(), cl_env);
		//cl_env[contentLength.size()] = '\0';

		char* envp[] = {cl_env, NULL};

		char* path = new char[cgiPath.size() + 1];
		strncpy(path, cgiPath.c_str(), cgiPath.size());
		path[cgiPath.size()] = '\0';
		//char* path = new char[cgiPath.size() + 1];
		//std::copy(cgiPath.begin(), cgiPath.end(), path);
		//path[cgiPath.size()] = '\0';

		char* const argv[] = {path, NULL};
		//std::cerr << "path: " << path << std::endl;
		//std::cerr << "argv: " << argv[0] << std::endl;
		//std::cerr << "envp: " << envp[0] << std::endl; 
		if (execve(path, argv, envp) == -1) {

			delete[] cl_env;
			delete[] path;
		}
		exit(0);
	}
	else {
		close(cgiInput[0]);
		close(cgiOutput[1]);

		write(cgiInput[1], this->_body.c_str(), this->_body.size());
		close(cgiInput[1]);

		std::string cgi_output;
		char buf[1024];
		ssize_t bytesRead;
		while ((bytesRead = read(cgiOutput[0], buf, sizeof(buf))) > 0) {
			cgi_output.append(buf, bytesRead);
		}

		close(cgiOutput[0]);
		waitpid(pid, &status, 0);

		//std::cout << "-------------CGI OUTPUT-------------" << std::endl;
		//std::cout << cgi_output << std::endl;
		//std::cout << "------------------------------------" << std::endl;
		std::string http_response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: " + std::to_string(cgi_output.length()) + "\r\n\r\n" + cgi_output;
		send(clientSock, http_response.c_str(), http_response.length(), 0);

		return 0;
	}
}

PostMethod::~PostMethod() {}