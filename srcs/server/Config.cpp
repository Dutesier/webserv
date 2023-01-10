#include "server/Config.hpp"

namespace webserv {

/* Config Class */
Config::Config(int argc, char* argv[]) : m_impl(new Config::impl(argc, argv)) {}

Config::~Config(void) { delete this->m_impl; }

std::vector<ServerConfig*> Config::server_config(void) const {
    return (this->m_impl->server);
}

/* Config::impl Class */
std::string const Config::impl::d_file = "default";
std::string const Config::impl::d_path = "../webserv/";

std::map<int, std::string> const Config::impl::exit_code =
    Config::impl::init_exit_code();
std::string const Config::impl::method = "GET POST DELETE";

std::map<int, std::string> Config::impl::init_exit_code(void) {
    std::map<int, std::string> code;
    code[100] = "StatusContinue";
    code[101] = "StatusSwitchingProtocols";
    code[102] = "StatusProcessing";
    code[103] = "StatusEarlyHints";
    code[200] = "StatusOK";
    code[201] = "StatusCreated";
    code[202] = "StatusAccepted";
    code[203] = "StatusNonAuthoritativeInfo";
    code[204] = "StatusNoContent";
    code[205] = "StatusResetContent";
    code[206] = "StatusPartialContent";
    code[207] = "StatusMultiStatus";
    code[208] = "StatusAlreadyReported";
    code[226] = "StatusIMUsed";
    code[300] = "StatusMultipleChoices";
    code[301] = "StatusMovedPermanently";
    code[302] = "StatusFound";
    code[303] = "StatusSeeOther";
    code[304] = "StatusNotModified";
    code[305] = "StatusUseProxy";
    code[307] = "StatusTemporaryRedirect";
    code[308] = "StatusPermanentRedirect";
    code[400] = "StatusBadRequest";
    code[401] = "StatusUnauthorized";
    code[402] = "StatusPaymentRequired";
    code[403] = "StatusForbidden";
    code[404] = "StatusNotFound";
    code[405] = "StatusMethodNotAllowed";
    code[406] = "StatusNotAcceptable";
    code[407] = "StatusProxyAuthRequired";
    code[408] = "StatusRequestTimeout";
    code[409] = "StatusConflict";
    code[410] = "StatusGone";
    code[411] = "StatusLengthRequired";
    code[412] = "StatusPreconditionFailed";
    code[413] = "StatusRequestEntityTooLarge";
    code[414] = "StatusRequestURITooLong";
    code[415] = "StatusUnsupportedMediaType";
    code[416] = "StatusRequestedRangeNotSatisfiable";
    code[417] = "StatusExpectationFailed";
    code[418] = "StatusTeapot";
    code[421] = "StatusMisdirectedRequest";
    code[422] = "StatusUnprocessableEntity";
    code[423] = "StatusLocked";
    code[424] = "StatusFailedDependency";
    code[425] = "StatusTooEarly";
    code[426] = "StatusUpgradeRequired";
    code[428] = "StatusPreconditionRequired";
    code[429] = "StatusTooManyRequests";
    code[431] = "StatusRequestHeaderFieldsTooLarge";
    code[451] = "StatusUnavailableForLegalReasons";
    code[500] = "StatusInternalServerError";
    code[501] = "StatusNotImplemented";
    code[502] = "StatusBadGateway";
    code[503] = "StatusServiceUnavailable";
    code[504] = "StatusGatewayTimeout";
    code[505] = "StatusHTTPVersionNotSupported";
    code[506] = "StatusVariantAlsoNegotiates";
    code[507] = "StatusInsufficientStorage";
    code[508] = "StatusLoopDetected";
    code[510] = "StatusNotExtended";
    code[511] = "StatusNetworkAuthenticationRequired";
    return (code);
}

char const* Config::impl::InvalidFileException::what(void) const throw() {
    return ("webserv::Config::impl invalid file exception");
}

char const* Config::impl::InvalidSyntaxException::what(void) const throw() {
    return ("webserv::Config::impl invalid syntax exception");
}

/* Config::impl Class */
Config::impl::impl(int argc, char* argv[]) {
    std::string filename;

    // checking to see if a filename was provided
    if (argc > 1) filename = std::string(argv[1]);
    else filename = Config::impl::d_file;

    // checking to see if a path for the file was provided
    if (filename.find("/") == std::string::npos)
        filename = Config::impl::d_path + filename;

    // open file and checking if everything is ok
    this->file.open(filename);
    if (!this->file.good()) { error_file(filename + ": failed to open") };

    // Parsing file
    std::string line;
    bool        in_server = false;
    bool        in_location = false;

    while (getline(this->file, line)) {
        if (line.empty()) continue;

        if (line == "server {") { // beginning of the server block
            this->server.push_back(new ServerConfig());
            in_server = true;
        }
        else if (line == "}") { // end of the server block
            if (!in_server) { error_syntax(filename, line); }
            in_server = false;
        }
        else if (in_server && !in_location && server_cmd(line)) { // directive
            ;
        }
        else if (in_server && // beginning of the location block
                 line.find("location") != std::string::npos) {
            if (in_location) { error_syntax(filename, line); }
            strtok(const_cast<char*>(line.c_str()), " \t");
            this->server.back()->location.push_back(
                new LocationConfig(strtok(NULL, " \t")));
            in_location = true;
        }
        else if (line.find("}") != std::string::npos) { // end of location
            if (!in_location) { error_syntax(filename, line); }
            in_location = false;
        }
        else if (in_server && in_location && location_cmd(line)) // directive
            continue;
        else { error_syntax(filename, line); }
    }
    if (in_server) { error_syntax(filename, line); }
}

Config::impl::~impl(void) {
    for (std::vector<ServerConfig*>::iterator it = this->server.begin();
         it != this->server.end(); ++it)
        delete *it;
    this->file.close();
}

bool Config::impl::server_cmd(std::string line) {
    std::vector<std::string> cmd;
    // splitting line into vector of strings
    char* word = strtok(const_cast<char*>(line.c_str()), " \t");
    while (word) {
        cmd.push_back(word);
        word = strtok(NULL, " \t");
    }
    // checking if cmd ends with a comma
    if (cmd.back() == ";") cmd.pop_back();
    else if (cmd.back().back() == ';') cmd.back().resize(cmd.back().size() - 1);
    else return (false);
    // forwarding cmd to the right function
    if (cmd[0] == "listen") return (cmd_listen(cmd));
    if (cmd[0] == "server_name") return (cmd_server_name(cmd));
    if (cmd[0] == "error_page") return (cmd_error_page(cmd));
    if (cmd[0] == "client_max_body_size") return (cmd_max_size(cmd));
    if (cmd[0] == "root") return (cmd_root(cmd));
    if (cmd[0] == "autoindex") return (cmd_autoindex(cmd));
    if (cmd[0] == "index") return (cmd_index(cmd));
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
    if (cmd.back() == ";") cmd.pop_back();
    else if (cmd.back().back() == ';') cmd.back().resize(cmd.back().size() - 1);
    else return (false);
    // forwarding cmd to the right function
    if (cmd[0] == "root") return (cmd_lroot(cmd));
    if (cmd[0] == "fastcgi_pass") return (cmd_fastcgi_pass(cmd));
    if (cmd[0] == "request_method") return (cmd_request_method(cmd));
    return (false);
}

bool Config::impl::cmd_listen(std::vector<std::string> cmd) {
    // checking if cmd size is valid
    if (cmd.size() != 2) return (false);
    // checking if cmd[1] is a numeric string
    bool flag = true;
    for (size_t i = 0; i < cmd[1].size(); i++)
        if (!isdigit(cmd[1][i])) flag = false;
    std::string p, a;
    size_t      n = cmd[1].find(":");
    if (n != std::string::npos) {
        a = cmd[1].substr(0, n);
        bool flag = true;
        p = cmd[1].substr(n + 1);
        for (size_t i = 0; i < p.size(); i++)
            if (!isdigit(p[i])) return (false);
    }
    else if (!flag) { a = cmd[1]; }
    else { p = cmd[1]; }
    if (!p.empty()) {
        std::stringstream ss(p);
        ss >> this->server.back()->port;
        // validating port number
        if (this->server.back()->port < 1 || this->server.back()->port > 65535)
            return (false);
    }
    if (!a.empty()) this->server.back()->host = a;
    return (true);
}

bool Config::impl::cmd_server_name(std::vector<std::string> cmd) {
    // checking if cmd size is valid
    if (cmd.size() < 2) return (false);
    for (size_t i = 1; i < cmd.size(); i++)
        this->server.back()->server_name.push_back(cmd[i]);
    return (true);
}

bool Config::impl::cmd_error_page(std::vector<std::string> cmd) {
    // checking if cmd size is valid
    if (cmd.size() < 3) return (false);
    // checking if error page exists
    std::ifstream file(cmd.back());
    if (!file.good()) return (false);
    file.close();
    std::string page = cmd.back();
    cmd.erase(cmd.end());
    cmd.erase(cmd.begin());
    for (size_t i = 0; i < cmd.size(); i++) {
        std::stringstream ss(cmd[i]);
        int               code;
        ss >> code;
        if (code < 100 || code > 600) return (false);
        this->server.back()->error_page[code] = page;
    }
    return (true);
}

bool Config::impl::cmd_max_size(std::vector<std::string> cmd) {
    // checking if cmd size is valid
    if (cmd.size() != 2) return (false);
    // checking if cmd[1] is a numeric string
    for (size_t i = 0; i < cmd[1].size(); i++)
        if (!isdigit(cmd[1][i])) return (false);
    // TODO: maybe validate max_size - it needs to be reasonable
    std::stringstream ss(cmd[1]);
    ss >> this->server.back()->body_size;
    return (true);
}

bool Config::impl::cmd_root(std::vector<std::string> cmd) {
    // checking if cmd size is valid
    if (cmd.size() != 2) return (false);
    // TODO: check if folder is available
    this->server.back()->root = cmd[1];
    return (true);
}

bool Config::impl::cmd_autoindex(std::vector<std::string> cmd) {
    // checking if cmd size is valid
    if (cmd.size() != 2) return (false);
    if (cmd[1] == "off") this->server.back()->autoindex = false;
    if (cmd[1] == "on") this->server.back()->autoindex = true;
    else return (false);
    return (true);
}

bool Config::impl::cmd_index(std::vector<std::string> cmd) {
    // checking if cmd size is valid
    if (cmd.size() < 2) return (false);
    // cleaning default index
    this->server.back()->index.clear();
    // TODO: check if cmd[0] is part of vector
    for (size_t i = 0; i < cmd.size(); i++)
        this->server.back()->index.push_back(cmd[i]);
    return (true);
}

bool Config::impl::cmd_lroot(std::vector<std::string> cmd) {
    // checking if cmd size is valid
    if (cmd.size() != 2) return (false);
    this->server.back()->location.back()->root = cmd[1];
    return (true);
}

bool Config::impl::cmd_fastcgi_pass(std::vector<std::string> cmd) {
    // checking if cmd size is valid
    if (cmd.size() != 2) return (false);
    this->server.back()->location.back()->fastcgi_pass = cmd[1];
    return (true);
}

bool Config::impl::cmd_request_method(std::vector<std::string> cmd) {
    // checking if cmd size is valid
    if (cmd.size() < 2) return (false);
    // cleaning default index
    this->server.back()->location.back()->request_method.clear();
    for (size_t i = 1; i < cmd.size(); i++) {
        if (Config::impl::method.find(cmd[i]) == std::string::npos)
            return (false);
        this->server.back()->location.back()->request_method.push_back(cmd[i]);
    }
    return (true);
}

} // namespace webserv
