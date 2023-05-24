#include "config/ConfigSocket.hpp"

namespace webserv {

std::vector< smt::shared_ptr<ServerBlock> > ConfigSocket::m_blocks;

std::set< std::pair<int, std::string> > ConfigSocket::m_specs;

std::vector< smt::shared_ptr<ServerAddress> > ConfigSocket::m_addresses;

smt::shared_ptr<ServerAddress>
    ConfigSocket::getAddress(std::pair<int, std::string> specs) {
    std::vector< smt::shared_ptr<ServerAddress> >::iterator it;
    for (it = m_addresses.begin(); it != m_addresses.end(); it++) {
        if ((*it)->getPort() == specs.first &&
            (*it)->getHost() == specs.second) {
            return (*it);
        }
    }
    throw NoSuchAddressException();
}

std::set< std::pair<int, std::string> > ConfigSocket::getSpecs(void) {
    return (m_specs);
}

smt::shared_ptr<ServerBlock>
    ConfigSocket::getConfigBlock(int port, std::string host,
                                 std::string hostHeader) {
    std::vector< smt::shared_ptr<ServerBlock> > correspondingBlocks;
    smt::shared_ptr<ServerBlock>                block;
    static std::vector< smt::shared_ptr<ServerBlock> >::iterator it;
    for (it = m_blocks.begin(); it != m_blocks.end(); it++) {
        if ((*it)->m_resolvPort == port && (*it)->m_resolvHost == host) {
            if (!block) { block = *it; }
            else if ((*it)->m_server_name == hostHeader) { block = *it; }
        }
    }
    if (!block) { throw NoSuchBlockException(); }
    return (block);
}

smt::shared_ptr<LocationBlock>
    ConfigSocket::getLocationBlock(int port, std::string host,
                                   std::string hostHeader, std::string uri) {
    smt::shared_ptr<ServerBlock> block = getConfigBlock(port, host, hostHeader);
    return (getLocationBlock(block, uri));
}

smt::shared_ptr<LocationBlock>
    ConfigSocket::getLocationBlock(smt::shared_ptr<ServerBlock> block,
                                   std::string                  uri) {
    smt::shared_ptr<LocationBlock>                          loc;
    std::map< std::string, smt::shared_ptr<LocationBlock> > locations =
        block->m_location;

    std::vector<std::string> cmd = splitLine(uri);
    std::map< std::string, smt::shared_ptr<LocationBlock> >::iterator it;
    int oldCount = -1;
    for (it = locations.begin(); it != locations.end(); it++) {
        // figure out how to get the best location block
        std::vector<std::string> target = splitLine((*it).first);

        int count = getCountOfDirs(cmd, target);
        if (count > oldCount) {
            loc = (*it).second;
            oldCount = count;
        }
    }
    return (loc);
}

int ConfigSocket::getCountOfDirs(std::vector<std::string> cmd,
                                 std::vector<std::string> target) {
    int count = 0;
    for (size_t i = 0; i < cmd.size() && i < target.size(); i++) {
        if (cmd[i] == target[i]) { count++; }
        else { break; }
    }
    return (count);
}

std::vector<std::string> ConfigSocket::splitLine(std::string uri) {
    std::vector<std::string> cmd;

    char* word = strtok(const_cast<char*>(uri.c_str()), "/");
    while (word) {
        cmd.push_back(word);
        word = strtok(NULL, " \t");
    }
    return (cmd);
}

void ConfigSocket::setBlocks(
    std::vector< smt::shared_ptr<ServerBlock> > blocks) {

    m_blocks = blocks;
    smt::shared_ptr<ServerAddress> addr;

    std::vector< smt::shared_ptr<ServerBlock> >::iterator it;
    for (it = m_blocks.begin(); it != m_blocks.end(); it++) {
        // getting the corresponding address
        addr =
            smt::make_shared(new ServerAddress((*it)->m_port, (*it)->m_host));

        // set resolvHost and resolvPort
        (*it)->m_resolvPort = addr->getPort();
        (*it)->m_resolvHost = addr->getHost();

        // seeing if that address already exists
        std::pair<int, std::string> spec(addr->getPort(), addr->getHost());
        if (m_specs.find(spec) == m_specs.end()) {
            m_specs.insert(spec);
            m_addresses.push_back(addr);
        }
    }
}

char const* ConfigSocket::NoSuchAddressException::what(void) const throw() {
    return ("Config Socket: failed to find address");
}

char const* ConfigSocket::NoSuchBlockException::what(void) const throw() {
    return ("Config Socket: failed to find block");
}

} // namespace webserv