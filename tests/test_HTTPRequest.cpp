#include "HTTPRequest.hpp"

#include <gtest/gtest.h>

TEST(test_HTTPRequest, constructor) {
    webserv::HTTPRequest req(std::string("hello"));
}
