#include "http/methods.hpp"

#include <gtest/gtest.h>

TEST(testConvert, testConvertMethod) {
    ASSERT_EQ(http::convertMethod("GET"), http::MethodType::GET);
    ASSERT_EQ(http::convertMethod("HEAD"), http::MethodType::HEAD);
    ASSERT_EQ(http::convertMethod("POST"), http::MethodType::POST);
    ASSERT_EQ(http::convertMethod("PUT"), http::MethodType::PUT);
    ASSERT_EQ(http::convertMethod("DELETE"), http::MethodType::DELETE);
    ASSERT_EQ(http::convertMethod("CONNECT"), http::MethodType::CONNECT);
    ASSERT_EQ(http::convertMethod("OPTIONS"), http::MethodType::OPTIONS);
    ASSERT_EQ(http::convertMethod("TRACE"), http::MethodType::TRACE);
    ASSERT_EQ(http::convertMethod("PATCH"), http::MethodType::PATCH);
    ASSERT_EQ(http::convertMethod("INVALID"), http::MethodType::INVALID);
    ASSERT_EQ(http::convertMethod("other"), http::MethodType::UNKNOWN_METHOD);
}

TEST(testConvert, testConvertVersion) {
    ASSERT_EQ(http::convertVersion("HTTP/1.1"), http::Version::HTTP_1_1);
    ASSERT_EQ(http::convertVersion("HTTP/1.0"), http::Version::UNKNOWN_VERSION);
    ASSERT_EQ(http::convertVersion("HTTP/2.0"), http::Version::UNKNOWN_VERSION);
}