#include "cgi/Envp.hpp"
#include "http/Request.hpp"
#include "utils/ft_array.hpp"

#include <gtest/gtest.h>

TEST(testEnvp, testBasic) {
    smt::shared_ptr<http::Request> req(
        new http::Request("POST "
                          "/cgi/myscript.py HTTP/1.1\r\n"
                          "Host: example.com\r\n"
                          "Accept-Encoding: gzip, deflate, br\r\n"
                          "Accept-Language: en-US,en;q=0.9\r\n"
                          "Connection: keep-alive\r\n\r\n"));
    cgi::Envp envp(req);
    char**    c_envp = envp;
    EXPECT_STREQ(c_envp[0], "REQUEST_METHOD=POST");
    EXPECT_STREQ(c_envp[1], "SCRIPT_NAME=/cgi/myscript.py");
    EXPECT_TRUE(!c_envp[2]);
    // delete[] c_envp;
    for (int i = 0; c_envp[i]; ++i) { delete[] c_envp[i]; }
    delete[] c_envp;
}

TEST(testEnvp, testMoreComplete) {
    smt::shared_ptr<http::Request> req(
        new http::Request("GET "
                          "/cgi/myscript.py?param1=value1&param2=value2 "
                          "HTTP/1.1\r\n"
                          "Host: example.com\r\n"
                          "Content-Type: text/html\r\n"
                          "Content-Length: 0\r\n"
                          "Accept-Encoding: gzip, deflate, br\r\n"
                          "Accept-Language: en-US,en;q=0.9\r\n"
                          "Connection: keep-alive\r\n\r\n"));
    cgi::Envp envp(req);
    char**    c_envp = envp;
    EXPECT_STREQ(c_envp[0], "REQUEST_METHOD=GET");
    EXPECT_STREQ(c_envp[1], "SCRIPT_NAME=/cgi/myscript.py");
    EXPECT_STREQ(c_envp[2], "QUERY_STRING=param1=value1&param2=value2");
    EXPECT_STREQ(c_envp[3], "CONTENT_LENGTH=0");
    EXPECT_STREQ(c_envp[4], "CONTENT_TYPE=text/html");
    EXPECT_TRUE(!c_envp[5]);
    for (int i = 0; c_envp[i]; ++i) { delete[] c_envp[i]; }
    delete[] c_envp;
}