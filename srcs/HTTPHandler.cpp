#include "HTTPHandler.hpp"

/*
 * This is my proposal of the HTTPHandler.
 *
 * This is an idea of how we could join our separate jobs.
 * First @dutesier is handling the parsing of the HTTPRequest class. Thats the
 * first step of the Handler, generating and HTTPRequest object from a string.
 * I think we should go even more further as to also validate said Request
 * inside the Hander::parse_request function and to create some sort of
 * exception mechanism to handle HTTP erros.
 *
 * Then I assigned myself, @mlanca-c the process_request method of the Handler.
 * This method will receive the already parsed, and valid Request, and it will
 * process it accordingly. It will focus on the three mandatory methods GET,
 * POST, and DELETE, as well as the CGI part of the project.
 * In this function the same logic of erros and exceptions should be applied.
 *
 * Finally @J0Santos got the job of generate_response methods of the Handler.
 * This method will, based on the previous methods returns, generate a
 * HTTPResponse. From what I can understand, its possible to generate this
 * response outside of its processing if the error and exception management is
 * correctly performed and if a body is the result of the process_request
 * method.
 *
 * Let me know what you think of this and if it fits with what youve been doing
 * so far.
 *
 * UPDATE:
 *
 * Found a solution to the error management. We're not using any
 * exceptions, we're using std::pair.
 *
 * During the execution of parse_request() and process_request(), in case an
 * HTTP error occurs, second element of the return type of each function
 * - HTTPResponse - is initialized by calling the function
 * generate_error_response().
 * If either function returns an uninitialize HTTPResponse ( pair.second ),
 * handle_request() continues its proccess, otherwise, it returns the
 * HTTPResponse corresponding to the error.
 */

namespace webserv {

HTTPHandler::HTTPHandler(): status_code(-1) {

}

/* HTTPHandler Class */
std::string HTTPHandler::handle_request(std::string request) {

    std::pair<HTTPRequest, HTTPResponse> req =
        HTTPHandler::parse_request(request); // Dutesier
    if (req.second.init == true) { return (req.second.to_str()); } // checking for errors

    std::pair<std::string, HTTPResponse> body =
        HTTPHandler::process_request(req.first); // mlanca-c
    if (body.second.init == true) { return (body.second.to_str()); }   // checking for errors

    HTTPResponse res =
        HTTPHandler::generate_response(req.first, body.first); // J0Santos

    return (res.to_str());
}

/* Converts std::string into webserv::HTTPRequest.
 * Should fail gracefully when request is not valid */
std::pair<HTTPRequest, HTTPResponse>
    HTTPHandler::parse_request(std::string request) {
    // creates an HTTPRequest from request
    // validates that request before returning it


/* HTTPHandler::impl Class */
// HTTPHandler::impl::impl(TCPSocket* socket, int fd)
//     : socket(socket), client(this->socket->connection(fd)) {

//     smt::shared_ptr<HTTPRequest> req = this->client->recv();
//     if (req != NULL) {
//         std::cout << req->getContent() << std::endl;
//         // std::cout << *req << std::endl;
//     }
//     this->client->send("A string");
}

/* Takes a valid HTTPRequest and processes it */
std::pair<std::string, HTTPResponse>
    HTTPHandler::process_request(HTTPRequest req) {

    // choose method - handle CGI (don't know how yet)
    // returns the body of the response
    typedef std::pair<std::string, HTTPResponse> result_type;

    switch (req.getMethod()) {
        // case GET:
        // 	return (get_method(req));
        // case POST:
        // 	return (post_method(req));
        // case DELETE:
        // 	return (delete_method(req));
        default:
            return (
                result_type(ft::nullptr_t, HTTPHandler::generate_error_response(
                                               405)));
    }
}

/* Takes a valid HTTPRequest and generates a HTTPResponse */
HTTPResponse HTTPHandler::generate_response(HTTPRequest req, std::string body) {
    // based on HTTPRequest and process_request returns an HTTPResponse
}

/* Creates a HTTPResponse based on code, and reason - error response */
HTTPResponse HTTPHandler::generate_error_response(int code) {
	return (HTTPResponse(code));
}

/* to get method functions started, I need to study Fast CGI and the methods
 * themselves */
std::pair<std::string, HTTPResponse> HTTPHandler::get_method(HTTPRequest req) {}

std::pair<std::string, HTTPResponse> HTTPHandler::post_method(HTTPRequest req) {
}

std::pair<std::string, HTTPResponse>
    HTTPHandler::delete_method(HTTPRequest req) {}

} // namespace webserv
