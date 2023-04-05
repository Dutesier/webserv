#ifndef HTTP_RESPONSE_HPP
#define HTTP_RESPONSE_HPP

#include <map>
#include <sstream>
#include <string>

namespace webserv {

struct HTTPResponse {

        HTTPResponse(int code, std::map<std::string, std::string> header,
                     std::string body = "", std::string version = "HTTP/1.1");
        ~HTTPResponse(void);

        std::string to_str(void);

        int                                m_status;
        std::map<std::string, std::string> m_header;
        std::string                        m_body;
        std::string                        m_version;
        std::string                        m_reason;

        static std::map<int, std::string> s_status_map;
        static std::map<int, std::string> create_status_map(void);
};
} // namespace webserv

#endif /* HTTP_RESPONSE_HPP */
