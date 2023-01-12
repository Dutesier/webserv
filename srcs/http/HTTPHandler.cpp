#include "http/HTTPHandler.hpp"

namespace webserv {

/* HTTPHandler Class */
void http_handle(smt::shared_ptr<ServerSocket> sock, int client_fd) {

    // receiving request string
    std::string req_str = sock->recv(client_fd);

    // getting the first request from string
	HTTPParser parser;
    smt::shared_ptr<HTTPRequest> request = parser.getNextRequest(req_str);
    bool has_next = true;
    while (has_next) {

        FLOG("Handling request...");
        // handle request here
		smt::shared_ptr<HTTPResponse> response = process_request(request, sock->m_config);

        sock->send(client_fd, response->to_str()); // sending response to client

        // checking if there are more requests to handle
		request = parser.getNextRequest("");
		if (!request->isValid()) { has_next = false; }
    }
}

smt::shared_ptr<HTTPResponse> process_request(smt::shared_ptr<HTTPRequest> request, smt::shared_ptr<ServerBlock> config) {

	(void)request;

	// getting method
	// if (request->getMethod == "GET") { return (Method::get(request)); }
	// if (request->getMethod == "POST") { return (Method::post(request)); }
	// if (request->getMethod == "DELETE") { return (Method::delete(request)); }

	return (generate_error_response(405, config));
}

smt::shared_ptr<HTTPResponse> generate_error_response(int code, smt::shared_ptr<ServerBlock> config) {

	std::string body;
	std::map<std::string, std::string> header;

	return (smt::shared_ptr<HTTPResponse>(new HTTPResponse(code, header, body)));
}

} // namespace webserv
