#include "webserv.hpp"

#include <csignal>
#include <iostream>

webserv::HTTPServer* g_webserv;

void webserv::webserv(int argc, char* argv[]) {
    try {
        signal(SIGINT, webserv::stop);
        g_webserv = new HTTPServer(argc, argv);
        g_webserv->start();
        g_webserv->run();
        // webserv::stop(SIGSTOP); // uncomment this when using valgrind
    } catch (std::exception& e) { std::cout << e.what() << std::endl; }
}

void webserv::stop(int signum) {
    try {
        g_webserv->stop();
        delete g_webserv;
    } catch (std::exception& e) { std::cout << e.what() << std::endl; }
}
