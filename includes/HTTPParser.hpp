#ifndef HTTPPARSER_HPP
# define HTTPPARSER_HPP

# include "smt.hpp"

# include <map>
# include <string>
# include <vector>

// Temporary struct to be implemented later
typedef struct s_HTTPRequest {
	unsigned int method; //enums later
	std::string resource;
	std::string version;

	// unsigned int code; //enums later
	// std::map<std::string, std::string> headers;
	// std::string content;
} HTTPRequest;

class HTTPParser{
public:
	smt::shared_ptr<HTTPRequest> parse(std::vector<std::string>& request);
private:

};

#endif /* HTTPPARSER_HPP */
