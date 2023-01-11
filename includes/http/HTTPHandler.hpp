#ifndef HTTP_HANDLER_HPP
#define HTTP_HANDLER_HPP

#include "http/HTTPRequest.hpp"
#include "socket/ServerSocket.hpp"
#include "utils/smt.hpp"

namespace webserv {

void http_handle(smt::shared_ptr<ServerSocket> sock, int client_fd);

} // namespace webserv

#endif /* HTTP_HANDLER_HPP */
