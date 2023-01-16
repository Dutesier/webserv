#include "http/Uri.hpp"

namespace webserv {

Uri::Uri(std::string uri) {

	// finding scheme
	size_t scheme_pos = uri.find(":");
	if (scheme_pos != std::string::npos) {
		m_scheme = uri.substr(0, scheme_pos);
		scheme_pos += 3;
	}
	else { scheme_pos = 0; }

	// finding host
	size_t host_pos;
	if (!m_scheme.empty()) {

		host_pos = uri.find("/", scheme_pos);
		if (host_pos != std::string::npos) {
			m_host = uri.substr(scheme_pos, host_pos - scheme_pos);
		}
	}
	else { host_pos = 0;}

	// finding path and query
	size_t path_pos = uri.find("?", host_pos);
	if (path_pos != std::string::npos) {

		m_path = uri.substr(host_pos, path_pos - host_pos);
		m_query = uri.substr(++path_pos);
	}
	else { m_path = uri.substr(host_pos); }
}

Uri::~Uri(void) {}

std::string Uri::to_str(void) {
	return ((m_scheme.empty() ? "" : m_scheme + "://" + m_host) + m_path + (m_query.empty() ? "" : "?" + m_query));
}

std::string Uri::path_w_root(std::string root) {

	// if path is just /
	if (m_path == "/") { return (root); }

	// path always begins with / so root should not end with / as well
	if (root[root.size() - 1] == '/') {
		return (root.substr(0, root.size() - 1) + m_path);
	}
	return (root + m_path);
}

}
