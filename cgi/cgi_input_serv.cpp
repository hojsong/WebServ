// file save;
void saveToFile(MemberRepository *mr, const std::string& filename) {
    std::ofstream file(filename);
    if (file.is_open()) {
        for (size_t i = 0; i < mr->getMemberSize(); i++){
			Member mem = mr->getNumMember(i);
			file << "id=" << mem.getById();
			file << "&password=" << mem.getPassword();
			file << "&name=" << mem.getName();
			file << "&grade=" << mem.getGrade();
			file << "&\n";
		}
        file.close();
	}
}

// file delete;
void deleteFile(const std::string& filename) {
    unlink(filename.c_str());
}

// buf == read() || recv() , mr = MemberRepository Class
std::string cgi_execve(char *buf, MemberRepository *mr){
	deleteFile("memberdb.txt");
	saveToFile(mr, "memberdb.txt");
	int cgiInput[2], cgiOutput[2];
    pid_t pid;
    int status;
    if (pipe(cgiInput) < 0 || pipe(cgiOutput) < 0) {
        perror("pipe error");
        return ;
    }
    if ((pid = fork()) < 0) {
        perror("fork error");
        return ;
    }
    if (pid == 0) {
        dup2(cgiInput[0], 0);
        dup2(cgiOutput[1], 1);
        close(cgiInput[1]);
        close(cgiOutput[0]);
        char *execPath = const_cast<char*>("cgi.exe");
    	char *args[] = {execPath, buf, NULL};
        execve(execPath, args, NULL);
        exit(0);
    }
    else {
        close(cgiInput[0]);
        close(cgiOutput[1]);
        // write(cgiInput[1], req.getBody().c_str(), req.getBody().size());
        close(cgiInput[1]);
        std::string cgi_output;
        char buf[1024];
        ssize_t bytesRead;
        while ((bytesRead = read(cgiOutput[0], buf, sizeof(buf))) > 0) {
            cgi_output.append(buf, bytesRead);
        }
        close(cgiOutput[0]);
        waitpid(pid, &status, 0);
        std::cout << "-------------CGI OUTPUT-------------" << std::endl;
        std::cout << cgi_output << std::endl;
        std::cout << "------------------------------------" << std::endl;
    }
	return cgi_output();
}

