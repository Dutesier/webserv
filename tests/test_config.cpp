#include <gtest/gtest.h>
#include "ConfigSetup.hpp"
#include <cstdio>

class test_config : public ::testing::Test {

	public:
		void	SetUp(void) {
            setup = NULL;
            config = NULL;
        }
		void	TearDown(void) {
            if (config)
                delete config;
            if (setup)
                delete setup;
        }

	protected:
		ConfigSetup* setup;
        Config* config;

};

TEST_F(test_config, TestWithNoFile) {
    int argc = 1;
    char *argv[1] = {
        "webserv"
    };

    setup = new ConfigSetup(argc, argv);
    config = setup->get_config();

    ASSERT_TRUE(NULL == config);
}

TEST_F(test_config, TestWithFileDefault) {
    int argc = 1;
    char *argv[1] = {
        "webserv"
    };

    std::ofstream MyFile("/tmp/webserv/webserv.config");
    if (!MyFile.good())
        std::cout << "[ATTENTION HERE] Could not create file for testing, please run with sudo" << std::endl;
    MyFile.close();

    setup = new ConfigSetup(argc, argv);
    config = setup->get_config();


    EXPECT_FALSE(NULL == config) << "[ATTENTION HERE] Make sure you have a /tmp/webserv directory on your machine";
    remove("/tmp/webserv/webserv.config");
}

TEST_F(test_config, TestWithFileProvided) {
    int argc = 2;
    char *argv[2] = {
        "webserv",
        "local.config"
    };

    std::ofstream MyFile("local.config");
    MyFile.close();

    setup = new ConfigSetup(argc, argv);
    config = setup->get_config();


    EXPECT_FALSE(NULL == config);
    remove("local.config");
}

TEST_F(test_config, TestWithBasicFileProvided) {
    int argc = 2;
    char *argv[2] = {
        "webserv",
        "../tests/example_config_files/basic_file.config"
    };

    // std::ofstream MyFile("local.config");
    // MyFile.close();

    setup = new ConfigSetup(argc, argv);
    config = setup->get_config();

    EXPECT_FALSE(NULL == config);
    EXPECT_FALSE(config->location);
    EXPECT_EQ(443, config->port);
    // EXPECT_TRUE(config->server); TODO: Needs fix
    EXPECT_STREQ(config->root.c_str(), "/var/www/html");

}