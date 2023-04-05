#ifndef HTTP_RESPONSE_HPP
#define HTTP_RESPONSE_HPP

#include <map>
#include <sstream>
#include <string>

namespace webserv {

class HTTPResponse {

    public:

        HTTPResponse(int code, std::map<std::string, std::string> headers,
                     std::string body = "", std::string version = "HTTP/1.1");
        ~HTTPResponse(void);

        std::string toStr(void);

        void setStatus(int status);
        int  getStatus(void) const;

        void        setHeader(std::string key, std::string value);
        void        setHeaders(std::map<std::string, std::string> headers);
        std::string getHeader(std::string key) const;
        std::map<std::string, std::string> getHeaders(void) const;

        void        setContent(std::string content);
        std::string getContent(void) const;

        void        setVersion(std::string version);
        std::string getVersion(void) const;

        void        setReason(std::string reason);
        std::string getReason(void) const;

        static std::map<int, std::string> s_statusMap;
        static std::map<int, std::string> createStatusMap(void);

    private:

        int                                m_status;
        std::map<std::string, std::string> m_headers;
        std::string                        m_content;
        std::string                        m_version;
        std::string                        m_reason;
};

} // namespace webserv

#endif /* HTTP_RESPONSE_HPP */
