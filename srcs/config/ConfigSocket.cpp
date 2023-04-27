#include "config/ConfigSocket.hpp"

namespace webserv {

ConfigSocket::ConfigSocket(std::vector< smt::shared_ptr<ServerBlock> > blocks)
    : m_blocks(blocks) {

    // initializing m_specs
    std::vector< smt::shared_ptr<ServerBlock> >::iterator it;
    for (it = m_blocks.begin(); it != m_blocks.end(); it++) {

        smt::shared_ptr<ServerBlock> block = *it;
        m_specs.insert(
            std::make_pair(block->m_resolvPort, block->m_resolvHost));
    }
}

ConfigSocket::~ConfigSocket(void) {}

std::vector< smt::shared_ptr<ServerBlock> >
    ConfigSocket::blocks(std::pair<unsigned, std::string> specs) {

    std::vector< smt::shared_ptr<ServerBlock> > ret;

    if (m_specs.find(specs) == m_specs.end()) { throw(NoSuchSpecsException()); }

    std::vector< smt::shared_ptr<ServerBlock> >::iterator it;
    for (it = m_blocks.begin(); it != m_blocks.end(); it++) {

        smt::shared_ptr<ServerBlock> block = *it;
        if (block->m_resolvPort == specs.first &&
            block->m_resolvHost == specs.second) {
            ret.push_back(block);
        }
    }
    return (ret);
}

std::set< std::pair<unsigned, std::string> >& ConfigSocket::specs(void) {
    return (m_specs);
}

char const* ConfigSocket::NoSuchSpecsException::what(void) const throw() {
    return ("ConfigSocket: no such sockets in config file");
}

} // namespace webserv
