#include <gtest/gtest.h>
#include "ConfigSetup.hpp"
#include <cstdio>


TEST(ConfigTests, TestWithNoFile) {
    int argc = 1;
    char *argv[1] = {
        "webserv"
    };

    ConfigSetup setup(argc, argv);

    ASSERT_TRUE(NULL == setup.get_config());
}

TEST(ConfigTests, TestWithFileDefault) {
    int argc = 1;
    char *argv[1] = {
        "webserv"
    };

    std::ofstream MyFile("/tmp/webserv/webserv.config");
    if (!MyFile.good())
        std::cout << "[ATTENTION HERE] Could not create file for testing, please run with sudo" << std::endl;
    MyFile.close();

    ConfigSetup setup(argc, argv);

    EXPECT_FALSE(NULL == setup.get_config()) << "[ATTENTION HERE] Make sure you have a /tmp/webserv directory on your machine";
    remove("/tmp/webserv/webserv.config");
}

TEST(ConfigTests, TestWithFileProvided) {
    int argc = 2;
    char *argv[2] = {
        "webserv",
        "local.config"
    };

    std::ofstream MyFile("local.config");
    MyFile.close();

    ConfigSetup setup(argc, argv);

    EXPECT_FALSE(NULL == setup.get_config());
    remove("local.config");
}