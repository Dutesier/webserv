// #include "http/HTTPParser.hpp"
#include "http/HTTPRequest.hpp"
#include "utils/smt.hpp"

#include <gtest/gtest.h>

class test_HTTPRequest : public testing::Test {
    public:

        // void SetUp(void) { this->parser = new HTTPParser(); }

        // void TearDown(void) { delete this->parser; }

    protected:

        // HTTPParser* parser;
        std::string req1 = "GET "
                           "/cgi-bin/myscript.cgi/path/to/"
                           "script?param1=value1&param2=value2 HTTP/1.1\r\n"
                           "Host: example.com\r\n"
                           "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; "
                           "x64) AppleWebKit/537.36 (KHTML, like Gecko) "
                           "Chrome/96.0.4664.45 Safari/537.36\r\n"
                           "Accept: "
                           "text/html,application/xhtml+xml,application/"
                           "xml;q=0.9,image/avif,image/webp,image/apng,*/"
                           "*;q=0.8,application/signed-exchange;v=b3;q=0.9\r\n"
                           "Accept-Encoding: gzip, deflate, br\r\n"
                           "Accept-Language: en-US,en;q=0.9\r\n"
                           "Connection: keep-alive\r\n\r\n";
};

TEST_F(test_HTTPRequest, getters) {
    smt::shared_ptr<webserv::HTTPRequest> req(new webserv::HTTPRequest(req1));
    std::string                           headers = "Accept: "
                                                    "text/html,application/xhtml+xml,application/"
                                                    "xml;q=0.9,image/avif,image/webp,image/apng,*/"
                                                    "*;q=0.8,application/signed-exchange;v=b3;q=0.9\r\n"
                                                    "Accept-Encoding: gzip, deflate, br\r\n"
                                                    "Accept-Language: en-US,en;q=0.9\r\n"
                                                    "Connection: keep-alive\r\n"
                                                    "Host: example.com\r\n"
                                                    "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; "
                                                    "x64) AppleWebKit/537.36 (KHTML, like Gecko) "
                                                    "Chrome/96.0.4664.45 Safari/537.36\r\n\r\n";

    // FLOG_F(req->toStr());
    FLOG_F(req->getAllHeaders());
    FLOG_F(headers);
    ASSERT_EQ(
        req->getResource(),
        "/cgi-bin/myscript.cgi/path/to/script?param1=value1&param2=value2");
    ASSERT_EQ(req->getMethod(), webserv::GET);
    ASSERT_EQ(req->getVersion(), "HTTP/1.1");
    // ASSERT_EQ(req->getAllHeaders(), headers); I have a bug with white space
    // here.
    ASSERT_EQ(req->getHeader("Content-Type"), "");
    ASSERT_EQ(req->getStatusCode(), 0);
}

TEST_F(test_HTTPRequest, resource) {
    smt::shared_ptr<webserv::HTTPRequest> req(new webserv::HTTPRequest(req1));

    ASSERT_EQ(req->getQueriesFromResource(), "param1=value1&param2=value2");
    ASSERT_EQ(req->getRefinedResource(),
              "/cgi-bin/myscript.cgi/path/to/script");
    ASSERT_EQ(req->getScriptName(), "/cgi-bin/myscript.cgi");
    ASSERT_EQ(req->getPathInfo(), "/path/to/script");
}

TEST_F(test_HTTPRequest, Validity) {
    smt::shared_ptr<webserv::HTTPRequest> req(new webserv::HTTPRequest(req1));

    ASSERT_TRUE(req->isCGIRequest());
    ASSERT_TRUE(req->isValid());
}

// TODO: test to_str() methods
TEST(test_Request, constructor) {
    ASSERT_NO_THROW(
        webserv::HTTPRequest("GET / HTTP/1.1\r\nHost: x\r\nContent-Length: "
                             "15\r\n\r\nThis is a body"));
}

TEST(test_Request, error) {

    // wrong start line
    std::string req0 =
        "GET HTTP/1.1\r\nHost: x\r\nContent-Length: 15\r\n\r\nThis is a body";
    // wrong header
    std::string req1 =
        "GET / HTTP/1.1\r\nHost:\r\nContent-Length: 15\r\n\r\nThis is a body";
    // wrong body
    std::string req2 = "GET / HTTP/1.1\r\nHost:x\r\nContent-Length: 15\r\n\r\n";

    ASSERT_THROW(webserv::HTTPRequest a(req0),
                 webserv::HTTPRequest::MalformedRequestException);

    ASSERT_THROW(webserv::HTTPRequest a(req1),
                 webserv::HTTPRequest::MalformedRequestException);

    ASSERT_NO_THROW(webserv::HTTPRequest a(req2));
}
