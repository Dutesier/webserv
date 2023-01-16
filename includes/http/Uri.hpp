#ifndef URI_HPP
#define URI_HPP

#include <string>

namespace webserv {

struct Uri {

        Uri(std::string uri);
        ~Uri(void);

        std::string to_str(void);
        std::string path_w_root(std::string root);

        std::string m_scheme;
        std::string m_host;
        std::string m_path;
        std::string m_query;

        std::string m_root;
};

} // namespace webserv

#endif /* URI_HPP */
