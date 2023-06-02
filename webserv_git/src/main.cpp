#include "../includes/ServerManage.hpp"
#include "../includes/ConfigParse.hpp"
#include "../includes/Server.hpp"

int	main(int argc, char **argv) {
	ServerManage manager;

	if (argc > 2) {
		std::cerr << "Invalid argument number." << std::endl;
		return (1);
	}
	if (argv[1])
		manager.serverRun(argv[1]);
	else
		manager.serverRun("default");
	return 0;
}
