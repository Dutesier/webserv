#include "http/HTTPHandler.hpp"

namespace webserv {

/* HTTPHandler Class */
// TODO: make sure there are no more requests in GetNextRequest
int httpHandle(smt::shared_ptr<ServerSocket> sock, int connectionFd) {

    try {
        // receiving request string
        std::string msg = sock->recv(connectionFd);
        if (msg.empty()) { return (0); }
        FLOG_D("Received " + msg + " from client");

        // getting first request from string (there can be more than one
        // request, or request can be incomplete)
        std::string reqStr = sock->getNextRequest(connectionFd, msg);
        // if request is incomplete, return 1
        if (reqStr.empty()) { return (1); }

        // handling all requests
        while (!reqStr.empty()) {

            FLOG_D("Received a full request from client");
            // creating request object
            smt::shared_ptr<HTTPRequest> req(new HTTPRequest(reqStr));
            // getting the correct configuration block according to request
            smt::shared_ptr<ServerBlock> config_block =
                sock->getConfigFromRequest(req);
            // generating a response
            smt::shared_ptr<HTTPResponse> resp =
                processRequest(req, config_block, connectionFd);
            FLOG_D("sending response " + resp->toStr() + "to client");
            // sending response to client
            if (resp) { sock->send(connectionFd, resp->toStr()); }

            // checking for more requests
            reqStr = sock->getNextRequest(connectionFd);
        }
    }
    catch (std::exception const& e) {
        LOG_E(e.what());
        return (-1);
    }
    return (0);
}

smt::shared_ptr<HTTPResponse>
    processRequest(smt::shared_ptr<HTTPRequest> request,
                   smt::shared_ptr<ServerBlock> config, int connectionFd) {

    smt::shared_ptr<webserv::LocationBlock> loc =
        config->getLocationBlockForRequest(request);

    bool runCGI = (loc) && (loc->m_cgiEnabled) && (loc->m_cgi->isValid());
    bool isCGI = request->isCGIRequest();
    if (isCGI && runCGI) {
        LOG_D("Running CGI script");
        return (loc->m_cgi->run(request, connectionFd));
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

    return (generateErrorResponse(405, config));
}

smt::shared_ptr<HTTPResponse>
    generateErrorResponse(int code, smt::shared_ptr<ServerBlock> config) {

    std::string                        body;
    std::map<std::string, std::string> headers;

    bool default_body = false;
    if (config->m_errorPage.find(code) != config->m_errorPage.end()) {
        // getting custom body and headers

        std::ifstream file(config->m_errorPage[code].c_str());
        if (!file.good()) {

            LOG_I("generateErrorResponse(): failed to open " +
                  config->m_errorPage[code]);
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
        std::string msg = status + " - " + HTTPResponse::s_statusMap[code];

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
