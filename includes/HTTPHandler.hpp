#ifndef HTTP_HANDLER_HPP
#define HTTP_HANDLER_HPP

#include "HTTPRequest.hpp"
#include "HTTPResponse.hpp"
#include "SocketConnection.hpp"
#include "utils.hpp"

namespace webserv {

class HTTPHandler {


    public:
		//HTTP response handling variables
		int									status_code;
		std::map<std::string, std::string>	headers;
		std::string							response_file;
		
		//HTTP response creation variables
		std::string	status_section;
		std::string	header_section;
		std::string	body_section;

		std::string	http_response;

		HTTPHandler();
		~HTTPHandler();


        /* Static Member Functions */
        static std::string handle_request(std::string request);


#ifndef GTEST_TESTING

    private:

#endif

        /* Static Private Member Functions */
        static std::pair<HTTPRequest, HTTPResponse>
            parse_request(std::string request);
        static std::pair<std::string, HTTPResponse>
                            process_request(HTTPRequest req);
        static HTTPResponse generate_response(HTTPRequest req,
                                              std::string body);
        static HTTPResponse generate_error_response(int         status,
                                                    std::string reason);

        // TODO: understand what each method needs and pass it directly, not
        // in HTTPRequest format
        static std::pair<std::string, HTTPResponse> get_method(HTTPRequest req);
        static std::pair<std::string, HTTPResponse>
            post_method(HTTPRequest req);
        static std::pair<std::string, HTTPResponse>
            delete_method(HTTPRequest req);
};

} // namespace webserv

#endif /* HTTP_HANDLER_HPP */
