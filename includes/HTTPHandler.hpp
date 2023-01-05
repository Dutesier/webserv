#ifndef HTTP_HANDLER_HPP
#define HTTP_HANDLER_HPP

#include "HTTPRequest.hpp"
#include "HTTPResponse.hpp"
#include "SocketConnection.hpp"
#include "utils.hpp"

namespace webserv {

class HTTPHandler {

    public:

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
        static HTTPResponse generate_response(HTTPRequest req);
};

} // namespace webserv

#endif /* HTTP_HANDLER_HPP */
