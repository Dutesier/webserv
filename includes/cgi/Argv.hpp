#ifndef CGI_ARGC_HPP
#define CGI_ARGC_HPP

#include <string>
#include <vector>

namespace cgi {

class Argv {

    public:

        Argv(void);
        Argv(std::vector<std::string> const& argc);
        Argv(Argv const& src);

        ~Argv(void);

        Argv& operator=(Argv const& rhs);

        operator char**(void) const;

    private:

        std::vector<std::string> m_argc;
};

} // namespace cgi

#endif /* CGI_ARGC_HPP */