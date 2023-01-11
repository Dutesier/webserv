#include "http/HTTPResponse.hpp"

namespace webserv {

HTTPResponse::HTTPResponse(void): init(false) {} 

HTTPResponse::HTTPResponse(int status_code, ServerConfig config): init(true), status_map(this->create_code_map()), version("HTTP/1.1") {
	
	std::string	error_file;
	
	//getting status
	for (StatusMap::const_iterator it = this->status_map.begin(); it != this->status_map.end(); it++) {
		if (status_code == it->first) {
			this->status.first = status_code;
			this->status.second = it->second;
			break;
		}
	}
	if (this->status.first == 0) {
		this->status.first = 500;
		this->status.second = "Internal Server Error";
	}
	//getting body
	/* This will need to change after getting access to server_config but it will
	be similar */

	// std::map<int, std::string>::const_iterator it = config.error_page.find(this->status.first);
	// if (it != config.error_page.end())
	// 	error_file = it->second;
	// else
	// 	error_file = "default_path" // get default path

	std::stringstream ss;
    ss << this->status.first;
    std::string error_code = ss.str();
	error_file = "../webserv/error_pages/" + error_code + ".html";
	std::ifstream	body_file(error_file);
	if (body_file.fail()) {
		body_file.close();
		body_file.open("../webserv/error_pages/500.html");
	}
	this->body = std::string((std::istreambuf_iterator<char>(body_file)), std::istreambuf_iterator<char>());
	if (!this->header.empty())
		this->header.clear();
	//getting headers
	this->header.insert(std::pair<std::string, std::string>("Content-type", "text/html"));
	ss.str("");
	ss << body.size();
	this->header.insert(std::pair<std::string, std::string>("Content-length", ss.str()));
	
}

HTTPResponse::HTTPResponse(int code, HTTPHeader header, std::string body): status_map(this->create_code_map()), header(header), body(body), init(true),
	version("HTTP/1.1") {
	StatusMap::const_iterator it = this->status_map.find(code);
	this->status.first = code;
	this->status.second = it->second;
	if (this->body.size() != 0) {
		std::stringstream ss;
		ss << body.size();
		this->header.insert(std::pair<std::string, std::string>("Content-Length", ss.str()));
	}
}

HTTPResponse::~HTTPResponse(void) {
}

std::string HTTPResponse::to_str(void) const {

    // status line
    std::stringstream ss;
    ss << this->status.first;
    std::string code = ss.str();
    std::string start_line =
        std::string(this->version + SP + code + SP + this->status.second);

    // header
    std::string h = "";
    for (HTTPHeader::const_iterator it = this->header.begin();
         it != this->header.end(); it++)
        h += std::string((*it).first + ": " + (*it).second + CRLF);

    return (std::string(start_line + CRLF + h +
                        (this->body.empty() ? (CRLF) : (CRLF + body))));

    // return (std::string(start_line /* + CRLF + header.to_str() */ +
    //                     (this->body.empty() ? (CRLF + CRLF) : (CRLF + CRLF +
    //                     this->body))));
}

std::map<int, std::string> HTTPResponse::create_code_map(void) const {
	std::map<int, std::string>	code_map;

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
	return code_map;
}

std::map<std::string, std::string> HTTPResponse::create_mime_map(void) const{
	std::map<std::string, std::string>	mime_map;
	
	mime_map[".html"] = "text/html";
	mime_map[".htm"] = "text/html";
	mime_map[".txt"] = "text/plain";
	mime_map[".json"] = "application/json";
	mime_map[".js"] = "application/javascript";
	mime_map[".xml"] = "application/xml";
	mime_map[".jpg"] = "image/jpeg";
	mime_map[".jpeg"] = "image/jpeg";
	mime_map[".png"] = "image/png";
	mime_map[".gif"] = "image/gif";
	mime_map[".pdf"] = "application/pdf";
	mime_map[".exe"] = "application/octet-stream";
	mime_map[".zip"] = "application/octet-stream";
	mime_map[".tar"] = "application/octet-stream";
	return mime_map;
}

} // namespace webserv