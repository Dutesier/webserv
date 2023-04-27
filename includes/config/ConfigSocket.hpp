#ifndef CONFIG_SOCKET_HPP
#define CONFIG_SOCKET_HPP

#include "config/Blocks.hpp"
#include "socket/ServerAddress.hpp"
#include "utils/Logger.hpp"
#include "utils/smt.hpp"

#include <set>
#include <vector>

namespace webserv {

class ConfigSocket {

    public:

        static smt::shared_ptr<ServerAddress>
            getAddress(std::pair<unsigned, std::string> specs);

        static std::set< std::pair<unsigned, std::string> > getSpecs(void);

        static smt::shared_ptr<ServerBlock>
            getConfigBlock(unsigned port, std::string host,
                           std::string hostHeader = "");

        static smt::shared_ptr<LocationBlock>
            getLocationBlock(unsigned port, std::string host,
                             std::string hostHeader, std::string uri);

        static void
            setBlocks(std::vector< smt::shared_ptr<ServerBlock> > blocks);

        struct NoSuchAddressException : std::exception {
                char const* what(void) const throw();
        };

        struct NoSuchBlockException : std::exception {
                char const* what(void) const throw();
        };

    private:

        static smt::shared_ptr<LocationBlock>
            getLocationBlock(smt::shared_ptr<ServerBlock> block,
                             std::string                  uri);

        static std::vector<std::string> splitLine(std::string uri);
        static int getCountOfDirs(std::vector<std::string> cmd,
                                  std::vector<std::string> target);

        static std::vector< smt::shared_ptr<ServerBlock> >   m_blocks;
        static std::set< std::pair<unsigned, std::string> >  m_specs;
        static std::vector< smt::shared_ptr<ServerAddress> > m_addresses;
};

} // namespace webserv

#endif /* CONFIG_SOCKET_HPP */
