#include "http/HTTPHandler.hpp"

namespace webserv {

/* HTTPHandler Class */
void http_handle(smt::shared_ptr<ServerSocket> sock, int client_fd) {

    // receiving request string
    std::string req_str = sock->recv(client_fd);

    // getting the first request from string
    HTTPParser                   parser;
    smt::shared_ptr<HTTPRequest> request = parser.getNextRequest(req_str);
    bool                         has_next = true;
    while (has_next) {

        // handle request here
        smt::shared_ptr<HTTPResponse> response =
            process_request(request, sock->m_config);

        sock->send(client_fd, response->to_str()); // sending response to client
		LOG(log_handler(request, response));
		FLOG(log_handler(request, response));

        // checking if there are more requests to handle
        request = parser.getNextRequest("");
        if (!request->isValid()) { has_next = false; }
    }
}

smt::shared_ptr<HTTPResponse>
    process_request(smt::shared_ptr<HTTPRequest> request,
                    smt::shared_ptr<ServerBlock> config) {

    // TODO: cgi is here

    // getting method
    if (request->getMethod() == GET) { return (http_get(request, config)); }
    if (request->getMethod() == POST) { return (http_post(request, config)); }
    if (request->getMethod() == DELETE) {
        return (http_delete(request, config));
    }

    return (generate_error_response(405, config));
}

smt::shared_ptr<HTTPResponse>
    generate_error_response(int code, smt::shared_ptr<ServerBlock> config) {

    std::string                        body;
    std::map<std::string, std::string> header;
    std::stringstream                  ss;

    // getting custom body
    bool default_body = false;
    if (config->m_error_page.find(code) != config->m_error_page.end()) {
        std::ifstream file(config->m_error_page[code]);
        if (!file.good()) {
            FLOG_I("Fail to open " + config->m_error_page[code]);
            default_body = true;
        }
        else {
            body = std::string((std::istreambuf_iterator<char>(file)),
                               std::istreambuf_iterator<char>());
        }
        file.close();
        // if something goes wrong
        // default_body = true;
    }
    else { default_body = true; }

    // getting default body
    if (default_body) {

        // converting code to str
        ss << code;
        std::string error_code = ss.str();

        // generating message
        std::string msg = error_code + " - " + HTTPResponse::s_status_map[code];
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

    // converting body.size() to str
    ss.str("");
    ss << body.size();
    // setting headers
    header["Content-Length"] = ss.str();

    return (
        smt::shared_ptr<HTTPResponse>(new HTTPResponse(code, header, body)));
}

smt::shared_ptr<HTTPResponse> http_get(smt::shared_ptr<HTTPRequest> request,
                                       smt::shared_ptr<ServerBlock> config) {

    // checking if get is allowed

    // creating body of the response
    std::string body;
    Uri         uri(request->getResource());
    std::string filename = uri.path_w_root(config->m_root);

    DIR* dir = opendir(filename.c_str());
    if (dir && errno != ENOENT) {

        // handling dir request
        if (!config->m_autoidx) {
            return (generate_error_response(404, config));
        }

        body = "<!DOCTYPE html>\n"
               "<html>\n"
               "<head>\n"
               "\t<title> index.html </title>\n"
               "</head>\n"
               "<body>\n"
               "\t<h1> index: </h1>\n"
               "\t<ul>\n";

        dirent*                  diread;
        std::vector<std::string> filenames;
        while ((diread = readdir(dir))) {
			// TODO: maybe here we need to check what we want to add.
			// things like '..' and '.' maybe dont make sense to have
            body += "\t\t<li><a href=" + std::string(diread->d_name) + "> " +
                    std::string(diread->d_name) + "</a></li>\n";
        }
        closedir(dir);

        body += "\t</ul>\n"
                "</body>\n"
                "</html>";
    }
    else {

        // handling file request
        std::ifstream file(filename.c_str());
        if (!file.good()) { return (generate_error_response(404, config)); }
        body = std::string(std::istreambuf_iterator<char>(file),
                           std::istreambuf_iterator<char>());
        file.close();
    }
    errno = 0;

    // creating headers of the response
    std::map<std::string, std::string> headers;
    // converting body.size() to str
    std::stringstream ss;
    ss << body.size();
    // setting headers
    headers["Content-Length"] = ss.str();

    // getting Content-Type header
    // headers["Content-Type"] = ;

	HTTPResponse* dontUse = new HTTPResponse(200, headers, body);
    return (smt::shared_ptr<HTTPResponse>(dontUse));
}

smt::shared_ptr<HTTPResponse> http_post(smt::shared_ptr<HTTPRequest> request,
                                        smt::shared_ptr<ServerBlock> config) {
    (void)request;
    return (generate_error_response(201, config));
}

smt::shared_ptr<HTTPResponse> http_delete(smt::shared_ptr<HTTPRequest> request,
                                          smt::shared_ptr<ServerBlock> config) {
    (void)request;
    return (generate_error_response(202, config));
}

std::string log_handler(smt::shared_ptr<HTTPRequest> request, smt::shared_ptr<HTTPResponse> response) {

	std::string method = (request->getMethod() == GET ? "GET" : (request->getMethod() == POST ? "POST" : "DELETE"));
	std::stringstream ss;
	ss << response->m_status;
	return (method + " " + request->getResource() + " " + request->getVersion() + " " + ss.str() + "\n" + response->m_header["Content-Length"]);
}

} // namespace webserv
