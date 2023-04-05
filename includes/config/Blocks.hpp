#ifndef BLOCKS_HPP
#define BLOCKS_HPP

#include "cgi/CGIHandler.hpp"
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
        std::string allowedMethods(std::vector<std::string> command);

        struct InvalidTargetException : public std::exception {
                char const* what(void) const throw();
        };

        bool                             m_cgiEnabled;
        smt::shared_ptr<cgi::CGIHandler> m_cgi;
        std::string                      m_target;
        std::string                      m_root;
        std::set<std::string>            m_allowedMethods;
};

struct ServerBlock {

        ServerBlock(void);
        ~ServerBlock(void);

        std::string autoindex(std::vector<std::string> command);
        std::string bodySize(std::vector<std::string> command);
        std::string listen(std::vector<std::string> command);
        std::string index(std::vector<std::string> command);
        std::string root(std::vector<std::string> command);
        std::string serverName(std::vector<std::string> command);
        std::string errorPage(std::vector<std::string> command);
        smt::shared_ptr<LocationBlock>
            getLocationBlockForRequest(smt::shared_ptr<HTTPRequest>& request);

        bool                                                   m_autoindex;
        unsigned                                               m_bodySize;
        unsigned                                               m_port;
        std::string                                            m_host;
        std::string                                            m_index;
        std::string                                            m_root;
        std::string                                            m_serverName;
        std::map<int, std::string>                             m_errorPage;
        std::map<std::string, smt::shared_ptr<LocationBlock> > m_location;
};

} // namespace webserv

#endif /* BLOCKS_HPP */
