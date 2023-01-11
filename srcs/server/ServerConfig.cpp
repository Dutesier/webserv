#include "server/ServerConfig.hpp"

namespace webserv {

/* ServerConfig Class */
ServerConfig::ServerConfig(void)
    : m_autoindex(false), m_port(80), m_host("localhost"),
      m_root("/var/www/html"), m_body_size(8000) {

    std::string arr[2] = {"index", "index.html"};
    m_index.insert(m_index.begin(), arr, arr + 2);
}

ServerConfig::~ServerConfig(void) {}

/* LocationConfig Class */
LocationConfig::LocationConfig(std::string uri) : m_uri(uri) {

    std::string arr[3] = {"GET", "POST", "DELETE"};
    m_request_method.insert(m_request_method.begin(), arr, arr + 3);
}

LocationConfig::~LocationConfig(void) {}

} // namespace webserv
