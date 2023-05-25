#include "config/Blocks.hpp"

namespace webserv {

ServerBlock::ServerBlock(void)
    : m_autoindex(true), m_body_size(BODY_SIZE), m_port("8080"),
      m_host("localhost"), m_index(std::string(D_ROOT_PATH) + "index.html"),
      m_root(D_ROOT_PATH) {}

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

    size_t n = command[1].find(":");

    if (n != std::string::npos) {

        m_host = command[1].substr(0, n);
        m_port = command[1].substr(n + 1);
        flag = true;
    }
    else if (!flag) { m_host = command[1]; }
    else { m_port = command[1]; }

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

LocationBlock::LocationBlock(std::string target) : m_target(target) {
    // TODO: Remove this (m_index and m_autoindex are temporary)
    m_index = "";
    m_autoindex = true;
    // setting up allowed method
    std::string arr[3] = {"GET", "POST", "DELETE"};
    m_allowed_methods = std::set<std::string>(arr, arr + 3);
}

LocationBlock::~LocationBlock(void) {}

char const* LocationBlock::InvalidTargetException::what(void) const throw() {
    return ("LocationBlock: invalid target");
}

// TODO: modify cgi flag.
std::string LocationBlock::cgi(std::vector<std::string> command) {

    // checking if command size is valid
    if (command.size() != 2) { return ("wrong number of arguments"); }

    // validating command[1]
    if (command[1] == ".py" || command[1] == ".php") { m_cgi = command[1]; }
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

// TODO: make this a function that also validates the server
// make this a config function
// if there's a listen directive
// if a location has a root directive
void ServerBlock::createDefaultLocation(void) {
    if (m_location.find("/") != m_location.end()) { return; }
    smt::shared_ptr<LocationBlock> loc(new LocationBlock("/"));
    loc->m_root = m_root;
    m_location.insert(std::make_pair("/", loc));
}

} // namespace webserv
