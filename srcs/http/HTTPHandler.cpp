#include "http/HTTPHandler.hpp"

namespace webserv {

/* HTTPHandler Class */
// TODO: make sure there are no more requests in GetNextRequest
int http_handle(smt::shared_ptr<ServerSocket> sock, int client_fd) {

    try {
        // receiving request string
        std::string msg = sock->recv(client_fd);
        if (msg.empty()) { return (0); }
        FLOG_D("Received " + msg + " from client");

        // getting first request from string (there can be more than one
        // request, or request can be incomplete)
        std::string req_str = sock->getNextRequest(client_fd, msg);
        // if request is incomplete, return 1
        if (req_str.empty()) { return (1); }

        // handling all requests
        while (!req_str.empty()) {

            FLOG_D("Received a full request from client");
            // creating request object
            smt::shared_ptr<HTTPRequest> req(new HTTPRequest(req_str));
            // getting the correct configuration block according to request
            smt::shared_ptr<ServerBlock> config_block =
                sock->getConfigFromRequest(req);
            // generating a response
            smt::shared_ptr<HTTPResponse> resp =
                process_request(req, config_block, client_fd);
            FLOG_D("sending response " + resp->toStr() + "to client");
            // sending response to client
            if (resp) { sock->send(client_fd, resp->toStr()); }

            // checking for more requests
            req_str = sock->getNextRequest(client_fd);
        }
    }
    catch (std::exception const& e) {
        LOG_E(e.what());
        return (-1);
    }
    return (0);
}

smt::shared_ptr<HTTPResponse>
    process_request(smt::shared_ptr<HTTPRequest> request,
                    smt::shared_ptr<ServerBlock> config, int client_fd) {

    smt::shared_ptr<webserv::LocationBlock> loc =
        config->getLocationBlockForRequest(request);

    bool runCGI = (loc) && (loc->m_cgi_enabled) && (loc->m_cgi->isValid());
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

    std::string                        body;
    std::map<std::string, std::string> headers;

    bool default_body = false;
    if (config->m_error_page.find(code) != config->m_error_page.end()) {
        // getting custom body and headers

        std::ifstream file(config->m_error_page[code].c_str());
        if (!file.good()) {

            LOG_I("generate_error_response(): failed to open " +
                  config->m_error_page[code]);
            default_body = true;
        }
        else {
            body = std::string(std::istreambuf_iterator<char>(file),
                               std::istreambuf_iterator<char>());
        }
        file.close();
    }
    else { default_body = true; }

    if (default_body) { // getting default body and headers

        // converting code to str
        std::stringstream ssa;
        ssa << code;
        std::string status = ssa.str();
        std::string msg = status + " - " + HTTPResponse::s_status_map[code];

        // generating body
        body = "<!DOCTYPE html>"
               "<html>"
               "  <head>"
               "    <title>" +
               msg +
               "</title>"
               "  </head>"
               "  <body>"
               "    <h1>" +
               msg +
               "</h1>"
               "  </body>"
               "</html>";
    }

    // getting body length in str
    std::stringstream ssb;
    ssb << body.size();
    std::string body_size = ssb.str();

    // generating necessary headers
    headers["Content-Length"] = body_size;
    headers["Content-Type"] = "text/html";

    return (
        smt::shared_ptr<HTTPResponse>(new HTTPResponse(code, headers, body)));
}

} // namespace webserv
