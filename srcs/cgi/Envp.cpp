#include "cgi/Envp.hpp"

#include "http/Request.hpp"
#include "utils/ft_array.hpp"
#include "utils/Logger.hpp"

namespace cgi {

Envp::Envp(void) {}

Envp::Envp(smt::shared_ptr<http::Request> request) {
    fillEnvp("REQUEST_METHOD", request->getMethod());
    fillEnvp("SCRIPT_NAME", request->routeRequest());
    fillEnvp("QUERY_STRING", request->getQuery());
    fillEnvp("CONTENT_LENGTH", request->getHeader("Content-Length"));
    fillEnvp("CONTENT_TYPE", request->getHeader("Content-Type"));
    // fillEnvp("DOCUMENT_ROOT", opts->m_root);

    LOG_E("ENVP: ");
    for (std::vector<std::string>::iterator it = m_envp.begin();
         it != m_envp.end(); ++it) {
        LOG_E((*it));
    }
}

Envp::Envp(Envp const& src) { *this = src; }

Envp::~Envp(void) {}

Envp& Envp::operator=(Envp const& rhs) {
    if (this != &rhs) { m_envp = rhs.m_envp; }
    return (*this);
}

Envp::operator char**(void) const { return (ft::array(m_envp)); }

void Envp::fillEnvp(std::string const& key, std::string const& value) {
    if (!value.empty()) { m_envp.push_back(key + "=" + value); }
}

} // namespace cgi
