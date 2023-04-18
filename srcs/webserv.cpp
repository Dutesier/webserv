#include "webserv.hpp"

#include <csignal>
#include <iostream>

static smt::shared_ptr<webserv::HTTPServer>& get_webserv(int   argc = 0,
                                                         char* argv[] = NULL);

void webserv::webserv(int argc, char* argv[]) {

    try {

        signal(SIGINT, webserv::stop);

        LOG("reading config file...");
        smt::shared_ptr<webserv::HTTPServer> srv = get_webserv(argc, argv);

        LOG("setting up server...");
        srv->start();

        LOG("starting server...");
        srv->run();
    }
    catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }
}

void webserv::stop(int signum) {

	(void)signum;

    try {

        smt::shared_ptr<webserv::HTTPServer> srv = get_webserv();
        LOG("stoping server..." + signum);
        srv->stop();
    }
    catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }
}

static smt::shared_ptr<webserv::HTTPServer>& get_webserv(int   argc,
                                                         char* argv[]) {

    static smt::shared_ptr<webserv::HTTPServer> srv(
        new webserv::HTTPServer(argc, argv));
    return (srv);
}
