#include "library.hpp"

ParseConfig::ParseConfig(void) {
}

ParseConfig::~ParseConfig() {}

void    ParseConfig::parsingFile(const std::string& filename) {
    std::ifstream   config_file(filename.c_str());
    //file format check?
    //file size check?
    if (!config_file.is_open())
        throw ErrorException("failed to open file.");
    std::stringstream   buffer;
    buffer << config_file.rdbuf();
    std::string fileContents = buffer.str();
    deleteComments(fileContents);
    deleteIsspace(fileContents);
    splitServer(fileContents);
    if (servers_config.size() == 0)
		throw ErrorException("no server config");
	for (size_t i = 0; i < servers_config.size(); i++) {
		Server new_server;
		createServer(servers_config[i], new_server);
		new_server.setMemberRepository(&this->memberRepository);
		servers.push_back(new_server);
	}
}

ParseConfig&	ParseConfig::operator=(const ParseConfig& param) {
	if (this != &param) {
		servers = param.servers;
		servers_config = param.servers_config;
	}
	return (*this);
}

std::vector<Server>	ParseConfig::getServers(void) {
	return servers;
}

void    ParseConfig::deleteIsspace(std::string& contents) {
    size_t	i = 0;

	while (contents[i] && isspace(contents[i]))
		i++;
	contents = contents.substr(i);
	i = contents.length() - 1;
	while (i > 0 && isspace(contents[i]))
		i--;
	contents = contents.substr(0, i + 1);
}

void    ParseConfig::deleteComments(std::string& contents) {
    size_t  pos;

    pos = contents.find('#');
    while (pos != std::string::npos) {
        size_t  pos_nl = contents.find('\n', pos);
        contents.erase(pos, pos_nl - pos);
        pos = contents.find('#');
    }
}

void    ParseConfig::splitServer(std::string& contents) {
    size_t  start = 0;
    size_t  end = 1;

    while (start != end && start < contents.length()) {
        start = findStart(start, contents);
        end = findEnd(start, contents);
		if (start == end)
			throw ErrorException("wrong use of scope");
		servers_config.push_back(contents.substr(start, end - start + 1));
		start = end + 1;
    }
}

size_t  ParseConfig::findStart(size_t start, std::string& contents) {
    size_t i;

	for (i = start; contents[i]; i++) {
		if (contents[i] == 's')
			break ;
		if (!std::isspace(contents[i]))
			throw  ErrorException("wrong character out of server scope");
	}
	if (!contents[i])
		return (start);
	if (contents.compare(i, 6, "server") != 0)
		throw ErrorException("wrong charecter of server");
	i += 6;
	while (contents[i] && std::isspace(contents[i]))
		i++;
	if (contents[i] == '{')
		return (i);
	else
		throw ErrorException("wrong fotmat");
}

size_t  ParseConfig::findEnd(size_t start, std::string& contents) {
    size_t	i;
	size_t	scope;
	
	scope = 0;
	for (i = start + 1; contents[i]; i++) {
		if (contents[i] == '{')
			scope++;
		if (contents[i] == '}') {
			if (!scope)
				return (i);
			scope--;
		}
	}
	return (start);
}

void	ParseConfig::createServer(std::string& config, Server& serv) {
	std::vector<std::string>	arg;
	std::vector<std::string>	errors;
	std::vector<std::string>	try_file;
	std::vector<Location>		locs;
	

	arg = splitArgs(config += ' ', std::string(" \n\t"));
	for (size_t i = 0; i < arg.size(); i++) {
		if (arg[i] == "listen") {
			if (serv.getPort() != 0)
				throw ErrorException("port duplicated");
			serv.setPort(arg[++i]);
		}
		else if (arg[i] == "server_name") {
			if (serv.getServerName() != "")
				throw ErrorException("server_name duplicated");
			serv.setServerName(arg[++i]);
		}
		else if (arg[i] == "host")
			serv.setHost(arg[++i]);
		else if (arg[i] == "root")
			serv.setRoot(arg[++i]);
		else if (arg[i] == "index")
			serv.setIndex(arg[++i]);
		else if (arg[i] == "client_max_body_size")
			serv.setClntMaxBodySize(arg[++i]);
		else if (arg[i] == "autoindex")
			serv.setAutoindex(arg[++i]);
		else if (arg[i] == "location") {
			Location	loc;

			loc.setPath(arg[++i]);
			if (arg[++i] != "{")
				throw ErrorException("config location scope");
			i++;
			while (i < arg.size() && arg[i] != "}") {
				if (arg[i] == "root")
					loc.setRoot(arg[++i]);
				else if (arg[i] == "index")
					loc.setIndex(arg[++i]);
				else if (arg[i] == "return") {
					loc.setReturnValue(arg[++i]);
				}
				else if (arg[i] == "alias")
					loc.setAlias(arg[++i]);
				else if (arg[i] == "autoindex")
					loc.setAutoindex(arg[++i]);
				else if (arg[i] == "try_files") {
					std::vector<std::string>	files;

					while (i < arg.size()) {
						files.push_back(arg[++i]);
						if (arg[i].find(';') != std::string::npos)
							break ;
					}
					if (i == arg.size())
						throw ErrorException("location try_files");
					loc.setTryFiles(files);
				}
				else if (arg[i] == "cgi_path") {
					std::vector<std::string>	paths;

					while (i < arg.size()) {
						paths.push_back(arg[++i]);
						if (arg[i].find(';') != std::string::npos)
							break ;
					}
					if (i == arg.size())
						throw ErrorException("location cgi_path");
					loc.setCgiPath(paths);
				}
				else if (arg[i] == "cgi_text") {
					std::vector<std::string>	texts;

					while (i < arg.size()) {
						texts.push_back(arg[++i]);
						if (arg[i].find(';') != std::string::npos)
							break ;
					}
					if (i == arg.size())
						throw ErrorException("location cgi_text");
					loc.setCgiText(texts);
				}
				else if (arg[i] == "client_max_body_size")
					loc.setClntMaxBodySize(arg[++i]);
				else if (arg[i] == "allow_methods") {
					std::vector<std::string>	buf;

					while (i < arg.size()) {
						buf.push_back(arg[++i]);
						if (arg[i].find(";") != std::string::npos) {
							break ;
						}
					}
					loc.setMethods(buf);
				}
				else {
					std::cout << arg[i] << std::endl;
					throw ErrorException("wrong config argument");
				}
				i++;
			}
			locs.push_back(loc);
		}
		else if (arg[i] == "error_page") {
			while (i < arg.size()) {
				errors.push_back(arg[++i]);
				if (arg[i].find(';') != std::string::npos)
					break ;
			}
			if (i == arg.size())
				throw ErrorException("server error_page");
		}
	}
	serv.setLocation(locs);
	serv.setErrorPages(errors);
	serv.setServerAddress();
	if (serv.getPort() == 0)
		throw ErrorException("undefined port");
	if (serv.getHost() == 0)
		throw ErrorException("undefined host");
	if (serv.getRoot() == "")
		throw ErrorException("undefined root");
}

void	ParseConfig::print(void) {
	int idx = 0;
	for (std::vector<Server>::iterator iter = servers.begin(); iter != servers.end(); iter++) {
		std::cout << idx++ << std::endl;
		(*iter).print();
	}
}