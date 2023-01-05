#include "HTTPResponse.hpp"

namespace webserv {

HTTPResponse::HTTPResponse(status_t status, HTTPHeader header, std::string body,
                           std::string version)
    : status(status), header(header), body(body), version(version) {}

HTTPResponse::~HTTPResponse(void) {}

std::string HTTPResponse::to_str(void) const {

    std::stringstream ss;
    ss << this->status.first;
    std::string code = ss.str();

    std::string start_line =
        std::string(this->version + SP + code + SP + this->status.second);
    return (std::string(start_line /* + CRLF + header.to_str() */ +
                        (body.empty() ? (CRLF + CRLF) : (CRLF + CRLF + body))));
}

} // namespace webserv
