#include "http/HTTPHandler.hpp"

namespace webserv {

/* HTTPHandler Class */
void http_handle(smt::shared_ptr<ServerSocket>     sock,
                 smt::shared_ptr<SocketConnection> connection, int client_fd) {

    // receiving request string
    std::string req_str = sock->recv(client_fd);

    // getting the first request from string
    smt::shared_ptr<HTTPRequest> request =
        connection->m_parser->getNextRequest(req_str);
    if (request->isValid()) {

        LOG_D("Handling request...");

        // Find the appropriate ServerBlock
        int serverBlockIdx = sock->bestServerBlockForRequest(request);
        serverBlockIdx = (serverBlockIdx == -1 ? 0 : serverBlockIdx);

        // handle request here
        smt::shared_ptr<HTTPResponse> response = process_request(
            request, sock->m_blocks[serverBlockIdx],
            client_fd); // TODO: This obviously needs to be fixed, just using
                        // the first block here so that rest of code can run

        // sending response to client
        if (response) sock->send(client_fd, response->to_str());

        // closing the connection
        std::map<int, smt::shared_ptr<webserv::SocketConnection> >::iterator
            connnectionIterator;
        connnectionIterator = sock->m_connection.find(client_fd);
        if (connnectionIterator != sock->m_connection.end()) {
            // Remove connection from map
            sock->m_connection.erase(client_fd);
        }
    }
    LOG_D("No valid request was received");
}

smt::shared_ptr<HTTPResponse>
    process_request(smt::shared_ptr<HTTPRequest> request,
                    smt::shared_ptr<ServerBlock> config, int client_fd) {

    smt::shared_ptr<webserv::LocationBlock> loc =
        config->getLocationBlockForRequest(request);

    bool runCGI =
        (loc) && (!loc->m_cgi_enabled.empty()) && (loc->m_cgi->isValid());
    bool isCGI = request->isCGIRequest();
    if (isCGI && runCGI) {
        LOG_D("Running CGI script");
        return (loc->m_cgi->run(request, client_fd));
    }
    // getting method
    // if (request->getMethod == "GET") { return (Method::get(request)); }
    // if (request->getMethod == "POST") { return (Method::post(request)); }

    // I think we can deal with GET and POST like this, on the CGI, and thats
    // that, then all we need is DELETE. I'm not sure how to deal with that

    // if (request->getMethod == "DELETE") {
    // int status = remove(request->getResource().c_str());

    // if (status == 0)
    //     return (200);
    // else
    //     return (404);
    // return (Method::delete(request)); }

    return (generate_error_response(405, config));
}

smt::shared_ptr<HTTPResponse>
    generate_error_response(int code, smt::shared_ptr<ServerBlock> config) {

    (void)code;
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
