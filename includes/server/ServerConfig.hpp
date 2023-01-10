#ifndef SERVER_CONFIG_HPP
#define SERVER_CONFIG_HPP

#include <map>
#include <string>
#include <vector>

namespace webserv {

struct LocationConfig;

struct ServerConfig {

        /* Constructor and Destructor */
        ServerConfig(void);
        ~ServerConfig(void);

        /* Attributes */
        bool        autoindex;
        unsigned    body_size;
        unsigned    port;
        std::string host;
        std::string root;

        std::vector<LocationConfig*> location;
        std::vector<std::string>     index;
        std::vector<std::string>     server_name;
        std::map<int, std::string>   error_page;
};

class LocationConfig {
    public:

        /* Constructor and Destructor */
        LocationConfig(std::string line);
        ~LocationConfig(void);

        /* Attributes */
        std::string              uri;
        std::string              root;
        std::string              fastcgi_pass;
        std::vector<std::string> request_method;
};

} // namespace webserv

#endif /* SERVER_CONFIG_HPP */
