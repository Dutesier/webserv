#ifndef BLOCKS_HPP
#define BLOCKS_HPP

#include "utils/smt.hpp"
#include "utils/utils.hpp"

#include <cstring>
#include <map>
#include <sstream>
#include <vector>

#define MAX_BODY_SIZE 2097152 // 2MB
#define BODY_SIZE     1048576 // 1MB

namespace webserv {

struct LocationBlock;

struct ServerBlock {

        ServerBlock(void);
        ~ServerBlock(void);

        bool        m_autoidx;
        unsigned    m_bsize;
        unsigned    m_port;
        std::string m_host;
        std::string m_root;

        std::vector<std::string>   m_idx;
        std::vector<std::string>   m_srv_name;
        std::map<int, std::string> m_error_page;

        std::vector<smt::shared_ptr<LocationBlock> > m_location;
};

struct LocationBlock {

        LocationBlock(char const* uri = "/");
        ~LocationBlock(void);

        std::string              m_uri;
        std::string              m_root;
        std::string              m_cgi;
        std::vector<std::string> m_req_methods;
};

} // namespace webserv

#endif /* BLOCKS_HPP */
