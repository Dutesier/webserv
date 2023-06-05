#include "http/Handler.hpp"

#include "cgi/CgiHandler.hpp"
#include "config/Options.hpp"
#include "http/methods.hpp"
#include "http/MimeType.hpp"
#include "http/Request.hpp"
#include "http/Response.hpp"
#include "utils/Logger.hpp"
#include "utils/Route.hpp"

namespace http {

smt::shared_ptr<Response>
    processRequest(int status, smt::shared_ptr<Request> const request,
                   smt::shared_ptr<config::Opts> const opts) {
    if (status) { return (generateErrorResponse(status, opts)); }

    // check if method is allowed
    if (opts->m_allowed_methods.find(request->getMethod()) ==
        opts->m_allowed_methods.end()) {
        LOG_I("Method not allowed");
        return (generateErrorResponse(405, opts));
    }

    // TODO: handle getNextRequest with max body size and a lot of requests at
    // the same time.

    http::Route route(opts->m_target, opts->m_root);
    request->setRoute(route);

    // TODO: check if request is cgi request
    if (opts->m_cgi_extension == request->getPath().getExtension()) {
        LOG_I("Running Cgi in " << request->routeRequest());
        std::string resp = cgi::runCgiScript(request, opts);
        return (smt::make_shared(new Response(resp)));
    }

    // getting method
    MethodType method = convertMethod(request->getMethod());
    switch (method) {
        case GET: return (methods::GET(request, opts));
        case POST: return (methods::POST(request, opts));
        case DELETE: return (methods::DELETE(request, opts));
        default: return (generateErrorResponse(501, opts));
    }
}

smt::shared_ptr<Response>
    generateErrorResponse(int code, smt::shared_ptr<config::Opts> const opts) {

    // TODO: protect this when opts is null
    std::string                        body;
    std::map<std::string, std::string> header;
    std::stringstream                  ss;

    // getting custom body
    bool default_body = false;
    if (opts->m_error_pages.find(code) != opts->m_error_pages.end()) {
        std::string   filename = std::string(opts->m_error_pages[code]);
        std::ifstream file(filename.c_str());
        if (!file.good()) {
            LOG_I("Fail to open " << filename);
            default_body = true;
        }
        else {
            body = std::string((std::istreambuf_iterator<char>(file)),
                               std::istreambuf_iterator<char>());
            header["Content-Type"] = http::MimeType(filename);
        }
        file.close();
    }
    else { default_body = true; }

    // getting default body
    if (default_body) {

        // converting code to str
        ss << code;
        std::string error_code = ss.str();

        // generating message
        std::string msg =
            error_code + " - " + std::string(http::ErrorCode(code));
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
        header["Content-Type"] = "text/html";
    }

    return (smt::shared_ptr<Response>(new Response(code, header, body)));
}

} // namespace http