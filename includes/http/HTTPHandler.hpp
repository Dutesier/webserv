#ifndef HTTP_HANDLER_HPP
#define HTTP_HANDLER_HPP

#include "socket/TCPSocket.hpp"
#include "http/HTTPRequest.hpp"
#include "http/HTTPResponse.hpp"
#include "server/ServerConfig.hpp"
#include "utils/smt.hpp"

namespace webserv {

void http_handle(TCPSocket* sock, int client_fd);
HTTPResponse	generate_response(std::pair<int, std::string> resp);

} // namespace webserv

#endif /* HTTP_HANDLER_HPP */
