#ifndef PARSECONFIG_HPP
#define PARSECONFIG_HPP

#include "library.hpp"

class   MemberRepository;
class   Server;

class   ParseConfig {
    private:
        std::vector<Server>         servers;
        std::vector<std::string>    servers_config;
        MemberRepository            memberRepository;
    
    public:
        ParseConfig(void);
        ~ParseConfig();
        ParseConfig&    operator=(const ParseConfig& param);

        void    parsingFile(const std::string& filename);
		void	deleteComments(std::string& contents);
		void	deleteIsspace(std::string& contents);
		void	splitServer(std::string& contents);
		size_t	findStart(size_t start, std::string& contents);
		size_t	findEnd(size_t start, std::string& contents);
		void	createServer(std::string& config, Server& serv);

        void                print(void);
        std::vector<Server> getServers(void);    

};

#endif