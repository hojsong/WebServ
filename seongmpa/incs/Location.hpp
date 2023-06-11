#ifndef LOCATION_HPP
#define LOCATION_HPP

#include "library.hpp"

class Location {
    private:
        std::string					path;
		std::string					root;
		std::string					index;
		std::vector<int>			methods; //0:GET 1:POST 2:DELETE 3:PUT 4:HEAD
		std::string					return_value;
		std::string					alias;
		std::vector<std::string>	try_files;
		std::vector<std::string>	cgi_path;
		std::vector<std::string>	cgi_text;
		bool						autoindex;
		unsigned long				client_max_body_size;

	public:
		Location(void);
		~Location();

		//setter
		void	setPath(std::string param);
		void	setRoot(std::string param);
		void	setAutoindex(std::string param);
		void	setIndex(std::string param);
		void	setMethods(std::vector<std::string> param);
		void	setReturnValue(std::string param);
		void	setAlias(std::string param);
		void	setTryFiles(std::vector<std::string> param);
		void	setCgiPath(std::vector<std::string> param);
		void	setCgiText(std::vector<std::string> param);
		void	setClntMaxBodySize(std::string param);

		//getter
		std::string					getPath(void);
		std::string					getRoot(void);
		bool						getAutoindex(void);
		std::string					getIndex(void);
		std::vector<int>			getMethods(void);
		std::string					getReturnValue(void);
		std::string					getAlias(void);
		std::vector<std::string>	getTryFiles(void);
		std::vector<std::string>	getCgiPath(void);
		std::vector<std::string>	getCgiText(void);
		unsigned long				getClntMaxBodySize(void);

		void	print(void);
};

#endif