#include "http/HTTPParser.hpp"
#include "http/HTTPRequest.hpp"

#include <gtest/gtest.h>

class test_HTTPRequest : public testing::Test {
    public:

        void SetUp(void) { this->parser = new HTTPParser(); }

        void TearDown(void) { delete this->parser; }

    protected:

        HTTPParser* parser;
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

TEST_F(test_HTTPRequest, headers) {
    smt::shared_ptr<HTTPRequest> req = parser->getNextRequest(req1).first;
    std::string                  headers = "Accept: "
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

    // ASSERT_EQ(req->getAllHeaders(), headers);
    ASSERT_EQ(req->getHeader("Content-Type"), "");
    ASSERT_EQ(req->getHeader("Accept"),
              "text/html,application/xhtml+xml,application/"
              "xml;q=0.9,image/avif,image/webp,image/apng,*/"
              "*;q=0.8,application/signed-exchange;v=b3;q=0.9");
    ASSERT_EQ(req->getHeader("Accept-Encoding"), "gzip, deflate, br");
    ASSERT_EQ(req->getHeader("Accept-Language"), "en-US,en;q=0.9");
    ASSERT_EQ(req->getHeader("Connection"), "keep-alive");
    ASSERT_EQ(req->getHeader("Host"), "example.com");
    ASSERT_EQ(req->getHeader("User-Agent"),
              "Mozilla/5.0 (Windows NT 10.0; Win64; "
              "x64) AppleWebKit/537.36 (KHTML, like Gecko) "
              "Chrome/96.0.4664.45 Safari/537.36");
}

TEST_F(test_HTTPRequest, getters) {
    smt::shared_ptr<HTTPRequest> req = parser->getNextRequest(req1).first;
    std::string                  headers = "Accept: "
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

    ASSERT_EQ(
        req->getResource(),
        "/cgi-bin/myscript.cgi/path/to/script?param1=value1&param2=value2");
    ASSERT_EQ(req->getMethod(), webserv::GET);
    ASSERT_EQ(req->getVersion(), "HTTP/1.1");
    // ASSERT_EQ(req->getAllHeaders(), headers);
    ASSERT_EQ(req->getHeader("Content-Type"), "");
    ASSERT_EQ(req->getStatusCode(), 0);
}

TEST_F(test_HTTPRequest, resource) {
    smt::shared_ptr<HTTPRequest> req = parser->getNextRequest(req1).first;

    ASSERT_EQ(req->getRefinedResource(),
              "/cgi-bin/myscript.cgi/path/to/script");
}

TEST_F(test_HTTPRequest, Validity) {
    smt::shared_ptr<HTTPRequest> req = parser->getNextRequest(req1).first;

    ASSERT_TRUE(req->isCGIRequest());
    ASSERT_TRUE(req->isValid());
}