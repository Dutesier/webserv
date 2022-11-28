#include "webserv.hpp"

#include <csignal>
#include <iostream>

webserv::HTTPServer* g_webserv;

void webserv::webserv(int argc, char* argv[]) {
    try {
        g_webserv = new HTTPServer(argc, argv);
    } catch (std::exception& e) {
        std::cout << "failure on parser: " << e.what() << std::endl;
    }

    try {
        g_webserv->start();
    } catch (std::exception& e) {
        std::cout << "failure on start: " << e.what() << std::endl;
    }

    try {
        g_webserv->run();
    } catch (std::exception& e) {
        std::cout << "failure on run: " << e.what() << std::endl;
    }
    webserv::stop(SIGSTOP);
}

void webserv::stop(int signum) {
    g_webserv->stop();
    delete g_webserv;
    exit(signum);
}
