#include "server/Blocks.hpp"

namespace webserv {

/* ServerBlock */
ServerBlock::ServerBlock(void)
    : m_autoidx(true), m_bsize(BODY_SIZE), m_port(80), m_host("localhost"),
      m_root(D_ROOT_PATH) {}

ServerBlock::~ServerBlock(void) {}

/* LocationBlock */
LocationBlock::LocationBlock(char const* uri) : m_uri(uri) {

    std::string arr[3] = D_METHODS;
    m_req_methods = std::vector<std::string>(arr, arr + 3);
}

LocationBlock::~LocationBlock(void) {}

} // namespace webserv
