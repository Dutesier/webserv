#include "server/Config.hpp"

namespace webserv {

#define ERROR(X) return (generate_error(filename, line, i, X))

/* Config Class */
Config::Config(int argc, char* argv[])
    : m_impl(smt::shared_ptr<Config::impl>(new Config::impl(argc, argv))) {}

Config::~Config(void) {}

std::vector<smt::shared_ptr<ServerBlock> > Config::config(void) {
    return (m_impl->m_config);
}

/* Config::impl struct */
Config::impl::impl(int argc, char* argv[]) {

    std::string filename;

    // checking to see if a filename was provided
    if (argc > 1) { filename = argv[1]; }
    else { filename = D_CONFIG_FILE; }

    // checking to see if a path for the file was provided
    if (filename.find("/") == std::string::npos) { // user's not provided a path
        filename = D_CONFIG_PATH + filename;
    }

    // open file to see if everything is ok
    m_file.open(filename.c_str());
    if (!m_file.good()) { error_file(filename); }

    // parsing file and checking for error
    std::string error = parse(filename);
    m_file.close();
    if (!error.empty()) { error_syntax(error); }
}

std::string Config::impl::parse(std::string filename) {

    // parsing file
    std::string line;
    bool        in_server = false;
    bool        in_location = false;
    int         i = 0;

    while (getline(m_file, line)) {

        i++; // incrementing line number
        if (line.empty()) { continue; }

        // beggining of a server block
        if (line.find("server {") != std::string::npos) {

            if (in_server) {
                ERROR("server block inside another server block");
            }

            // this is the beggining of a server block
            // a new ServerBlock needs to be allocated
            ServerBlock*                 dontUse = new ServerBlock();
            smt::shared_ptr<ServerBlock> srv(dontUse);
            m_config.push_back(srv);

            in_server = true;

            continue;
        }

        // beggining of a location block
        if (line.find("location") != std::string::npos) {

            if (in_location) {
                ERROR("location block inside another location block");
            }
            else if (!in_server) {
                ERROR("location block not inside server block");
            }

            // this is the beggining of a location block
            // a new LocationBlock needs to be allocated
            strtok(const_cast<char*>(line.c_str()), " \t");
            LocationBlock* dontUse = new LocationBlock(strtok(NULL, " \t"));
            smt::shared_ptr<LocationBlock> loc(dontUse);
            m_config.back()->m_location.push_back(loc);

            in_location = true;

            continue;
        }

        // end of a block
        if (line.find("}") != std::string::npos) {

            if (in_location && in_server) { in_location = false; }
            else if (in_server) { in_server = false; }
            else { ERROR("invalid closing of bracket"); }

            continue;
        }

        // directives
        if (in_server) {

            if (in_location) {
                result_type res = location(line);
                if (!res.first) { return (res.second); }

                continue;
            }
            else if (!in_location) {

                result_type res = server(line);
                if (!res.first) { ERROR(res.second); }

                continue;
            }
        }

        ERROR("directive outside server block");
    }
    if (in_server) { ERROR("unclosed bracket"); }
    return ("");
}

Config::impl::~impl(void) {}

void Config::impl::error_file(std::string filename) const {

    // setting message for Logger
    LOG_E(filename + ": failed to open");

    // throwing exception
    throw(InvalidFileException());
}

std::string Config::impl::generate_error(std::string filename, std::string line,
                                         int i, std::string msg) const {

    // converting line number to std::string
    std::stringstream ss;
    ss << i;
    std::string nu = ss.str();

    // setting message for Logger
    return ("\n" + filename + ":" + nu + ": \"" + line +
            "\" (syntax error)\n\t" + msg);
}

void Config::impl::error_syntax(std::string msg) const {

    LOG_E(msg);
    // throwing exception
    throw(InvalidSyntaxException());
}

typename Config::impl::result_type Config::impl::server(std::string line) {

    std::vector<std::string> cmd;

    // splitting line into vector of strings
    char* word = strtok(const_cast<char*>(line.c_str()), " \t");
    while (word) {

        cmd.push_back(word);
        word = strtok(NULL, " \t");
    }

    // checking if cmd ends with a comma
    if (cmd.back() == ";") { cmd.pop_back(); }
    else if (cmd.back()[cmd.back().size() - 1] == ';') {
        cmd.back().resize(cmd.back().size() - 1);
    }
    else { return (result_type(false, "invalid end")); }

    // forwarding cmd to the right function
    if (cmd[0] == "listen") { return (cmd_listen(cmd)); }
    if (cmd[0] == "server_name") { return (cmd_server_name(cmd)); }
    if (cmd[0] == "error_page") { return (cmd_error_page(cmd)); }
    if (cmd[0] == "client_max_body_size") { return (cmd_max_size(cmd)); }
    if (cmd[0] == "root") { return (cmd_root(cmd)); }
    if (cmd[0] == "autoindex") { return (cmd_autoindex(cmd)); }
    if (cmd[0] == "index") { return (cmd_index(cmd)); }

    return (result_type(false, cmd[0] + ": unknown command"));
}

typename Config::impl::result_type Config::impl::location(std::string line) {

    std::vector<std::string> cmd;

    // splitting line into vector of strings
    char* word = strtok(const_cast<char*>(line.c_str()), " \t");
    while (word) {

        cmd.push_back(word);
        word = strtok(NULL, " \t");
    }

    // checking if cmd ends with a comma
    if (cmd.back() == ";") { cmd.pop_back(); }
    else if (cmd.back()[cmd.back().size() - 1] == ';') {
        cmd.back().resize(cmd.back().size() - 1);
    }
    else { return (result_type(false, "invalid end")); }

    // forwarding cmd to the right function
    if (cmd[0] == "root") return (cmd_lroot(cmd));
    if (cmd[0] == "fastcgi_pass") return (cmd_fastcgi_pass(cmd));
    if (cmd[0] == "request_method") return (cmd_request_method(cmd));
    return (result_type(false, cmd[0] + ": unknown command"));
}

typename Config::impl::result_type
    Config::impl::cmd_listen(std::vector<std::string> cmd) {

    // checking if cmd size is valid
    if (cmd.size() != 2) {
        return (result_type(false, "wrong number of arguments"));
    }

    // checking if cmd[1] is a numeric string
    bool flag = true;

    for (size_t i = 0; i < cmd[1].size(); i++) {
        if (!isdigit(cmd[1][i])) { flag = false; }
    }

    std::string p, a;
    size_t      n = cmd[1].find(":");

    if (n != std::string::npos) {

        a = cmd[1].substr(0, n);
        flag = true;
        p = cmd[1].substr(n + 1);
        for (size_t i = 0; i < p.size(); i++) {
            if (!isdigit(p[i])) {
                return (result_type(false, p + ": is not a valid port"));
            }
        }
    }
    else if (!flag) { a = cmd[1]; }
    else { p = cmd[1]; }
    if (!p.empty()) {

        std::stringstream ss(p);
        ss >> m_config.back()->m_port;

        // validating port number
        if (m_config.back()->m_port < 1 || m_config.back()->m_port > 65535) {
            return (result_type(false, p + ": is not a valid port"));
        }
    }
    if (!a.empty()) { m_config.back()->m_host = a; }

    return (result_type(true, ""));
}

typename Config::impl::result_type
    Config::impl::cmd_server_name(std::vector<std::string> cmd) {

    // checking if cmd size is valid
    if (cmd.size() < 2) {
        return (result_type(false, "wrong number of arguments"));
    }

    for (size_t i = 1; i < cmd.size(); i++) {
        m_config.back()->m_srv_name.push_back(cmd[i]);
    }
    return (result_type(true, ""));
}

typename Config::impl::result_type
    Config::impl::cmd_error_page(std::vector<std::string> cmd) {

    // checking if cmd size is valid
    if (cmd.size() < 3) {
        return (result_type(false, "wrong number of arguments"));
    }

    // checking if error page exists
    std::ifstream file(cmd.back());

    if (!file.good()) {
        return (result_type(false, cmd.back() + ": failed to open"));
    }
    file.close();

    std::string page = cmd.back();
    cmd.erase(cmd.end());
    cmd.erase(cmd.begin());

    for (size_t i = 0; i < cmd.size(); i++) {

        // checking is std::string is a number
        for (size_t j = 0; j < cmd[i].size(); j++) {
            if (!isdigit(cmd[i][j])) {
                return (result_type(false, cmd[i] + ": invalid status code"));
            }
        }

        // converting std::string into number
        std::stringstream ss(cmd[i]);
        int               code;
        ss >> code;

        // adding status code in m_error_page
        m_config.back()->m_error_page[code] = page;
    }

    return (result_type(true, ""));
}

typename Config::impl::result_type
    Config::impl::cmd_max_size(std::vector<std::string> cmd) {

    // checking if cmd size is valid
    if (cmd.size() != 2) {
        return (result_type(false, "wrong number of arguments"));
    }

    // checking if cmd[1] is a numeric string
    for (size_t i = 0; i < cmd[1].size(); i++) {
        if (!isdigit(cmd[1][i])) {
            return (result_type(false, cmd[1] + ": invalid size"));
        }
    }

    std::stringstream ss(cmd[1]);
    ss >> m_config.back()->m_bsize;
    if (m_config.back()->m_bsize > MAX_BODY_SIZE) {
        return (result_type(false, cmd[1] + ": invalid size"));
    }

    return (result_type(true, ""));
}

typename Config::impl::result_type
    Config::impl::cmd_root(std::vector<std::string> cmd) {

    // checking if cmd size is valid
    if (cmd.size() != 2) {
        return (result_type(false, "wrong number of arguments"));
    }

    // check if folder is available
    DIR* dir = opendir(cmd[1].c_str());
    if (!dir || errno == ENOENT) {
        return (result_type(false, cmd[1] + ": failed to open"));
    }
    closedir(dir);

    m_config.back()->m_root = cmd[1];
    return (result_type(true, ""));
}

typename Config::impl::result_type
    Config::impl::cmd_autoindex(std::vector<std::string> cmd) {

    // checking if cmd size is valid
    if (cmd.size() != 2) {
        return (result_type(false, "wrong number of arguments"));
    }

    // validating command
    if (cmd[1] == "off") { m_config.back()->m_autoidx = false; }
    else if (cmd[1] == "on") { m_config.back()->m_autoidx = true; }
    else { return (result_type(false, cmd[1] + ": unrecognized syntax")); }

    return (result_type(true, ""));
}

typename Config::impl::result_type
    Config::impl::cmd_index(std::vector<std::string> cmd) {

    // checking if cmd size is valid
    if (cmd.size() < 2) {
        return (result_type(false, "wrong number of arguments"));
    }

    // cleaning default index
    m_config.back()->m_idx.clear();

    for (size_t i = 0; i < cmd.size(); i++) {
        m_config.back()->m_idx.push_back(cmd[i]);
    }

    return (result_type(true, ""));
}

typename Config::impl::result_type
    Config::impl::cmd_lroot(std::vector<std::string> cmd) {

    // checking if cmd size is valid
    if (cmd.size() != 2) {
        return (result_type(false, "wrong number of arguments"));
    }

    std::string filename = m_config.back()->m_root + cmd[1];

    // check if folder is available
    DIR* dir = opendir(filename.c_str());
    if (!dir || ENOENT == errno) {
        return (result_type(false, filename + ": failed to open"));
    }
    closedir(dir);

    m_config.back()->m_location.back()->m_root = filename;
    return (result_type(true, ""));
}

typename Config::impl::result_type
    Config::impl::cmd_fastcgi_pass(std::vector<std::string> cmd) {

    // checking if cmd size is valid
    if (cmd.size() != 2) {
        return (result_type(false, "wrong number of arguments"));
    }

    // Im guessing this can be a bool
    m_config.back()->m_location.back()->m_cgi = cmd[1];
    return (result_type(true, ""));
}

typename Config::impl::result_type
    Config::impl::cmd_request_method(std::vector<std::string> cmd) {

    // checking if cmd size is valid
    if (cmd.size() < 2) {
        return (result_type(false, "wrong number of arguments"));
    }

    // cleaning default index
    m_config.back()->m_location.back()->m_req_methods.clear();
    for (size_t i = 1; i < cmd.size(); i++) {
        m_config.back()->m_location.back()->m_req_methods.push_back(cmd[i]);
    }
    return (result_type(true, ""));
}

char const* Config::impl::InvalidFileException::what(void) const throw() {
    return ("webserv::Config::impl invalid file exception");
}

char const* Config::impl::InvalidSyntaxException::what(void) const throw() {
    return ("webser::Config::impl invalid syntax exception");
}

} // namespace webserv
