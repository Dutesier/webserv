#include "ServerConfig.hpp"

namespace webserv {

/* ServerConfig Class */
ServerConfig::ServerConfig(void)
    : autoindex(false), port(80), host("localhost"), root("/var/www/html"),
      body_size(8000) {
    std::string arr[2] = {"index", "index.html"};
    this->index.insert(this->index.begin(), arr, arr + 2);
}

ServerConfig::~ServerConfig(void) {
    for (std::vector<LocationConfig*>::iterator it = this->location.begin();
         it != this->location.end(); ++it)
        delete *it;
}

/* LocationConfig Class */
LocationConfig::LocationConfig(std::string uri) : uri(uri) {
    std::string arr[3] = {"GET", "POST", "DELETE"};
    this->request_method.insert(this->request_method.begin(), arr, arr + 3);
}

LocationConfig::~LocationConfig(void) {}

} // namespace webserv
