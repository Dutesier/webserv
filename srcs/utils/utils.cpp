#include "utils/utils.hpp"

namespace webserv {
std::string utils::d_file(std::string f) {

    static std::string file = "default";

    if (!f.empty()) { file = f; }
    return (file);
}

std::string utils::d_path(std::string p) {

    static std::string path = "../webserv/";
    if (!p.empty()) { path = p; }
    return (path);
}

std::string utils::d_root(std::string r) {

    static std::string root = d_path() + "website/";
    if (!r.empty()) { root = d_path() + r; }
    return (root);
}

std::vector<std::string> utils::index_pages(void) {

    std::string arr[2] = {"index", "index.html"};
    return (std::vector<std::string>(arr, arr + 2));
}

std::vector<std::string> utils::allowed_methods(void) {

    std::string arr[4] = {"GET", "POST", "PUT", "DELETE"};
    return (std::vector<std::string>(arr, arr + 4));
}

// TODO: Complete unnecessary...
std::map<int, std::string> utils::status_codes(void) {

    static std::map<int, std::string> code_map;
    if (!code_map.empty()) { return (code_map); }

    code_map[100] = "Continue";
    code_map[101] = "Switching Protocols";
    code_map[102] = "Processing";
    code_map[103] = "Early Hints";
    code_map[200] = "OK";
    code_map[201] = "Created";
    code_map[202] = "Accepted";
    code_map[203] = "Non Authoritative Info";
    code_map[204] = "No Content";
    code_map[205] = "Reset Content";
    code_map[206] = "Partial Content";
    code_map[207] = "Multi Status";
    code_map[208] = "Already Reported";
    code_map[226] = "IM Used";
    code_map[300] = "Multiple Choices";
    code_map[301] = "Moved Permanently";
    code_map[302] = "Found";
    code_map[303] = "See Other";
    code_map[304] = "Not Modified";
    code_map[305] = "Use Proxy";
    code_map[307] = "Temporary Redirect";
    code_map[308] = "Permanent Redirect";
    code_map[400] = "Bad Request";
    code_map[401] = "Unauthorized";
    code_map[402] = "Payment Required";
    code_map[403] = "Forbidden";
    code_map[404] = "Not Found";
    code_map[405] = "Method Not Allowed";
    code_map[406] = "Not Acceptable";
    code_map[407] = "Proxy Auth Required";
    code_map[408] = "Request Timeout";
    code_map[409] = "Conflict";
    code_map[410] = "Gone";
    code_map[411] = "Length Required";
    code_map[412] = "Precondition Failed";
    code_map[413] = "Request Entity Too Large";
    code_map[414] = "Request URI Too Long";
    code_map[415] = "Unsupported Media Type";
    code_map[416] = "Requested Range Not Satisfiable";
    code_map[417] = "Expectation Failed";
    code_map[418] = "Teapot";
    code_map[421] = "Misdirected Request";
    code_map[422] = "Unprocessable Entity";
    code_map[423] = "Locked";
    code_map[424] = "Failed Dependency";
    code_map[425] = "Too Early";
    code_map[426] = "Upgrade Required";
    code_map[428] = "Precondition Required";
    code_map[429] = "Too Many Requests";
    code_map[431] = "Request Header Fields Too Large";
    code_map[451] = "Unavailable For Legal Reasons";
    code_map[500] = "Internal Server Error";
    code_map[501] = "Not Implemented";
    code_map[502] = "Bad Gateway";
    code_map[503] = "Service Unavailable";
    code_map[504] = "GatewayTimeout";
    code_map[505] = "HTTP Version Not Supported";
    code_map[506] = "Variant Also Negotiates";
    code_map[507] = "Insufficient Storage";
    code_map[508] = "Loop Detected";
    code_map[510] = "Not Extended";
    code_map[511] = "Network Authentication Required";

    return (code_map);
}

} // namespace webserv
