#include "utils/string.hpp"

#include <gtest/gtest.h>

TEST(testString, testConstructors) {
    EXPECT_EQ(ft::string(), "");
    EXPECT_EQ(ft::string("Hello"), "Hello");
    EXPECT_EQ(ft::string(ft::string("test")), "test");
}

TEST(testString, testSplit) {
    ASSERT_EQ(ft::string("Hello World").split(" "),
              std::vector<ft::string>({"Hello", "World"}));

    ASSERT_EQ(ft::string("type: something").split(":"),
              std::vector<ft::string>({"type", " something"}));
    ASSERT_EQ(ft::string("type: something").split(": "),
              std::vector<ft::string>({"type", "something"}));

    ASSERT_EQ(ft::string(":type::: something: else:::").split(":"),
              std::vector<ft::string>({"type", " something", " else"}));
    ASSERT_EQ(ft::string(":type::: something: else:::").split(": "),
              std::vector<ft::string>({"type", "something", "else"}));
}

TEST(testString, testTrim) {
    ASSERT_EQ(ft::string("Hello World").trim(), "Hello World");
    ASSERT_EQ(ft::string(" Hello World").trim(), "Hello World");
    ASSERT_EQ(ft::string("Hello World ").trim(), "Hello World");
    ASSERT_EQ(ft::string(" Hello World ").trim(), "Hello World");
    ASSERT_EQ(ft::string(" \t\n\r\f\vHello World \t\n\r\f\v").trim(),
              "Hello World");
}