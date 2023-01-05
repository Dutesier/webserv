#ifndef HTTPPARSER_HPP
# define HTTPPARSER_HPP

# include "smt.hpp"
# include "HTTPRequest.hpp"

# include <map>
# include <string>
# include <vector>

// // Temporary struct to be implemented later
// typedef struct s_HTTPRequest {
// 	unsigned int method; //enums later
// 	std::string resource;
// 	std::string version;
// 	std::string host;

// 	// unsigned int code; //enums later
// 	std::map<std::string, std::string> headers;
// 	std::string content;
// } HTTPRequest;

class HTTPParser{
public:
	HTTPParser();
	~HTTPParser();
	smt::shared_ptr<HTTPRequest> parse_header(std::string& header);
	int find_next_request(const char* buff) const;
private:
	std::pair<webserv::Method, bool> getMethod(std::string& firstLine);
	std::pair<std::string, bool> getResource(std::string& firstLine);
	std::pair<std::string, bool> getVersion(std::string& firstLine);
	std::pair<std::string, std::string> getKeyValueHeader(std::string& header);
	bool setHeader(smt::shared_ptr<HTTPRequest> pReq, std::string& header);
	bool setHeader(smt::shared_ptr<HTTPRequest> pReq, std::string& header, std::string mustBe);
	std::vector<std::string> separateByCRLF(std::string& raw);

};

#endif /* HTTPPARSER_HPP */
