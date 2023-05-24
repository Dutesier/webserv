#include "http/HTTPHandler.hpp"

#include "http/methods.hpp"

#include <utils/Logger.hpp>

namespace webserv {

/* HTTPHandler Class */
void http_handle(smt::shared_ptr<ServerSocket>     sock,
                 smt::shared_ptr<SocketConnection> connection, int client_fd) {

    // receiving request string
    std::string req_str = sock->recv(client_fd);
    bool        answeredAtLeastOneRequest = false;

    // getting the first request from string
    // while there are requests to process we process,
    // once we've reached end of file we stop processing
    bool keepAskingForRequests = true;
    while (keepAskingForRequests) {
        LOG_D("Looking for the next request that might be cached");
        std::pair<smt::shared_ptr<HTTPRequest>, bool> requestPair =
            connection->m_parser->getNextRequest(req_str);
        smt::shared_ptr<HTTPRequest> request = requestPair.first;
        keepAskingForRequests = requestPair.second;
        if (request->isValid()) {

            LOG_D("Valid request was received. Request: " +
                  request->toString());

            // Find the appropriate ServerBlock
            int serverBlockIdx = sock->bestServerBlockForRequest(request);
            serverBlockIdx = (serverBlockIdx == -1 ? 0 : serverBlockIdx);
            // handle request here
            smt::shared_ptr<HTTPResponse> response =
                process_request(request, sock->m_blocks[serverBlockIdx],
                                client_fd); // TODO: This obviously needs to be
                                            // fixed, just using the first block
                                            // here so that rest of code can run

            // sending response to client
            if (response) {
                LOG_D(("Trying to send to fd " + client_fd) +
                      (" response: " + response->to_str()));
                sock->send(client_fd, response->to_str());
                answeredAtLeastOneRequest = true;
            }
            req_str = "";
        }
        else { LOG_D("No valid request was received"); }
    }

    if (answeredAtLeastOneRequest) {
        // closing the connection
        LOG_D("Closing socket connection to fd:" + client_fd);
        std::map<int, smt::shared_ptr<webserv::SocketConnection> >::iterator
            connnectionIterator;
        connnectionIterator = sock->m_connections.find(client_fd);
        if (connnectionIterator != sock->m_connections.end()) {
            // Remove connection from map
            sock->m_connections.erase(client_fd);
        }
    }
}

smt::shared_ptr<HTTPResponse>
    process_request(smt::shared_ptr<HTTPRequest> request,
                    smt::shared_ptr<ServerBlock> config, int client_fd) {

    LOG_D("Processing Request");
    smt::shared_ptr<webserv::LocationBlock> location =
        config->getLocationBlockForRequest(request);

    // What do we do if we cant get location?
    if (!location) {
        LOG_D("No location found");
        return (generate_error_response(405, config));
    }

    bool runCGI =
        (location) && (location->m_cgi_enabled) && (location->m_cgi->isValid());
    bool isCGI = request->isCGIRequest();
    if (isCGI && runCGI) {
        LOG_D("Running CGI script");
        return (location->m_cgi->run(request, client_fd));
    }

    // getting method
    if (request->getMethod() == webserv::GET) {
        return (webserv::methods::GET(request, location));
    }
    if (request->getMethod() == webserv::POST) {
        return (webserv::methods::POST(request, location));
    }
    if (request->getMethod() == webserv::DELETE) {
        return (webserv::methods::DELETE(request, location));
    }

    return (generate_error_response(405, config));
}

smt::shared_ptr<HTTPResponse>
    generate_error_response(int code, smt::shared_ptr<ServerBlock> config) {

    (void)config;
    return (smt::shared_ptr<HTTPResponse>(
        new HTTPResponse(code, std::map<std::string, std::string>(), "")));

    // std::string                        body;
    // std::map<std::string, std::string> header;
    // std::stringstream                  ss;
    //
    // // getting custom body
    // bool default_body = false;
    // if (config->m_error_page.find(code) != config->m_error_page.end()) {
    //     std::ifstream file(config->m_error_page[code]);
    //     if (!file.good()) {
    //         FLOG_I("Fail to open " + config->m_error_page[code]);
    //         default_body = true;
    //     }
    //     else {
    //         body = std::string((std::istreambuf_iterator<char>(file)),
    //                            std::istreambuf_iterator<char>());
    //     }
    //     file.close();
    //     // if something goes wrong
    //     // default_body = true;
    // }
    // else { default_body = true; }
    //
    // // getting default body
    // if (default_body) {
    //
    //     // converting code to str
    //     ss << code;
    //     std::string error_code = ss.str();
    //
    //     // generating message
    //     std::string msg = error_code + " - " +
    //     HTTPResponse::s_status_map[code]; body = "<!DOCTYPE html>"
    //            "<html>"
    //            "  <head>"
    //            "    <title>" +
    //            msg +
    //            "</title>"
    //            "  </head>"
    //            "  <body>"
    //            "    <h1>" +
    //            msg +
    //            "</h1>"
    //            "  </body>"
    //            "</html>";
    // }
    //
    // // converting body.size() to str
    // ss.str("");
    // ss << body.size();
    // // setting headers
    // header["Content-Length"] = ss.str();
    //
    // return (
    //     smt::shared_ptr<HTTPResponse>(new HTTPResponse(code, header, body)));
}

} // namespace webserv
