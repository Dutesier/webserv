#ifndef HTTP_HANDLER_HPP
#define HTTP_HANDLER_HPP

#include "http/HTTPParser.hpp"
#include "http/HTTPRequest.hpp"
#include "http/HTTPResponse.hpp"
#include "socket/ServerSocket.hpp"
#include "utils/smt.hpp"

namespace webserv {

int httpHandle(smt::shared_ptr<ServerSocket> sock, int connectionFd);
smt::shared_ptr<HTTPResponse>
    generateErrorResponse(int code, smt::shared_ptr<ServerBlock> config);
smt::shared_ptr<HTTPResponse>
    processRequest(smt::shared_ptr<HTTPRequest> request,
                    smt::shared_ptr<ServerBlock> config, int connectionFd);

} // namespace webserv

#endif /* HTTP_HANDLER_HPP */
