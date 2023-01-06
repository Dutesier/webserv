#include "HTTPParser.hpp"

#include <gtest/gtest.h>
#include <iostream>

class test_HTTPParser : public testing::Test {
    public:

        void SetUp(void) {
            this->parser = new HTTPParser();
        }

        void TearDown(void) { delete this->parser; }

    protected:

        HTTPParser* parser;
        std::string req1 = "GET /hello.htm HTTP/1.1\r\nUser-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)\r\nHost: www.tutorialspoint.com\r\nAccept-Language: en-us\r\nAccept-Encoding: gzip, deflate\r\nConnection: Keep-Alive\r\n\r\n";
        std::string req2 = "POST /cgi-bin/process.cgi HTTP/1.1\r\nUser-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)\r\nHost: www.tutorialspoint.com\r\nContent-Type: text/xml; charset=utf-8\r\nContent-Length: 96\r\nAccept-Language: en-us\r\nAccept-Encoding: gzip, deflate\r\nConnection: Keep-Alive\r\n\r\n<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n<string xmlns=\"http://clearforest.com/\">string</string>";
        std::string req3 = "GET /hello.htm HTTP/1.0\r\nUser-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)\r\nAccept-Language: en-us\r\nAccept-Encoding: gzip, deflate\nConnection: Keep-Alive\r\n\r\n";
        std::string reqNoMethod = "/hello.htm HTTP/1.1\r\nUser-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)\r\nHost: www.tutorialspoint.com\r\nAccept-Language: en-us\r\nAccept-Encoding: gzip, deflate\nConnection: Keep-Alive\r\n\r\n";
        std::string reqNoPath = "GET HTTP/1.1\r\nUser-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)\r\nHost: www.tutorialspoint.com\r\nAccept-Language: en-us\r\nAccept-Encoding: gzip, deflate\nConnection: Keep-Alive\r\n\r\n";
        std::string reqNoVersion = "GET /hello.htm \r\nUser-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)\r\nHost: www.tutorialspoint.com\r\nAccept-Language: en-us\r\nAccept-Encoding: gzip, deflate\nConnection: Keep-Alive\r\n\r\n";
        std::string reqCrazyFirstLine = "ajsadasdhaa a23\r\nGET /hello.htm \r\nUser-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)\r\nHost: www.tutorialspoint.com\r\nAccept-Language: en-us\r\nAccept-Encoding: gzip, deflate\nConnection: Keep-Alive\r\n\r\n";
};


TEST_F(test_HTTPParser, getMethod) {
    
    smt::shared_ptr<HTTPRequest> res1 = parser->parse_header(req1);
    ASSERT_NE(NULL, res1);
    EXPECT_EQ(1, res1->getMethod());

    smt::shared_ptr<HTTPRequest> res2 = parser->parse_header(req2);
    ASSERT_NE(NULL, res2);
    EXPECT_EQ(2, res2->getMethod());

    smt::shared_ptr<HTTPRequest> res3 = parser->parse_header(reqNoMethod);
    ASSERT_EQ(NULL, res3);
}

TEST_F(test_HTTPParser, getResource) {

    smt::shared_ptr<HTTPRequest> res1 = parser->parse_header(req1);
    ASSERT_NE(NULL, res1);
    EXPECT_STREQ("/hello.htm", res1->getResource().c_str());

    smt::shared_ptr<HTTPRequest> res2 = parser->parse_header(req2);
    ASSERT_NE(NULL, res2);
    EXPECT_STREQ("/cgi-bin/process.cgi", res2->getResource().c_str());

    smt::shared_ptr<HTTPRequest> res3 = parser->parse_header(reqNoPath);
    ASSERT_EQ(NULL, res3);
}

TEST_F(test_HTTPParser, getVersion) {

    smt::shared_ptr<HTTPRequest> res1 = parser->parse_header(req1);
    ASSERT_NE(NULL, res1);
    EXPECT_STREQ("HTTP/1.1", res1->getVersion().c_str());

    smt::shared_ptr<HTTPRequest> res2 = parser->parse_header(req2);
    ASSERT_NE(NULL, res2);
    EXPECT_STREQ("HTTP/1.1", res2->getVersion().c_str());

    smt::shared_ptr<HTTPRequest> res3 = parser->parse_header(req3);
    ASSERT_NE(NULL, res3);
    EXPECT_STREQ("HTTP/1.0", res3->getVersion().c_str());

    smt::shared_ptr<HTTPRequest> res4 = parser->parse_header(reqNoVersion);
    ASSERT_EQ(NULL, res4);
}

TEST_F(test_HTTPParser, parse_header) {

    smt::shared_ptr<HTTPRequest> res1 = parser->parse_header(req1);
    ASSERT_NE(NULL, res1);
    EXPECT_EQ(1, res1->getMethod());
    EXPECT_STREQ("/hello.htm", res1->getResource().c_str());
    EXPECT_STREQ("HTTP/1.1", res1->getVersion().c_str());

    ASSERT_FALSE(res1->getHeader("Host").empty());
    EXPECT_STREQ(res1->getHeader("Host").c_str(), " www.tutorialspoint.com");
    ASSERT_FALSE(res1->getHeader("User-Agent").empty());
    EXPECT_STREQ(res1->getHeader("User-Agent").c_str(), " Mozilla/4.0 (compatible; MSIE5.01; Windows NT)");
    ASSERT_FALSE(res1->getHeader("Accept-Language").empty());
    EXPECT_STREQ(res1->getHeader("Accept-Language").c_str(), " en-us");
    ASSERT_FALSE(res1->getHeader("Accept-Encoding").empty());
    EXPECT_STREQ(res1->getHeader("Accept-Encoding").c_str(), " gzip, deflate");
    ASSERT_FALSE(res1->getHeader("Connection").empty());
    EXPECT_STREQ(res1->getHeader("Connection").c_str(), " Keep-Alive");


    smt::shared_ptr<HTTPRequest> res2 = parser->parse_header(req2);
    ASSERT_NE(NULL, res2);
    EXPECT_EQ(2, res2->getMethod());
    EXPECT_STREQ("/cgi-bin/process.cgi", res2->getResource().c_str());
    EXPECT_STREQ("HTTP/1.1", res2->getVersion().c_str());

    ASSERT_FALSE(res2->getHeader("Host").empty());
    EXPECT_STREQ(res2->getHeader("Host").c_str(), " www.tutorialspoint.com");
    ASSERT_FALSE(res2->getHeader("User-Agent").empty());
    EXPECT_STREQ(res2->getHeader("User-Agent").c_str(), " Mozilla/4.0 (compatible; MSIE5.01; Windows NT)");
    ASSERT_FALSE(res2->getHeader("Content-Type").empty());
    EXPECT_STREQ(res2->getHeader("Content-Type").c_str(), " text/xml; charset=utf-8");
    ASSERT_FALSE(res2->getHeader("Content-Length").empty());
    EXPECT_STREQ(res2->getHeader("Content-Length").c_str(), " 96");
    ASSERT_FALSE(res2->getHeader("Accept-Language").empty());
    EXPECT_STREQ(res2->getHeader("Accept-Language").c_str(), " en-us");
    ASSERT_FALSE(res2->getHeader("Accept-Encoding").empty());
    EXPECT_STREQ(res2->getHeader("Accept-Encoding").c_str(), " gzip, deflate");
    ASSERT_FALSE(res2->getHeader("Connection").empty());
    EXPECT_STREQ(res2->getHeader("Connection").c_str(), " Keep-Alive");

}