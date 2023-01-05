#ifndef HTTP_HANDLER_HPP
#define HTTP_HANDLER_HPP

#include "HTTPRequest.hpp"
#include "HTTPResponse.hpp"
#include "SocketConnection.hpp"

namespace webserv {

class HTTPHandler {

    public:

        /* Static Member Functions */
        static std::string handle_request(std::string request);

#ifndef GTEST_TESTING

    private:

#endif

        /* Static Private Member Functions */
        static HTTPRequest  parse_request(std::string request);
        // TODO: return type could be changed to string - body of response
        static std::string  process_request(HTTPRequest req);
        static HTTPResponse generate_response(HTTPRequest req);

        /* Exceptions */
};

} // namespace webserv

#endif /* HTTP_HANDLER_HPP */
