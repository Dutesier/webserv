#include "HTTPResponse.hpp"

namespace webserv {

HTTPResponse::HTTPResponse(status_t status, HTTPHeader header, std::string body,
                           std::string version)
    : status(status), header(header), body(body), version(version) {}

HTTPResponse::~HTTPResponse(void) {}

std::string HTTPResponse::to_str(void) const {

    // status line
    std::stringstream ss;
    ss << this->status.first;
    std::string code = ss.str();
    std::string start_line =
        std::string(this->version + SP + code + SP + this->status.second);

    // header
    std::string h = "";
    for (HTTPHeader::const_iterator it = this->header.begin();
         it != this->header.end(); it++)
        h += std::string((*it).first + ": " + (*it).second + CRLF);

    return (std::string(start_line + CRLF + h +
                        (this->body.empty() ? (CRLF) : (CRLF + body))));

    // return (std::string(start_line /* + CRLF + header.to_str() */ +
    //                     (this->body.empty() ? (CRLF + CRLF) : (CRLF + CRLF +
    //                     this->body))));
}

} // namespace webserv
