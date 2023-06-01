#include "library.hpp"

int main(int ac, char** av) {
    if (ac > 2) {
        std::cout << "Error: Invalid arguments" << std::endl;
        return (1);
    }
    try {
        std::string config;
        config = (ac == 1 ? "config/default.conf" : av[1]);

        ParseConfig parsed;
        parsed.parsingFile(config);
        // parsed.print();
        
        ServerManage manage;
        manage.setupServer(parsed);
        manage.runServer();
        // manage.testServer();

    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
        return (1);
    }
    // system("leaks webserv");
}