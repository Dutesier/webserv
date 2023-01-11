#ifndef HTTPPARSER_HPP
# define HTTPPARSER_HPP

# include "smt.hpp"
# include "HTTPRequest.hpp"

# include <map>
# include <string>
# include <vector>

#ifndef MAX_HEADER_SIZE
# define MAX_HEADER_SIZE 8192
#endif

#ifndef MAX_BODY_SIZE
# define MAX_BODY_SIZE 2147483648
#endif


class HTTPParser{
public:
	HTTPParser();
	~HTTPParser();

	smt::shared_ptr<HTTPRequest> getNextRequest(std::string received);
private:
	std::pair<webserv::Method, bool> getMethod(std::string& firstLine);
	std::pair<std::string, bool> getResource(std::string& firstLine);
	std::pair<std::string, bool> getVersion(std::string& firstLine);
	std::pair<std::string, std::string> getKeyValueHeader(std::string& header);
	bool setHeader(smt::shared_ptr<HTTPRequest> pReq, std::string& header);
	bool setHeader(smt::shared_ptr<HTTPRequest> pReq, std::string& header, std::string mustBe);
	std::vector<std::string> separateByCRLF(std::string& raw);
	smt::shared_ptr<HTTPRequest> parse_header(std::string& header);
	int find_next_request(std::string& buff) const;

};

#endif /* HTTPPARSER_HPP */
