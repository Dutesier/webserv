#include "http/HTTPResponse.hpp"

#include <gtest/gtest.h>

TEST(test_HTTPResponse, constructor) {

    ASSERT_NO_THROW(
        webserv::HTTPResponse(404, std::map<std::string, std::string>()));
    ASSERT_NO_THROW(webserv::HTTPResponse(
        200, std::map<std::string, std::string>(), "Hello"));
    ASSERT_NO_THROW(webserv::HTTPResponse(
        404, std::map<std::string, std::string>(), "a", "b"));
}

TEST(test_HTTPResponse, destructor) {

    webserv::HTTPResponse* a;
    ASSERT_NO_THROW(a = new webserv::HTTPResponse(
                        404, std::map<std::string, std::string>()));
    ASSERT_NO_THROW(delete a);
}

TEST(test_HTTPResponse, to_str) {

    // testing without body
    std::string resp1 = "HTTP/1.1 200 OK\r\nDuarte: gaja\r\nMaria: "
                        "passivo-agressiva\r\nZe: gaja\r\n\r\n";
    std::map<std::string, std::string> header;
    header["Maria"] = "passivo-agressiva";
    header["Ze"] = "gaja";
    header["Duarte"] = "gaja";
    webserv::HTTPResponse* a;
    ASSERT_NO_THROW(a = new webserv::HTTPResponse(200, header, ""));
    ASSERT_EQ(a->to_str(), resp1);
    ASSERT_NO_THROW(delete a);

    // testing with body
    std::string            resp2 = "HTTP/1.1 200 OK\r\nDuarte: gaja\r\nMaria: "
                                   "passivo-agressiva\r\nZe: gaja\r\n\r\nDepois disto a "
                                   "Maria decidiu que ja nao se vai dar mais com gajas";
    webserv::HTTPResponse* b;
    ASSERT_NO_THROW(
        b = new webserv::HTTPResponse(200, header,
                                      "Depois disto a Maria decidiu que ja nao "
                                      "se vai dar mais com gajas"));
    ASSERT_EQ(b->to_str(), resp2);
    ASSERT_NO_THROW(delete b);
}