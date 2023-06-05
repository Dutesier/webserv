#include "server/Middleware.hpp"

#include "config/Options.hpp"
#include "http/Handler.hpp"
#include "http/Request.hpp"
#include "http/Response.hpp"
#include "sockets/ServerSocket.hpp"

namespace webserv {

void Middleware::handleRecv(smt::shared_ptr<net::ServerSocket> sock, int fd) {
    smt::shared_ptr<config::Opts> opts(new config::Opts());

    // receiving request string
    std::string reqStr = sock->recv(fd);

    int status = 0;
    reqStr = getNextRequest(reqStr);
    while (!reqStr.empty()) {

        smt::shared_ptr<http::Request> request;
        try {
            request = smt::make_shared(new http::Request(reqStr));
            LOG_I("Received Request" << std::endl << request->toString());
        }
        catch (http::Request::MalformedRequestException const&) {
            LOG_E("Malformed request");
            LOG_D("TODO: Here we should remove the socket and continue");
            status = 404;
        }

        try {
            opts = getOptions(sock, request);
        }
        catch (config::Options::NoSuchOptionsException const&) {
            LOG_E("No config found for this request");
            LOG_D("TODO: Here we should remove the socket and continue");
            status = 500;
        }

        // handle request
        smt::shared_ptr<http::Response> response =
            http::processRequest(status, request, opts);

        LOG_D("Response: " << std::endl << response->toString());

        // send response
        sock->send(fd, response->toString());

        reqStr = getNextRequest();
    }
}

std::string Middleware::getNextRequest(std::string const& reqStr) {
    static std::string buff;

    if (!reqStr.empty()) { buff = reqStr; }
    std::string ret;

    size_t endHeaders = buff.find("\r\n\r\n");
    // Request is incomplete
    if (endHeaders == std::string::npos) {
        ret = buff;
        buff.clear();
        return (ret);
    }

    // getting request until the end of headers
    ret = buff.substr(0, endHeaders + 4);

    // getting Content-Length
    size_t pos;
    if ((pos = ret.find("Content-Length: ")) != std::string::npos) {

        // getting Content-Length
        std::string        l;
        std::istringstream iss(buff.substr(pos + 16));
        iss >> l;

        // converting to int
        int               len;
        std::stringstream ss(l);
        ss >> len;

        // adding body to ret
        if (len) { ret += buff.substr(endHeaders + 4, len); }
    }

    buff = buff.substr(ret.size());

    return (ret);
}

smt::shared_ptr<config::Opts>
    Middleware::getOptions(smt::shared_ptr<net::ServerSocket> sock,
                           smt::shared_ptr<http::Request>     request) {

    std::stringstream ss;
    ss << sock->getPort();
    return (config::Options::getOptions(ss.str(), sock->getHost(),
                                        request->getPath(),
                                        request->getHeader("Host")));
}
} // namespace webserv