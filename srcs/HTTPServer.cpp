#include "HTTPServer.hpp"

namespace webserv {

HTTPServer::HTTPServer(int argc, char* argv[])
    : config(new Config(argc, argv)) {}

HTTPServer::~HTTPServer(void) {}

void HTTPServer::start(void) {}

void HTTPServer::run(void) {}

void HTTPServer::stop(void) {}

} // namespace webserv
