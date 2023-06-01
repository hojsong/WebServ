#include "library.hpp"

std::vector<std::string>	splitArgs(std::string line, std::string sep) {
	std::vector<std::string>	str;
	size_t	start = 0;
	size_t	end = 0;

	while (1) {
		end = line.find_first_of(sep, start);
		if (end == std::string::npos)
			break ;
		std::string parsed = line.substr(start, end - start);
		str.push_back(parsed);
		start = line.find_first_not_of(sep, end);
		if (start == std::string::npos)
			break ;
	}
	return str;
}
