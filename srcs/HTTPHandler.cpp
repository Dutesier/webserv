#include "HTTPHandler.hpp"

/* This is my proposal of the HTTPHandler.
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
*/

namespace webserv {

/* HTTPHandler Class */
std::string HTTPHandler::handle_request(std::string request) {

    HTTPRequest req = HTTPHandler::parse_request(request);  // Dutesier
    HTTPHandler::process_request(req);                      // mlanca-c
    HTTPResponse res = HTTPHandler::generate_response(req); // josantos
    return (res.to_str());
}

/* Converts std::string into webserv::HTTPRequest.
 * Should fail gracefully when request is not valid */
HTTPRequest HTTPHandler::parse_request(std::string request) {
	// creates an HTTPRequest from request
	// validates that request before returning it
}

/* Takes a valid HTTPRequest and processes it */
std::string HTTPHandler::process_request(HTTPRequest req) {
    // choose method - handle CGI (don't know how yet)
	// returns the body of the response
}

/* Takes a valid HTTPRequest and generates a HTTPResponse */
HTTPResponse HTTPHandler::generate_response(HTTPRequest req) {
    // based on HTTPRequest and process_request returns an HTTPResponse
}

} // namespace webserv
