#ifndef HTTP_HANDLER_HPP
#define HTTP_HANDLER_HPP

#include "http/HTTPRequest.hpp"
#include "http/HTTPResponse.hpp"
#include "http/HTTPParser.hpp"
#include "socket/ServerSocket.hpp"
#include "utils/smt.hpp"

namespace webserv {

void http_handle(smt::shared_ptr<ServerSocket> sock, int client_fd);
smt::shared_ptr<HTTPResponse> generate_error_response(int code, smt::shared_ptr<ServerBlock> config);
smt::shared_ptr<HTTPResponse> process_request(smt::shared_ptr<HTTPRequest> request, smt::shared_ptr<ServerBlock> config);

} // namespace webserv

#endif /* HTTP_HANDLER_HPP */
