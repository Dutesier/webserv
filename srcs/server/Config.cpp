#include "server/Config.hpp"

namespace webserv {

/* Config Class */
Config::Config(int argc, char* argv[])
	: m_impl(smt::shared_ptr<impl>(new impl(argc, argv))) {}

Config::~Config(void) {}

std::vector< smt::shared_ptr< ServerConfig> > Config::server_config(void) {
    return (m_impl->m_server);
}

/* Config::impl Class */
std::string const Config::impl::d_file = "default";
std::string const Config::impl::d_path = "../webserv/";

std::string const Config::impl::d_method = "GET POST DELETE";

char const* Config::impl::InvalidFileException::what(void) const throw() {
    return ("webserv::Config::impl invalid file exception");
}

char const* Config::impl::InvalidSyntaxException::what(void) const throw() {
    return ("webser::Config::impl invalid syntax exception");
}

/* Config::impl Class */
Config::impl::impl(int argc, char* argv[]) {

    std::string filename;

    // checking to see if a filename was provided
    if (argc > 1) { filename = std::string(argv[1]); }
    else { filename = Config::impl::d_file; }

    // checking to see if a path for the file was provided
    if (filename.find("/") == std::string::npos) {
        filename = Config::impl::d_path + filename;
	}

    // open file and checking if everything is ok
    m_file.open(filename);
    if (!m_file.good()) { error_file(filename + ": failed to open") };

    // Parsing file
    std::string line;
    bool        in_server = false;
    bool        in_location = false;

    while (getline(m_file, line)) {

        if (line.empty()) { continue; }

        if (line == "server {") { // beginning of the server block

            m_server.push_back(smt::shared_ptr<ServerConfig>(new ServerConfig()));
            in_server = true;
        }
		else if (line == "}") { // end of the server block

            if (!in_server) { error_syntax(filename, line); }
            in_server = false;
        }
		else if (in_server && !in_location && server_cmd(line)) { continue; }
		else if (in_server &&  line.find("location") != std::string::npos) {

			// beginning of the location block
            if (in_location) { error_syntax(filename, line); }

            strtok(const_cast<char*>(line.c_str()), " \t");
            m_server.back()->m_location.push_back(smt::shared_ptr<LocationConfig>(new LocationConfig(strtok(NULL, " \t"))));

            in_location = true;
        }
		else if (line.find("}") != std::string::npos) { // end of location

            if (!in_location) { error_syntax(filename, line); }
            in_location = false;
        }
		else if (in_server && in_location && location_cmd(line)) { continue; }
        else { error_syntax(filename, line); }
    }

    if (in_server) { error_syntax(filename, line); }

	m_file.close();
}

Config::impl::~impl(void) {}

bool Config::impl::server_cmd(std::string line) {

    std::vector<std::string> cmd;

    // splitting line into vector of strings
    char* word = strtok(const_cast<char*>(line.c_str()), " \t");
    while (word) {

        cmd.push_back(word);
        word = strtok(NULL, " \t");
    }

    // checking if cmd ends with a comma
    if (cmd.back() == ";") { cmd.pop_back(); }
    else if (cmd.back().back() == ';') { cmd.back().resize(cmd.back().size() - 1); }
    else { return (false); }

    // forwarding cmd to the right function
    if (cmd[0] == "listen") { return (cmd_listen(cmd)); }
    if (cmd[0] == "server_name") { return (cmd_server_name(cmd)); }
    if (cmd[0] == "error_page") { return (cmd_error_page(cmd)); }
    if (cmd[0] == "client_max_body_size") { return (cmd_max_size(cmd)); }
    if (cmd[0] == "root") { return (cmd_root(cmd)); }
    if (cmd[0] == "autoindex") { return (cmd_autoindex(cmd)); }
    if (cmd[0] == "index") { return (cmd_index(cmd)); }

    return (false);
}

bool Config::impl::location_cmd(std::string line) {

    std::vector<std::string> cmd;

    // splitting line into vector of strings
    char* word = strtok(const_cast<char*>(line.c_str()), " \t");
    while (word) {

        cmd.push_back(word);
        word = strtok(NULL, " \t");
    }

    // checking if cmd ends with a comma
    if (cmd.back() == ";") { cmd.pop_back(); }
    else if (cmd.back().back() == ';') { cmd.back().resize(cmd.back().size() - 1); }
    else { return (false); }

    // forwarding cmd to the right function
    if (cmd[0] == "root") { return (cmd_lroot(cmd)); }
    if (cmd[0] == "fastcgi_pass") { return (cmd_fastcgi_pass(cmd)); }
    if (cmd[0] == "request_method") { return (cmd_request_method(cmd)); }

    return (false);
}

bool Config::impl::cmd_listen(std::vector<std::string> cmd) {

    // checking if cmd size is valid
    if (cmd.size() != 2) { return (false); }

    // checking if cmd[1] is a numeric string
    bool flag = true;
    for (size_t i = 0; i < cmd[1].size(); i++) {
        if (!isdigit(cmd[1][i])) flag = false;
	}

    std::string p, a;
    size_t      n = cmd[1].find(":");
    if (n != std::string::npos) {

        a = cmd[1].substr(0, n);
        bool flag = true;
        p = cmd[1].substr(n + 1);

        for (size_t i = 0; i < p.size(); i++) {
            if (!isdigit(p[i])) { return (false); }
		}
    }
	else if (!flag) { a = cmd[1]; }
	else { p = cmd[1]; }

    if (!p.empty()) {
        std::stringstream ss(p);
        ss >> m_server.back()->m_port;
        // validating port number
        if (m_server.back()->m_port < 1 || m_server.back()->m_port > 65535) {
            return (false);
		}
    }

    if (!a.empty()) { m_server.back()->m_host = a; }

    return (true);
}

bool Config::impl::cmd_server_name(std::vector<std::string> cmd) {

    // checking if cmd size is valid
    if (cmd.size() < 2) { return (false); }

    for (size_t i = 1; i < cmd.size(); i++) {
        m_server.back()->m_server_name.push_back(cmd[i]);
	}

    return (true);
}

bool Config::impl::cmd_error_page(std::vector<std::string> cmd) {

    // checking if cmd size is valid
    if (cmd.size() < 3) { return (false); }

    // checking if error page exists
    std::ifstream file(cmd.back());
    if (!file.good()) { return (false); }
    file.close();

    std::string page = cmd.back();
    cmd.erase(cmd.end());
    cmd.erase(cmd.begin());
    for (size_t i = 0; i < cmd.size(); i++) {

        std::stringstream ss(cmd[i]);
        int               code;
        ss >> code;
        if (code < 100 || code > 600) { return (false); }
        m_server.back()->m_error_page[code] = page;
    }
    return (true);
}

bool Config::impl::cmd_max_size(std::vector<std::string> cmd) {

    // checking if cmd size is valid
    if (cmd.size() != 2) { return (false); }

    // checking if cmd[1] is a numeric string
    for (size_t i = 0; i < cmd[1].size(); i++) {
        if (!isdigit(cmd[1][i])) { return (false); }
	}

    // TODO: maybe validate max_size - it needs to be reasonable
    std::stringstream ss(cmd[1]);
    ss >> m_server.back()->m_body_size;

    return (true);
}

bool Config::impl::cmd_root(std::vector<std::string> cmd) {

    // checking if cmd size is valid
    if (cmd.size() != 2) { return (false); }

    m_server.back()->m_root = cmd[1];
    return (true);
}

bool Config::impl::cmd_autoindex(std::vector<std::string> cmd) {

    // checking if cmd size is valid
    if (cmd.size() != 2) { return (false); }

	if (cmd[1] == "off") { m_server.back()->m_autoindex = false; }
	else if (cmd[1] == "on") { m_server.back()->m_autoindex = true; }
    else { return (false); }

    return (true);
}

bool Config::impl::cmd_index(std::vector<std::string> cmd) {

    // checking if cmd size is valid
    if (cmd.size() < 2) { return (false); }

    // cleaning default index
    m_server.back()->m_index.clear();

    // TODO: check if cmd[0] is part of vector
    for (size_t i = 0; i < cmd.size(); i++) {
        m_server.back()->m_index.push_back(cmd[i]);
	}
    return (true);
}

bool Config::impl::cmd_lroot(std::vector<std::string> cmd) {

    // checking if cmd size is valid
    if (cmd.size() != 2) { return (false); }
    m_server.back()->m_location.back()->m_root = cmd[1];
    return (true);
}

bool Config::impl::cmd_fastcgi_pass(std::vector<std::string> cmd) {

    // checking if cmd size is valid
    if (cmd.size() != 2) { return (false); }
    m_server.back()->m_location.back()->m_fastcgi_pass = cmd[1];
    return (true);
}

bool Config::impl::cmd_request_method(std::vector<std::string> cmd) {

    // checking if cmd size is valid
    if (cmd.size() < 2) { return (false); }

    // cleaning default index
    m_server.back()->m_location.back()->m_request_method.clear();
    for (size_t i = 1; i < cmd.size(); i++) {
        if (Config::impl::d_method.find(cmd[i]) == std::string::npos) {
            return (false);
		}
        m_server.back()->m_location.back()->m_request_method.push_back(cmd[i]);
    }
    return (true);
}

} // namespace webserv
