#include "http/HTTPHandler.hpp"

namespace webserv {

/* HTTPHandler Class */
void http_handle(smt::shared_ptr<ServerSocket> sock, int client_fd) {
    LOG("This is not done (yet)");
}

/* this is how http_handle will look like
void http_handle(TCPSocket* sock, int client_fd) {

    // receiving request string
    std::string req_str = sock->recv(client_fd);

    // getting the first request from string
    smt::shared_ptr<HTTPRequest> request = sock->get_next_request(req_str);
    bool has_next = true;
    while (has_next) {

        FLOG("Handling request...");
        // handle request here
        smt::shared_ptr<HTTPResponse> response;

        // int         -> status code
        // std::string -> body of response
        std::pair<int, std::string> resp = process_request(request);
        response = generate_response(reps); // generating response

        sock->send(client_fd, response.to_str()); // sending response to client

        // checking if there are more requests to handle
        if (!(request = get_next_request(NULL))) { has_next = false; }
    }
}
*/

} // namespace webserv
