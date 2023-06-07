#ifndef CGI_ENVP_HPP
#define CGI_ENVP_HPP

#include "utils/smt.hpp"

#include <string>
#include <vector>

namespace http {
class Request;
}

namespace cgi {

class Envp {

    public:

        Envp(void);
        Envp(smt::shared_ptr<http::Request> request);
        Envp(Envp const& src);

        ~Envp(void);

        Envp& operator=(Envp const& rhs);

        operator char**(void) const;

    private:

        void fillEnvp(std::string const& key, std::string const& value);

        std::vector<std::string> m_envp;
};

} // namespace cgi

#endif /* CGI_ENVP_HPP */