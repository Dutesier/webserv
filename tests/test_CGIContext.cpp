#include <cgi/CGIContext.hpp>
#include <gtest/gtest.h>
#include <http/HTTPParser.hpp>
#include <http/HTTPRequest.hpp>

class test_CGIContext : public testing::Test {
    public:

        void SetUp(void) { this->parser = new HTTPParser(); }

        void TearDown(void) { delete this->parser; }

    protected:

        HTTPParser* parser;
        std::string req1 =
            "GET /cgi//myscript.py/path/to/script?param1=value1&param2=value2 "
            "HTTP/1.1\r\n"
            "Host: example.com\r\n"
            "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) "
            "AppleWebKit/537.36 (KHTML, like Gecko) Chrome/96.0.4664.45 "
            "Safari/537.36\r\n"
            "Accept: "
            "text/html,application/xhtml+xml,application/xml;q=0.9,image/"
            "avif,image/webp,image/apng,*/*;q=0.8,application/"
            "signed-exchange;v=b3;q=0.9\r\n"
            "Accept-Encoding: gzip, deflate, br\r\n"
            "Accept-Language: en-US,en;q=0.9\r\n"
            "Connection: keep-alive\r\n\r\n";
};

TEST_F(test_CGIContext, memb_var) {
    smt::shared_ptr<HTTPRequest> req = parser->getNextRequest(req1).first;

    // req->setResource("/home?key=value");
    // cgi::CGIContext ctx(req, "../webserv/website/cgi/test.py");
    // std::cout << ctx << std::endl;
    req->setResource(
        "/cgi-bin/myscript.cgi/path/to/script?param1=value1&param2=value2");
    cgi::CGIContext ctx(req, "../webserv/website/cgi/test_py");
    std::cout << ctx << std::endl;
}