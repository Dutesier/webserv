#include "http/HTTPResponse.hpp"

#include <gtest/gtest.h>

// class test_HTTPResponse : public testing::Test {
// 	public:
// 		void	SetUp( void ) {
// 			this->response = new webserv::HTTPResponse();
// 		}

// 		void TearDown( void ) {
// 			delete this->response;
// 		}
	
// 	protected:
// 		webserv::HTTPResponse *response;

// };

TEST(test_HTTPResponse, constructor) {

	webserv::ServerConfig	conf;

	std::string	str1 = "HTTP/1.1 404 Not Found\r\nContent-length: 197\r\nContent-type: text/html\r\n\r\n<!DOCTYPE html>\n<html>\n  <head>\n    <title>404 Not Found</title>\n  </head>\n  <body>\n    <h1>404 Not Found</h1>\n    <p>The requested resource could not be found on the server.</p>\n  </body>\n</html>\n";
	std::string str2 = "HTTP/1.1 500 Internal Server Error\r\nContent-length: 269\r\nContent-type: text/html\r\n\r\n<!DOCTYPE html>\n<html>\n  <head>\n    <title>Error 500: Internal Server Error</title>\n  </head>\n  <body>\n    <h1>Error 500: Internal Server Error</h1>\n    <p>An unexpected condition was encountered by the server while trying to fulfill the request.</p>\n  </body>\n</html>\n";

    webserv::HTTPResponse resp(200, std::map<std::string, std::string>(),
                               "This is the\nBOOTY");
    ASSERT_EQ(str1, resp.to_str());
	
	webserv::HTTPResponse resp2(404, conf);
	ASSERT_EQ(str1, resp2.to_str());
	
	webserv::HTTPResponse resp3(70, conf);
	ASSERT_EQ(str2, resp3.to_str());
	
	webserv::HTTPResponse resp4;
	ASSERT_EQ(resp4.init, false);

    webserv::HTTPResponse resp5(800, std::map<std::string, std::string>(),
								"This is the\nBOOTY");
    ASSERT_EQ(str2, resp5.to_str());

}
