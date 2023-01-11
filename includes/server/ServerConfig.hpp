#ifndef SERVER_CONFIG_HPP
#define SERVER_CONFIG_HPP

#include "utils/smt.hpp"

#include <map>
#include <string>
#include <vector>

namespace webserv {

struct LocationConfig;

struct ServerConfig {

        ServerConfig(void);
        ~ServerConfig(void);

        bool        m_autoindex;
        unsigned    m_body_size;
        unsigned    m_port;
        std::string m_host;
        std::string m_root;

        std::vector<smt::shared_ptr<LocationConfig> > m_location;
        std::vector<std::string>                      m_index;
        std::vector<std::string>                      m_server_name;
        std::map<int, std::string>                    m_error_page;
};

class LocationConfig {

    public:

        LocationConfig(std::string line);
        ~LocationConfig(void);

        std::string              m_uri;
        std::string              m_root;
        std::string              m_fastcgi_pass;
        std::vector<std::string> m_request_method;
};

} // namespace webserv

#endif /* SERVER_CONFIG_HPP */
