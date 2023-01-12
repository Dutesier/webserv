#include "http/HTTPHandler.hpp"
#include "http/HTTPResponse.hpp"

#include <gtest/gtest.h>

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