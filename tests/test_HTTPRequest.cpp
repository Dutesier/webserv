#include "http/HTTPRequest.hpp"

#include <gtest/gtest.h>

TEST(test_HTTPRequest, constructor) {
    HTTPRequest req(std::string("hello"));
}
