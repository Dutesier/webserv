#include "http/HTTPHandler.hpp"
#include "http/HTTPResponse.hpp"
#include "http/HTTPRequest.hpp"
#include "server/Blocks.hpp"

#include <gtest/gtest.h>

#define REQ_DIR "GET / HTTP/1.1\r\nHost: example.com\r\nContent-Type: 0\r\n\r\n"
#define REQ_FILE1 "GET /index.html HTTP/1.1\r\nHost: example.com\r\nContent-Type: 0\r\n\r\n"
#define REQ_FILE2 "GET /file/doest/not/exist HTTP/1.1\r\nHost: example.com\r\nContent-Type: 0\r\n\r\n"

TEST(test_HTTPHandler, generate_error_response_default) {

    // creating body
    std::string body =
        "<!DOCTYPE html><html>  <head>    <title>404 - Not Found</title>  "
        "</head>  <body>    <h1>404 - Not Found</h1>  </body></html>";
    // creating header
    std::map<std::string, std::string> header;
    header["Content-Length"] = "124";

    // creating the desired response for comparison
    smt::shared_ptr<webserv::HTTPResponse> resp(
        new webserv::HTTPResponse(404, header, body));
    smt::shared_ptr<webserv::ServerBlock> config(new webserv::ServerBlock);

    // calling generate_error_response()
    smt::shared_ptr<webserv::HTTPResponse> err =
        webserv::generate_error_response(404, config);

    // checking if responses are equal
    ASSERT_EQ(err->m_status, resp->m_status);
    ASSERT_EQ(err->m_reason, resp->m_reason);
    ASSERT_EQ(err->m_header, resp->m_header);
    ASSERT_EQ(err->m_body, resp->m_body);
    ASSERT_EQ(err->m_version, resp->m_version);
}

TEST(test_HTTPHandler, generate_error_response_custom) {

    // creating body
    std::string body =
        "<!DOCTYPE html>\n<html "
        "lang=\"en\">\n\t<head>\n\t\t<title></title>\n\t\t<meta "
        "charset=\"UTF-8\">\n\t\t<meta name=\"viewport\" "
        "content=\"width=device-width, initial-scale=1\">\n\t\t<link "
        "href=\"css/style.css\" "
        "rel=\"stylesheet\">\n\t</head>\n\t<body>\n\t\n\t</body>\n</html>\n";
    // creating header
    std::map<std::string, std::string> header;
    header["Content-Length"] = "238";

    // creating the desired response for comparison
    smt::shared_ptr<webserv::HTTPResponse> resp(
        new webserv::HTTPResponse(500, header, body));
    smt::shared_ptr<webserv::ServerBlock> config(new webserv::ServerBlock);
    config->m_error_page[500] = "../webserv/website/50X.html";

    // calling generate_error_response()
    smt::shared_ptr<webserv::HTTPResponse> err =
        webserv::generate_error_response(500, config);

    // checking if responses are equal
    ASSERT_EQ(err->m_status, resp->m_status);
    ASSERT_EQ(err->m_reason, resp->m_reason);
    ASSERT_EQ(err->m_header, resp->m_header);
    ASSERT_EQ(err->m_body, resp->m_body);
    ASSERT_EQ(err->m_version, resp->m_version);
}

TEST(test_HTTPHandler, generate_error_response_custom_error) {

    // creating body
    std::string body = "<!DOCTYPE html><html>  <head>    <title>500 - Internal "
                       "Server Error</title>  </head>  <body>    <h1>500 - "
                       "Internal Server Error</h1>  </body></html>";
    // creating header
    std::map<std::string, std::string> header;
    header["Content-Length"] = "148";

    // creating the desired response for comparison
    smt::shared_ptr<webserv::HTTPResponse> resp(
        new webserv::HTTPResponse(500, header, body));
    smt::shared_ptr<webserv::ServerBlock> config(new webserv::ServerBlock);
    config->m_error_page[500] = "../webserv/website/doesnt/exist";

    // calling generate_error_response()
    smt::shared_ptr<webserv::HTTPResponse> err =
        webserv::generate_error_response(500, config);

    // checking if responses are equal
    ASSERT_EQ(err->m_status, resp->m_status);
    ASSERT_EQ(err->m_reason, resp->m_reason);
    ASSERT_EQ(err->m_header, resp->m_header);
    ASSERT_EQ(err->m_body, resp->m_body);
    ASSERT_EQ(err->m_version, resp->m_version);
}

TEST(test_HTTPHandler, http_get) {

	smt::shared_ptr<webserv::HTTPRequest>  request;
	smt::shared_ptr<webserv::ServerBlock>  config;
	smt::shared_ptr<webserv::HTTPResponse> response;
	webserv::HTTPParser                    parser;

	config = smt::shared_ptr<webserv::ServerBlock>(new webserv::ServerBlock());
	config->m_root = "../webserv/website/";
	request = parser.getNextRequest(REQ_DIR);

	// testing get to directory with autoindex off
	config->m_autoidx = false;
	response = http_get(request, config);
	ASSERT_EQ(response->m_status, 404) << response->to_str();

	// testing get to file that doesnt exist
	request = parser.getNextRequest(REQ_FILE2);
	response = http_get(request, config);
	ASSERT_EQ(response->m_status, 404) << response->to_str();

	// testing get to directory with autoindex on
	config->m_autoidx = true;
	request = parser.getNextRequest(REQ_DIR);
	response = http_get(request, config);
	ASSERT_EQ(response->m_status, 200) << response->to_str();

	// testing get to file
	request = parser.getNextRequest(REQ_FILE1);
	response = http_get(request, config);
	ASSERT_EQ(response->m_status, 200) << response->to_str();
}
