#ifndef CONFIG_SOCKET_HPP
#define CONFIG_SOCKET_HPP

#include "config/Blocks.hpp"
#include "utils/Logger.hpp"
#include "utils/smt.hpp"

#include <set>
#include <vector>

namespace webserv {

class ConfigSocket {

    public:

        ConfigSocket(std::vector< smt::shared_ptr<ServerBlock> > blocks);
        ~ConfigSocket(void);

        std::vector< smt::shared_ptr<ServerBlock> >
            blocks(std::pair<unsigned, std::string> specs);
        std::set< std::pair<unsigned, std::string> >& specs(void);

        struct NoSuchSpecsException : public std::exception {
                char const* what(void) const throw();
        };

#ifndef GTEST_TESTING

    private:

#endif /* GTEST_TESTING */

        std::vector< smt::shared_ptr<ServerBlock> >  m_blocks;
        std::set< std::pair<unsigned, std::string> > m_specs;
};

} // namespace webserv

#endif /* CONFIG_SOCKET_HPP */
