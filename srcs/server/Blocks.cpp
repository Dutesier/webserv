#include "server/Blocks.hpp"

namespace webserv {

/* ServerBlock */
ServerBlock::ServerBlock(void)
    : m_autoidx(false), m_bsize(BODY_SIZE), m_port(80), m_host("localhost"),
      m_root(utils::d_root()), m_idx(utils::index_pages()) {}

ServerBlock::~ServerBlock(void) {}

/* LocationBlock */
LocationBlock::LocationBlock(char const* uri)
    : m_uri(uri), m_req_methods(utils::allowed_methods()) {}

LocationBlock::~LocationBlock(void) {}

} // namespace server
