#include "Config.hpp"

namespace webserv {

/* Config Class */
std::string Config::default_file = "default";
std::string Config::default_path = "../webserv/";

std::map<int, std::string> const Config::exit_code = Config::init_exit_code();

char const* Config::InvalidSyntaxException::what(void) const throw() {
    return ("Config: Exception: Invalid Syntax");
}

char const* Config::InvalidFileException::what(void) const throw() {
    return ("Config: Exception: Invalid File");
}

std::map<int, std::string> Config::init_exit_code(void) {
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

Config::Config(int argc, char* argv[]) {
    std::string filename;

    // checking to see if a filename was provided
    if (argc > 1) filename = std::string(argv[1]);
    else filename = Config::default_file;

    // checking to see if a path for the file was provided
    if (filename.find("/") == std::string::npos)
        filename = Config::default_path + filename;

    // open file and checking if everything is ok
    this->file.open(filename);
    if (!this->file.good()) { error_file(filename + ": failed to open") };

    // Parsing file
    std::string    line;
    ServerBlock*   server;
    LocationBlock* location;
    bool           in_server = false;
    bool           in_location = false;

    while (getline(this->file, line)) {
        if (line.empty()) continue;

        if (line == "server {") { // beginning of the server block
            if (in_server) { error_syntax(filename, line); }
            server = new ServerBlock();
            in_server = true;
        } else if (line == "}") { // end of the server block
            if (!in_server) { error_syntax(filename, line); }
            this->server_block.push_back(server);
            in_server = false;
        } else if (in_server && !in_location &&
                   server->add_directive(line)) { // directive
            ;
        } else if (in_server &&
                   line.find("location") !=
                       std::string::npos) { // beginning of the location block
            if (in_location) { error_syntax(filename, line); }
            location = new LocationBlock(line);
            in_location = true;
        } else if (line.find("}") !=
                   std::string::npos) { // end of location block
            if (!in_location) { error_syntax(filename, line); }
            server->location_block.push_back(location);
            in_location = false;
        } else if (in_server && in_location &&
                   location->add_directive(line)) // directive
            continue;
        else { error_syntax(filename, line); }
    }
    if (in_server) { error_syntax(filename, line); }
}

Config::~Config(void) {
    for (std::vector<ServerBlock*>::iterator it = this->server_block.begin();
         it != this->server_block.end(); ++it)
        delete *it;
    this->file.close();
}

std::vector<Config::ServerBlock*> Config::server_configs(void) const {
    return (this->server_block);
}

/* ServerBlock Class */

Config::ServerBlock::ServerBlock(void)
    : autoindex(false), port(80), host("localhost"), root("/var/www/html"),
      access_log(Config::default_path + "webserv.log"), body_size(8000) {
    this->index.push_back("index");
    this->index.push_back("index.html");
}

Config::ServerBlock::~ServerBlock(void) {
    for (std::vector<LocationBlock*>::iterator it =
             this->location_block.begin();
         it != this->location_block.end(); ++it)
        delete *it;
}

bool Config::ServerBlock::add_directive(std::string line) {
    std::vector<std::string> command;

    // splitting line into vector of strings
    char* word = strtok(const_cast<char*>(line.c_str()), " \t");
    while (word) {
        command.push_back(word);
        word = strtok(NULL, " \t");
    }

    // checking if command ends with a comma
    if (command.back() == ";") command.pop_back();
    else if (command.back().back() == ';')
        command.back().resize(command.back().size() - 1);
    else return (false);

    // forwarding command to the right function
    if (command[0] == "listen") return (directive_listen(command));
    if (command[0] == "server_name") return (directive_server_name(command));
    if (command[0] == "error_page") return (directive_error_page(command));
    if (command[0] == "client_max_body_size")
        return (directive_max_size(command));
    if (command[0] == "access_log") return (directive_access_log(command));
    if (command[0] == "root") return (directive_root(command));
    if (command[0] == "autoindex") return (directive_autoindex(command));
    if (command[0] == "location") return (directive_location(command));
    if (command[0].find("index") != std::string::npos)
        return (directive_index(command));
    return (false);
}

bool Config::ServerBlock::directive_listen(std::vector<std::string> command) {
    // checking if command size is valid
    if (command.size() != 2) return (false);

    // checking if command[1] is a numeric string
    bool flag = true;
    for (size_t i = 0; i < command[1].size(); i++)
        if (!isdigit(command[1][i])) flag = false;

    std::string p, a;
    size_t      n = command[1].find(":");
    if (n != std::string::npos) {
        a = command[1].substr(0, n);
        bool flag = true;
        p = command[1].substr(n + 1);
        for (size_t i = 0; i < p.size(); i++)
            if (!isdigit(p[i])) return (false);
    } else if (!flag) {
        a = command[1];
    } else {
        p = command[1];
    }

    if (!p.empty()) {
        std::stringstream ss(p);
        ss >> this->port;
        // validating port number
        if (this->port < 1 || this->port > 65535) return (false);
    }

    if (!a.empty()) this->host = a;

    return (true);
}

bool Config::ServerBlock::directive_root(std::vector<std::string> command) {
    // checking if command size is valid
    if (command.size() != 2) return (false);
    // TODO: check if folder is available
    this->root = command[1];
    return (true);
}

bool Config::ServerBlock::directive_server_name(
    std::vector<std::string> command) {
    // checking if command size is valid
    if (command.size() < 2) return (false);

    for (size_t i = 1; i < command.size(); i++)
        this->server_name.push_back(command[i]);
    return (true);
}

bool Config::ServerBlock::directive_error_page(
    std::vector<std::string> command) {
    // checking if command size is valid
    if (command.size() < 3) return (false);

    // checking if error page exists
    std::ifstream file(command.back());
    if (!file.good()) return (false);
    file.close();
    std::string page = command.back();
    command.erase(command.end());
    command.erase(command.begin());

    for (size_t i = 0; i < command.size(); i++) {
        std::stringstream ss(command[i]);
        int               code;
        ss >> code;
        if (code < 100 || code > 600) return (false);
        this->error_page[code] = page;
    }
    return (true);
}

bool Config::ServerBlock::directive_max_size(std::vector<std::string> command) {
    // checking if command size is valid
    if (command.size() != 2) return (false);
    // checking if command[1] is a numeric string
    for (size_t i = 0; i < command[1].size(); i++)
        if (!isdigit(command[1][i])) return (false);
    // TODO: maybe validate max_size - it needs to be reasonable
    std::stringstream ss(command[1]);
    ss >> this->body_size;
    return (true);
}

// TODO: maybe change this->access_log type to std::ifstream
bool Config::ServerBlock::directive_access_log(
    std::vector<std::string> command) {
    // checking if command size is valid
    if (command.size() != 2) return (false);

    std::ifstream file(command[1]);
    if (!file.good()) return (false);
    file.close();

    this->access_log = command[1];
    return (true);
}

bool Config::ServerBlock::directive_autoindex(
    std::vector<std::string> command) {
    // checking if command size is valid
    if (command.size() != 2) return (false);

    if (command[1] == "off") this->autoindex = false;
    if (command[1] == "on") this->autoindex = true;
    else return (false);

    return (true);
}

bool Config::ServerBlock::directive_index(std::vector<std::string> command) {
    // checking if command size is valid
    if (command.size() < 2) return (false);
    // cleaning default index
    this->index.clear();

    // TODO: check if command[0] is part of vector
    for (size_t i = 0; i < command.size(); i++)
        this->index.push_back(command[i]);

    return (true);
}

bool Config::ServerBlock::directive_location(std::vector<std::string> command) {
    // checking if command size is valid
    if (command.size() < 2) return (false);
    return (true);
}

/* LocationBlock Class */
std::string Config::LocationBlock::methods = "GET POST DELETE";

Config::LocationBlock::LocationBlock(std::string line) {
    strtok(const_cast<char*>(line.c_str()), " \t");
    this->uri = strtok(NULL, " \t");
    this->request_method.push_back("GET");
    this->request_method.push_back("POST");
    this->request_method.push_back("DELETE");
}

Config::LocationBlock::~LocationBlock(void) {}

bool Config::LocationBlock::add_directive(std::string line) {
    std::vector<std::string> command;

    // splitting line into vector of strings
    char* word = strtok(const_cast<char*>(line.c_str()), " \t");
    while (word) {
        command.push_back(word);
        word = strtok(NULL, " \t");
    }

    // checking if command ends with a comma
    if (command.back() == ";") command.pop_back();
    else if (command.back().back() == ';')
        command.back().resize(command.back().size() - 1);
    else return (false);

    // forwarding command to the right function
    if (command[0] == "root") return (directive_root(command));
    if (command[0] == "fastcgi_pass") return (directive_fastcgi_pass(command));
    if (command[0] == "request_method")
        return (directive_request_method(command));
    return (false);
}

bool Config::LocationBlock::directive_root(std::vector<std::string> command) {
    // checking if command size is valid
    if (command.size() != 2) return (false);
    // TODO: check if folder is available
    this->root = command[1];
    return (true);
}

bool Config::LocationBlock::directive_fastcgi_pass(
    std::vector<std::string> command) {
    // checking if command size is valid
    if (command.size() != 2) return (false);
    this->fastcgi_pass = command[1];
    return (true);
}

bool Config::LocationBlock::directive_request_method(
    std::vector<std::string> command) {
    // checking if command size is valid
    if (command.size() < 2) return (false);
    // cleaning default index
    this->request_method.clear();

    for (size_t i = 1; i < command.size(); i++) {
        if (Config::LocationBlock::methods.find(command[i]) ==
            std::string::npos)
            return (false);
        this->request_method.push_back(command[i]);
    }
    return (true);
}

} // namespace webserv
