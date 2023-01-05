#include "HTTPResponse.hpp"

#include <gtest/gtest.h>

TEST( test_HTTPResponse, constructor) {

	webserv::HTTPResponse resp(std::pair<int, std::string>(200, "OK"),
							   std::map<std::string, std::string>(),
							   "This is the\nBOOTY" );
	std::string str("HTTP/1.1 200 OK\r\n\r\nThis is the\nBOOTY");
	ASSERT_EQ( str, resp.to_str());
}
