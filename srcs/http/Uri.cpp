#include "http/Uri.hpp"

Uri::Uri(ft::string uri) {

    size_t scheme_pos = ft::string(uri).find("://");
    if (scheme_pos != std::string::npos) {
        m_scheme = uri.substr(0, scheme_pos);
        uri = uri.substr(scheme_pos + 3);
        scheme_pos += 3;
    }
    else { scheme_pos = 0; }

    std::vector<ft::string> subs = uri.split("/");
    if (subs.size() > 1) {
        std::vector<ft::string> tmp = subs[0].split(":");
        m_host = tmp[0];
        if (tmp.size() > 1) { m_port = tmp[1]; }
        size_t host_pos = uri.find("/");
        if (host_pos != std::string::npos) { uri = uri.substr(host_pos + 1); }
        else { uri = uri.substr(scheme_pos); }
    }

    if (uri.at(0) == '/') { uri = uri.substr(1); }
    size_t path_pos = uri.find("?");
    if (path_pos != std::string::npos) {

        m_path = uri.substr(0, path_pos);
        m_query = uri.substr(++path_pos);
    }
    else { m_path = uri; }
}

Uri::~Uri(void) {}

void Uri::route(std::string target, std::string root) {
    if (target == "" || root == "") { return; }
    else if (target == "/") { m_routedPath = root; }
    else {
        m_routedPath = m_path;
        if (m_routedPath.find(target) == 0) {
            m_routedPath.replace(
                0, target.size() - ((target[target.size() - 1] == '/') ? 1 : 0),
                root);
        }
    }
}