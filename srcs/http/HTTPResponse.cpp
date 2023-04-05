#include "http/HTTPResponse.hpp"

namespace webserv {

std::map<int, std::string> HTTPResponse::s_statusMap =
    HTTPResponse::createStatusMap();

HTTPResponse::HTTPResponse(int                                status,
                           std::map<std::string, std::string> headers,
                           std::string body, std::string version)
    : m_status(status), m_headers(headers), m_content(body), m_version(version),
      m_reason(HTTPResponse::s_statusMap[m_status]) {}

HTTPResponse::~HTTPResponse(void) {}

std::string HTTPResponse::toStr(void) {

    std::string sp(" ");
    std::string crlf("\r\n");

    // converting m_status to string
    std::stringstream ss;
    ss << m_status;
    std::string status = ss.str();

    // converting m_headers to string
    std::string header;
    for (std::map<std::string, std::string>::iterator it = m_headers.begin();
         it != m_headers.end(); it++) {
        header += (*it).first + ": " + (*it).second + crlf;
    }
    return (m_version + sp + status + sp + m_reason + crlf + header + crlf +
            m_content);
}

std::map<int, std::string> HTTPResponse::createStatusMap(void) {
    std::map<int, std::string> code_map;

    code_map[100] = "Continue";
    code_map[101] = "Switching Protocols";
    code_map[102] = "Processing";
    code_map[103] = "Early Hints";
    code_map[200] = "OK";
    code_map[201] = "Created";
    code_map[202] = "Accepted";
    code_map[203] = "Non Authoritative Info";
    code_map[204] = "No Content";
    code_map[205] = "Reset Content";
    code_map[206] = "Partial Content";
    code_map[207] = "Multi Status";
    code_map[208] = "Already Reported";
    code_map[226] = "IM Used";
    code_map[300] = "Multiple Choices";
    code_map[301] = "Moved Permanently";
    code_map[302] = "Found";
    code_map[303] = "See Other";
    code_map[304] = "Not Modified";
    code_map[305] = "Use Proxy";
    code_map[307] = "Temporary Redirect";
    code_map[308] = "Permanent Redirect";
    code_map[400] = "Bad Request";
    code_map[401] = "Unauthorized";
    code_map[402] = "Payment Required";
    code_map[403] = "Forbidden";
    code_map[404] = "Not Found";
    code_map[405] = "Method Not Allowed";
    code_map[406] = "Not Acceptable";
    code_map[407] = "Proxy Auth Required";
    code_map[408] = "Request Timeout";
    code_map[409] = "Conflict";
    code_map[410] = "Gone";
    code_map[411] = "Length Required";
    code_map[412] = "Precondition Failed";
    code_map[413] = "Request Entity Too Large";
    code_map[414] = "Request URI Too Long";
    code_map[415] = "Unsupported Media Type";
    code_map[416] = "Requested Range Not Satisfiable";
    code_map[417] = "Expectation Failed";
    code_map[418] = "Teapot";
    code_map[421] = "Misdirected Request";
    code_map[422] = "Unprocessable Entity";
    code_map[423] = "Locked";
    code_map[424] = "Failed Dependency";
    code_map[425] = "Too Early";
    code_map[426] = "Upgrade Required";
    code_map[428] = "Precondition Required";
    code_map[429] = "Too Many Requests";
    code_map[431] = "Request Header Fields Too Large";
    code_map[451] = "Unavailable For Legal Reasons";
    code_map[500] = "Internal Server Error";
    code_map[501] = "Not Implemented";
    code_map[502] = "Bad Gateway";
    code_map[503] = "Service Unavailable";
    code_map[504] = "GatewayTimeout";
    code_map[505] = "HTTP Version Not Supported";
    code_map[506] = "Variant Also Negotiates";
    code_map[507] = "Insufficient Storage";
    code_map[508] = "Loop Detected";
    code_map[510] = "Not Extended";
    code_map[511] = "Network Authentication Required";
    return code_map;
}

} // namespace webserv
