#include "config/Config.hpp"

namespace webserv {

Config::Config(int argc, char* argv[]) {

    // checking to see if a filename was provided
    if (argc > 1) { m_filename = argv[1]; }
    else { m_filename = D_CONFIG_FILE; }

    // checking to see if a path for filename was provided
    if (m_filename.find("/") == std::string::npos) {
        m_filename = D_CONFIG_PATH + m_filename;
    }

    // parsing config file
    m_file.open(m_filename.c_str());
    if (!m_file.good()) { ERROR_FILE(m_filename); }

    // parsing file and checking for errors
    std::string line;
    bool        in_server = false;
    bool        in_location = false;
    int         i = 0;

    smt::shared_ptr<ServerBlock>   server;
    smt::shared_ptr<LocationBlock> location;

    while (getline(m_file, line)) {

        i++; // incrementing line number
        if (line.empty()) { continue; }

        // beggining of a server block
        if (line.find("server {") != std::string::npos) {

            if (in_server) {
                ERROR_SYNTAX("server block inside another server block");
            }

            // allocating a new block::server and setting in_server to true
            server = smt::shared_ptr<ServerBlock>(new ServerBlock());
            in_server = true;

            continue;
        }

        // beggining of a location block
        if (line.find("location") != std::string::npos) {

            if (in_location) {
                ERROR_SYNTAX("location block inside another location block");
            }
            else if (!in_server) {
                ERROR_SYNTAX("location block not inside server block");
            }

            // allocating a new block::location and setting in_location to true
            std::vector<std::string> target = split_line(line);
            if (target.size() != 3 || target[2] != "{") {
                ERROR_SYNTAX("unrecognized syntax");
            }
            location =
                smt::shared_ptr<LocationBlock>(new LocationBlock(target[1]));

            in_location = true;

            continue;
        }

        // end of a block
        if (line.find("}") != std::string::npos) {

            if (in_location && in_server) {

                // end of location block
                in_location = false;

                // adding location to m_blocks
                server->m_location.insert(
                    std::make_pair(location->m_target, location));
            }
            else if (in_server) {

                // end of server block
                in_server = false;

                // adding server to m_blocks
                m_blocks.push_back(server);
            }
            else { ERROR_SYNTAX("invalid closing of bracket"); }

            continue;
        }

        // directives
        if (in_server) {

            if (in_location) {

                // parsing location block directives

                // splitting line
                std::vector<std::string> command = split_line(line);
                if (command.empty()) { ERROR_SYNTAX("invalid directory"); }

                // checking if command ends with a comma
                if (command.back() == ";") { command.pop_back(); }
                else if (command.back()[command.back().size() - 1] == ';') {
                    command.back().resize(command.back().size() - 1);
                }
                else { ERROR_SYNTAX("invalid end"); }

                std::string err;
                // calling the right directive
                if (command[0] == "root") { err = location->root(command); }
                else if (command[0] == "fastcgi_pass") {
                    err = location->cgi(command);
                }
                else if (command[0] == "allowed_method") {
                    err = location->allowed_methods(command);
                }
                else { ERROR_SYNTAX(command[0] + ": unrecognized command"); }

                if (!err.empty()) { ERROR_SYNTAX(err); }

                continue;
            }
            else if (!in_location) {

                // parsing server block directives

                // splitting line
                bool                     f = false;
                std::vector<std::string> command = split_line(line);
                if (command.empty()) { ERROR_SYNTAX("invalid directory"); }

                // checking if command ends with a comma
                if (command.back() == ";") { command.pop_back(); }
                else if (command.back()[command.back().size() - 1] == ';') {
                    command.back().resize(command.back().size() - 1);
                }
                else { ERROR_SYNTAX("invalid end"); }

                std::string err;
                // calling the right directive
                if (command[0] == "autoindex") {
                    err = server->autoindex(command);
                }
                else if (command[0] == "max_body_size") {
                    err = server->body_size(command);
                }
                else if (command[0] == "listen" && !f) {
                    f = true;
                    err = server->listen(command);
                }
                else if (command[0] == "index") {
                    err = server->index(command);
                }
                else if (command[0] == "root") { err = server->root(command); }
                else if (command[0] == "server_name") {
                    err = server->server_name(command);
                }
                else if (command[0] == "error_page") {
                    err = server->error_page(command);
                }
                else { ERROR_SYNTAX(command[0] + ": unrecognized command"); }

                if (!err.empty()) { ERROR_SYNTAX(err); }

                continue;
            }
        }

        ERROR_SYNTAX("directive outside server block");
    }
    if (in_server) { ERROR_SYNTAX("unclosed bracket"); }

    m_file.close();
}

Config::~Config(void) {}

std::vector<std::string> Config::split_line(std::string line) {

    std::vector<std::string> command;

    // splitting line into vector of strings
    char* word = strtok(const_cast<char*>(line.c_str()), " \t");
    while (word) {

        command.push_back(word);
        word = strtok(NULL, " \t");
    }
    return (command);
}

std::vector< smt::shared_ptr<ServerBlock> > Config::getBlocks(void) {
    return (m_blocks);
}

char const* Config::InvalidFileException::what(void) const throw() {
    return ("webserv::Config invalid file exception");
}

char const* Config::InvalidSyntaxException::what(void) const throw() {
    return ("webserv::Config invalid syntax exception");
}

} // namespace webserv
