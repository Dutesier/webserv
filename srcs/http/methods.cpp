#include "http/methods.hpp"

#include "config/Options.hpp"
#include "http/Handler.hpp"
#include "http/Request.hpp"
#include "http/Response.hpp"
#include "utils/smt.hpp"

namespace http {
namespace methods {

smt::shared_ptr<http::Response>
    GET(smt::shared_ptr<http::Request> const request,
        smt::shared_ptr<config::Opts> const  opts) {

    // Initialize a response
    std::string                        body;
    std::map<std::string, std::string> headers;
    smt::shared_ptr<http::Response>    response;
    response = smt::make_shared<webserv::HTTPResponse>(
        new http::Response(200, headers, body));
    (void)request;
    return (generateErrorResponse(501, opts));
}

smt::shared_ptr<http::Response>
    POST(smt::shared_ptr<http::Request> const request,
         smt::shared_ptr<config::Opts> const  opts) {
    (void)request;
    return (generateErrorResponse(502, opts));
}

smt::shared_ptr<http::Response>
    DELETE(smt::shared_ptr<http::Request> const request,
           smt::shared_ptr<config::Opts> const  opts) {
    (void)request;
    return (generateErrorResponse(503, opts));
}

} // namespace methods

MethodType convertMethod(std::string const& methodStr) {
    if (methodStr == "GET") { return (GET); }
    if (methodStr == "HEAD") { return (HEAD); }
    if (methodStr == "POST") { return (POST); }
    if (methodStr == "PUT") { return (PUT); }
    if (methodStr == "DELETE") { return (DELETE); }
    if (methodStr == "CONNECT") { return (CONNECT); }
    if (methodStr == "OPTIONS") { return (OPTIONS); }
    if (methodStr == "TRACE") { return (TRACE); }
    if (methodStr == "PATCH") { return (PATCH); }
    if (methodStr == "INVALID") { return (INVALID); }
    return (UNKNOWN_METHOD);
}

Version convertVersion(std::string const& versionStr) {
    if (versionStr == "HTTP/1.1") { return (HTTP_1_1); }
    return (UNKNOWN_VERSION);
}

} // namespace http