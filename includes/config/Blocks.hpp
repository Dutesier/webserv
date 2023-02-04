#ifndef BLOCKS_HPP
#define BLOCKS_HPP

#include "utils/smt.hpp"
#include "utils/utils.hpp"

#include <cstring>
#include <dirent.h>
#include <fstream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

namespace webserv {

struct LocationBlock {

        LocationBlock(std::string target);
        ~LocationBlock(void);

        std::string cgi(std::vector<std::string> command);
        std::string root(std::vector<std::string> command);
        std::string allowed_methods(std::vector<std::string> command);

        struct InvalidTargetException : public std::exception {
                char const* what(void) const throw();
        };

        bool                  m_cgi;
        std::string           m_target;
        std::string           m_root;
        std::set<std::string> m_allowed_methods;
};

struct ServerBlock {

        ServerBlock(void);
        ~ServerBlock(void);

        std::string autoindex(std::vector<std::string> command);
        std::string body_size(std::vector<std::string> command);
        std::string listen(std::vector<std::string> command);
        std::string index(std::vector<std::string> command);
        std::string root(std::vector<std::string> command);
        std::string server_name(std::vector<std::string> command);
        std::string error_page(std::vector<std::string> command);

        bool                                                   m_autoindex;
        unsigned                                               m_body_size;
        unsigned                                               m_port;
        std::string                                            m_host;
        std::string                                            m_index;
        std::string                                            m_root;
        std::string                                            m_server_name;
        std::map<int, std::string>                             m_error_page;
        std::map<std::string, smt::shared_ptr<LocationBlock> > m_location;
};

} // namespace webserv

#endif /* BLOCKS_HPP */
