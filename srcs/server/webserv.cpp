#include "webserv.hpp"

namespace webserv {

static void stop(int);

void webserv(std::string const& filename) {
    signal(SIGINT, stop);

    LOG_I("Reading config file: " << filename << "...");
    try {
        config::parse(filename);
    }
    catch (std::exception& e) {
        LOG_F("Failed parsing file " << filename << ": "
                                     << std::string(e.what()));
        return;
    }

    LOG_I("Starting server...");
    try {
        Server::start();
    }
    catch (std::exception& e) {
        LOG_F("Failure while server was starting: " << std::string(e.what()));
        return;
    }

    LOG_I("Server is running. Press Ctrl+C to stop.");
    try {
        Server::run();
    }
    catch (std::exception& e) {
        LOG_F("Failure while server was running: " << std::string(e.what()));
        return;
    }
}

static void stop(int) {
    LOG_I("Stopping server...");
    try {
        Server::stop();
    }
    catch (std::exception& e) {
        LOG_F("Failure while server was stopping: " << std::string(e.what()));
        return;
    }
    LOG_I("Server stopped.");
}

} // namespace webserv
