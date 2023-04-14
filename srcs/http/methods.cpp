#include "http/methods.hpp"

#include "http/pathValidation.hpp"

#include <sstream>

namespace webserv::methods
{

smt::shared_ptr<HTTPResponse>
GET(smt::shared_ptr<HTTPRequest>& request,
                    smt::shared_ptr<LocationBlock>& location) {
    LOG_D("GET method called");
    std::string                        body;
    std::map<std::string, std::string> headers;
    smt::shared_ptr<webserv::HTTPResponse> resp;
    resp = smt::make_shared<webserv::HTTPResponse>(
        new webserv::HTTPResponse(200, headers, body));

    // Check if method is allowed
    bool allowed = location->m_allowed_methods.find("GET") != location->m_allowed_methods.end();
    if (!allowed) {
        resp->m_status = 403; // Need to check this
        LOG_D("Returning error");
        return resp;
    }

    // Check if file in path and access is allowed (This logic is common to CGI)
    std::string refined = request->getRefinedResource();
    std::string directory = location->m_root; // FIXME: Shouldn't we have different roots for different methods

    LOG_D("Checking requested resource: " + directory+refined);
    // If so, read file into buffer.
    if (webserv::path::fileExists(directory, refined) && !webserv::path::isCrawler(refined)) {

        std::string filepath = directory + refined;
        std::ifstream file(filepath.c_str());
        std::string line;
        std::stringstream ss;
        std::string str = ss.str();
        if (file.good()) {
            while(std::getline(file, line)) {
                resp->m_body += line;
            }
            ss << resp->m_body.size();
            resp->m_header["Content-Length:"] = ss.str();
            return resp;
        } else {
            resp->m_status = 500;
            return resp;
        }
    } else {
        resp->m_status = 403; // Need to check this
        return resp;
    }

    // Write buffer to HTTPResponse
    // Return HTTPResponse
}

// smt::shared_ptr<HTTPResponse>
// POST(smt::shared_ptr<HTTPRequest>& request,
//                     smt::shared_ptr<LocationBlock>& location) {
//     // Check if method is allowed
//     bool allowed = location->m_allowed_methods.find("POST") != location->m_allowed_methods.end();

// }

// smt::shared_ptr<HTTPResponse>
// HEAD(smt::shared_ptr<HTTPRequest>& request,
//                     smt::shared_ptr<LocationBlock>& location) {
//     // Check if method is allowed
//     bool allowed = location->m_allowed_methods.find("HEAD") != location->m_allowed_methods.end();

// }

// smt::shared_ptr<HTTPResponse>
// DELETE(smt::shared_ptr<HTTPRequest>& request,
//                     smt::shared_ptr<LocationBlock>& location) {
//     // Check if method is allowed
//     bool allowed = location->m_allowed_methods.find("DELETE") != location->m_allowed_methods.end();

// }

} // namespace webserv::methods