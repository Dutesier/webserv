#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include "server/HTTPServer.hpp"

namespace webserv {

void webserv(int argc, char* argv[]);
void stop(int signum);

} // namespace webserv

#endif /* WEBSERV_HPP */
