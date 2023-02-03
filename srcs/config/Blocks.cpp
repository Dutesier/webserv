#include "config/Blocks.hpp"

namespace webserv {

ServerBlock::ServerBlock(void)
    : m_autoindex(true), m_body_size(BODY_SIZE), m_port(80), m_host("*"),
      m_index(std::string(D_ROOT_PATH) + "index.html"), m_root(D_ROOT_PATH) {}

ServerBlock::~ServerBlock(void) {}

std::string ServerBlock::autoindex(std::vector<std::string> command) {

    // checking if command size is valid
    if (command.size() != 2) { return ("wrong number of arguments"); }

    // validating command[1]
    if (command[1] == "off") { m_autoindex = false; }
    else if (command[1] == "on") { m_autoindex = true; }
    else { return (command[1] + ": unrecognized syntax"); }

    return ("");
}

std::string ServerBlock::body_size(std::vector<std::string> command) {

    // checking if command size is valid
    if (command.size() != 2) { return ("wrong number of arguments"); }

    // checking if command[1] is a numeric string
    for (size_t i = 0; i < command[1].size(); i++) {
        if (!isdigit(command[1][i])) {
            return (command[1] + ": invalid max body size");
        }
    }

    // converting command[1] to a int
    unsigned          size;
    std::stringstream ss(command[1]);
    ss >> size;

    // checking if m_body_size is whithin valid limits
    if (size > MAX_BODY_SIZE) {
        return (command[1] + ": invalid max body size");
    }

    m_body_size = size;

    return ("");
}

std::string ServerBlock::listen(std::vector<std::string> command) {

    // checking if command size is valid
    if (command.size() != 2) { return ("wrong number of arguments"); }

    // checking if command[1] is a numeric string
    bool flag = true;

    for (size_t i = 0; i < command[1].size(); i++) {
        if (!isdigit(command[1][i])) { flag = false; }
    }

    std::string p, a;
    size_t      n = command[1].find(":");

    if (n != std::string::npos) {

        a = command[1].substr(0, n);
        if (a == "localhost") { a = "127.0.0.1"; }
        if (a == "*") { a = "127.0.0.1"; }
        flag = true;
        p = command[1].substr(n + 1);
        for (size_t i = 0; i < p.size(); i++) {
            if (!isdigit(p[i])) { return (p + ": invalid port"); }
        }
    }
    else if (!flag) {

        if (command[1] == "localhost") { a = "127.0.0.1"; }
        else if (command[1] == "*") { a = "*"; }
        else { a = command[1]; }
    }
    else { p = command[1]; }

    if (!p.empty()) {

        unsigned          port;
        std::stringstream ss(p);
        ss >> port;

        // validating port number
        if (port < 1 || port > 65535) { return (p + ": invalid port"); }
        m_port = port;
    }
    if (!a.empty()) {

        if (a != "*") {
            // validating address
            int i1, i2, i3, i4;
            int matched = sscanf(a.c_str(), "%d.%d.%d.%d", &i1, &i2, &i3, &i4);
            if (matched != 4 || i1 < 0 || i1 > 255 || i2 < 0 || i2 > 255 ||
                i3 < 0 || i3 > 255 || i4 < 0 || i4 > 255) {
                return (a + ": invalid address");
            }
        }

        m_host = a;
    }

    return ("");
}

std::string ServerBlock::index(std::vector<std::string> command) {

    // checking if command size is valid
    if (command.size() != 2) { return ("wrong number of arguments"); }

    // check if file is available
    std::ifstream file(command[1].c_str());
    if (!file.good()) { return (command[1] + ": failed to open"); }
    file.close();

    m_index = command[1];
    return ("");
}

std::string ServerBlock::root(std::vector<std::string> command) {

    // checking if command size is valid
    if (command.size() != 2) { return ("wrong number of arguments"); }

    // check if folder is available
    DIR* dir = opendir(command[1].c_str());
    if (!dir || errno == ENOENT) { return (command[1] + ": failed to open"); }
    closedir(dir);

    m_root = command[1];
    return ("");
}

std::string ServerBlock::server_name(std::vector<std::string> command) {

    // checking if command size is valid
    if (command.size() != 2) { return ("wrong number of arguments"); }

    m_server_name = command[1];
    return ("");
}

std::string ServerBlock::error_page(std::vector<std::string> command) {

    // checking if command size is valid
    if (command.size() < 3) { return ("wrong number of arguments"); }

    // checking if file is available
    std::ifstream file(command.back().c_str());
    if (!file.good()) { return (command.back() + ": failed to open"); }
    file.close();

    std::string page = command.back();
    command.erase(command.end());
    command.erase(command.begin());

    for (size_t i = 0; i < command.size(); i++) {

        // checking is std::string is a number
        for (size_t j = 0; j < command[i].size(); j++) {
            if (!isdigit(command[i][j])) {
                return (command[i] + ": invalid status code");
            }
        }

        // converting std::string into number
        std::stringstream ss(command[i]);
        int               code;
        ss >> code;

        // adding status code in m_error_page
        m_error_page[code] = page;
    }
    return ("");
}

LocationBlock::LocationBlock(std::string target)
    : m_cgi(false), m_target(target) {

    // setting up allowed method
    std::string arr[3] = {"GET", "POST", "DELETE"};
    m_allowed_methods = std::set<std::string>(arr, arr + 3);
}

LocationBlock::~LocationBlock(void) {}

char const* LocationBlock::InvalidTargetException::what(void) const throw() {
    return ("LocationBlock: invalid target");
}

std::string LocationBlock::cgi(std::vector<std::string> command) {

    // checking if command size is valid
    if (command.size() != 2) { return ("wrong number of arguments"); }

    // validating command[1]
    if (command[1] == "false") { m_cgi = false; }
    else if (command[1] == "true") { m_cgi = true; }
    else { return (command[1] + ": unrecognized syntax"); }

    return ("");
}

std::string LocationBlock::root(std::vector<std::string> command) {

    // checking if command size is valid
    if (command.size() != 2) { return ("wrong number of arguments"); }

    // check if folder is available
    DIR* dir = opendir(command[1].c_str());
    if (!dir || errno == ENOENT) { return (command[1] + ": failed to open"); }
    closedir(dir);

    m_root = command[1];
    return ("");
}

std::string LocationBlock::allowed_methods(std::vector<std::string> command) {

    // checking if command size is valid
    if (command.size() < 2) { return ("wrong number of arguments"); }

    // cleaning default methods
    m_allowed_methods.clear();
    for (size_t i = 1; i < command.size(); i++) {
        m_allowed_methods.insert(command[i]);
    }

    return ("");
}

bool operator==(LocationBlock const& lhs, LocationBlock const& rhs) {

    return (lhs.m_cgi == rhs.m_cgi && lhs.m_target == rhs.m_target &&
            lhs.m_root == rhs.m_root &&
            lhs.m_allowed_methods == rhs.m_allowed_methods);
}

bool operator==(ServerBlock const& lhs, ServerBlock const& rhs) {

    return (lhs.m_autoindex == rhs.m_autoindex &&
            lhs.m_body_size == rhs.m_body_size && lhs.m_port == rhs.m_port &&
            lhs.m_host == rhs.m_host && lhs.m_index == rhs.m_index &&
            lhs.m_root == rhs.m_root &&
            lhs.m_server_name == rhs.m_server_name &&
            lhs.m_error_page == rhs.m_error_page &&
            lhs.m_location == rhs.m_location);
}

} // namespace webserv
