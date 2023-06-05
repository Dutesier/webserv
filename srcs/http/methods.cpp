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

    // Check if the method is allowed
    if (opts->m_allowed_methods.find("GET") == opts->m_allowed_methods.end()) {
        return (generateErrorResponse(405, opts));
    }

    // Log the request resource route
    LOG_I("Request route: " + request->routeRequest());

    // Create a file resource from the request route
    ft::file resource(request->routeRequest());

    // Check if the path exists
    if (!resource.exists()) { return (generateErrorResponse(404, opts)); }

    // TODO: check if the file is a CGI and if so, pass it to the CGI handler

    // Check if the path is not a crawler trap
    if (resource.isCrawler()) { return (generateErrorResponse(404, opts)); }

    // Check if the path is a directory
    if (resource.isDirectory()) { return (generateErrorResponse(404, opts)); }
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

bool isCGI(ft::file& resource, smt::shared_ptr<config::Opts> const& opts) {
    std::string ext = resource.getExtension();
    return (opts->m_cgi_extension == ext);
}

} // namespace http