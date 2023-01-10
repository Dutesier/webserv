#ifndef HTTP_HANDLER_HPP
#define HTTP_HANDLER_HPP

#include "socket/TCPSocket.hpp"
#include "http/HTTPRequest.hpp"
#include "utils/smt.hpp"

namespace webserv {

void http_handle(TCPSocket* sock, int client_fd);

} // namespace webserv

#endif /* HTTP_HANDLER_HPP */
