#include "cgi/Argv.hpp"
#include "utils/ft_array.hpp"

#include <gtest/gtest.h>

TEST(testArgv, testSimple) {
    cgi::Argv argv({"hello", "world"});
    char**    c_argv = argv;
    EXPECT_STREQ(c_argv[0], "hello");
    EXPECT_STREQ(c_argv[1], "world");
    EXPECT_TRUE(!c_argv[2]);
    ft::array::erase(c_argv);
}

TEST(testArgv, testCopy) {
    cgi::Argv argv({"hello", "world"});
    cgi::Argv argv2(argv);
    char**    c_argv = argv2;
    EXPECT_STREQ(c_argv[0], "hello");
    EXPECT_STREQ(c_argv[1], "world");
    EXPECT_TRUE(!c_argv[2]);
    ft::array::erase(c_argv);
}