#include "Error.hpp"

#include <gtest/gtest.h>
#include <iostream>
#include <string>

TEST(test_Error, constructor) {
    webserv::Error error("../webserv/default: failed to open",
                         webserv::Error::invalid_file);
}

TEST(test_Error, destructor) {
    webserv::Error* file_error = new webserv::Error(
        "../webserv/default: failed to open", webserv::Error::invalid_file);
    webserv::Error* syntax_error = new webserv::Error(
        "../webserv/default 12: listen 443;", webserv::Error::invalid_syntax);
    delete file_error;
    delete syntax_error;
}

TEST(test_Error, message) {
    webserv::Error file_error("../webserv/default: failed to open",
                              webserv::Error::invalid_file);
    webserv::Error syntax_error("../webserv/default 12: listen 443;",
                                webserv::Error::invalid_syntax);
    ASSERT_TRUE(file_error.message() ==
                "invalid_file: ../webserv/default: failed to open");
    ASSERT_TRUE(syntax_error.message() ==
                "invalid_syntax: ../webserv/default 12: listen 443;");
}
