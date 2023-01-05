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
    EXPECT_EQ(1, res1->method);

    smt::shared_ptr<HTTPRequest> res2 = parser->parse_header(req2);
    ASSERT_NE(NULL, res2);
    EXPECT_EQ(2, res2->method);

    smt::shared_ptr<HTTPRequest> res3 = parser->parse_header(reqNoMethod);
    ASSERT_EQ(NULL, res3);
}

TEST_F(test_HTTPParser, getResource) {

    smt::shared_ptr<HTTPRequest> res1 = parser->parse_header(req1);
    ASSERT_NE(NULL, res1);
    EXPECT_STREQ("/hello.htm", res1->resource.c_str());

    smt::shared_ptr<HTTPRequest> res2 = parser->parse_header(req2);
    ASSERT_NE(NULL, res2);
    EXPECT_STREQ("/cgi-bin/process.cgi", res2->resource.c_str());

    smt::shared_ptr<HTTPRequest> res3 = parser->parse_header(reqNoPath);
    ASSERT_EQ(NULL, res3);
}

TEST_F(test_HTTPParser, getVersion) {

    smt::shared_ptr<HTTPRequest> res1 = parser->parse_header(req1);
    ASSERT_NE(NULL, res1);
    EXPECT_STREQ("HTTP/1.1", res1->version.c_str());

    smt::shared_ptr<HTTPRequest> res2 = parser->parse_header(req2);
    ASSERT_NE(NULL, res2);
    EXPECT_STREQ("HTTP/1.1", res2->version.c_str());

    smt::shared_ptr<HTTPRequest> res3 = parser->parse_header(req3);
    ASSERT_NE(NULL, res3);
    EXPECT_STREQ("HTTP/1.0", res3->version.c_str());

    smt::shared_ptr<HTTPRequest> res4 = parser->parse_header(reqNoVersion);
    ASSERT_EQ(NULL, res4);
}

TEST_F(test_HTTPParser, parse_header) {

    smt::shared_ptr<HTTPRequest> res1 = parser->parse_header(req1);
    ASSERT_NE(NULL, res1);
    EXPECT_EQ(1, res1->method);
    EXPECT_STREQ("/hello.htm", res1->resource.c_str());
    EXPECT_STREQ("HTTP/1.1", res1->version.c_str());

    ASSERT_TRUE(res1->headers.find("Host") != res1->headers.end());
    EXPECT_STREQ(res1->headers.find("Host")->second.c_str(), " www.tutorialspoint.com");
    ASSERT_TRUE(res1->headers.find("User-Agent") != res1->headers.end());
    EXPECT_STREQ(res1->headers.find("User-Agent")->second.c_str(), " Mozilla/4.0 (compatible; MSIE5.01; Windows NT)");
    ASSERT_TRUE(res1->headers.find("Accept-Language") != res1->headers.end());
    EXPECT_STREQ(res1->headers.find("Accept-Language")->second.c_str(), " en-us");
    ASSERT_TRUE(res1->headers.find("Accept-Encoding") != res1->headers.end());
    EXPECT_STREQ(res1->headers.find("Accept-Encoding")->second.c_str(), " gzip, deflate");
    ASSERT_TRUE(res1->headers.find("Connection") != res1->headers.end());
    EXPECT_STREQ(res1->headers.find("Connection")->second.c_str(), " Keep-Alive");


    smt::shared_ptr<HTTPRequest> res2 = parser->parse_header(req2);
    ASSERT_NE(NULL, res2);
    EXPECT_EQ(2, res2->method);
    EXPECT_STREQ("/cgi-bin/process.cgi", res2->resource.c_str());
    EXPECT_STREQ("HTTP/1.1", res2->version.c_str());

    ASSERT_TRUE(res2->headers.find("Host") != res2->headers.end());
    EXPECT_STREQ(res2->headers.find("Host")->second.c_str(), " www.tutorialspoint.com");
    ASSERT_TRUE(res2->headers.find("User-Agent") != res2->headers.end());
    EXPECT_STREQ(res2->headers.find("User-Agent")->second.c_str(), " Mozilla/4.0 (compatible; MSIE5.01; Windows NT)");
    ASSERT_TRUE(res2->headers.find("Content-Type") != res2->headers.end());
    EXPECT_STREQ(res2->headers.find("Content-Type")->second.c_str(), " text/xml; charset=utf-8");
    ASSERT_TRUE(res2->headers.find("Content-Length") != res2->headers.end());
    EXPECT_STREQ(res2->headers.find("Content-Length")->second.c_str(), " 96");
    ASSERT_TRUE(res2->headers.find("Accept-Language") != res2->headers.end());
    EXPECT_STREQ(res2->headers.find("Accept-Language")->second.c_str(), " en-us");
    ASSERT_TRUE(res2->headers.find("Accept-Encoding") != res2->headers.end());
    EXPECT_STREQ(res2->headers.find("Accept-Encoding")->second.c_str(), " gzip, deflate");
    ASSERT_TRUE(res2->headers.find("Connection") != res2->headers.end());
    EXPECT_STREQ(res2->headers.find("Connection")->second.c_str(), " Keep-Alive");

}