#include "cgi/Argv.hpp"

#include "utils/ft_array.hpp"

namespace cgi {

Argv::Argv(void) {}

Argv::Argv(std::vector<std::string> const& argc) : m_argc(argc) {}

Argv::Argv(Argv const& src) { *this = src; }

Argv::~Argv(void) {}

Argv& Argv::operator=(Argv const& rhs) {
    if (this != &rhs) { m_argc = rhs.m_argc; }
    return (*this);
}

Argv::operator char**(void) const { return (ft::array(m_argc)); }

} // namespace cgi
