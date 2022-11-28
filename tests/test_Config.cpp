#define GTEST_TESTING 1

#include "Config.hpp"
#include "ServerConfig.hpp"

#include <gtest/gtest.h>
#include <iostream>
#include <vector>

class test_Config : public ::testing::Test {
    public:

        void SetUp(void) {
            char* argv[1] = {"webserv"};
            this->impl = new webserv::Config::impl(1, argv);
        }

        void TearDown(void) { delete this->impl; }

    protected:

        webserv::Config::impl* impl;
};

TEST_F(test_Config, constructor) {
	char*	argv[1] = {"webserv"};
	webserv::Config a(1, argv);
}

TEST_F(test_Config, destructor) {
	char*	argv[1] = {"webserv"};
	webserv::Config* a = new webserv::Config(1, argv);
	delete a;
}

TEST_F(test_Config, server_cmd) {
	ASSERT_FALSE(this->impl->server_cmd("\tlisten 80"));
	ASSERT_FALSE(this->impl->server_cmd("\tserver server_name;"));
	ASSERT_TRUE(this->impl->server_cmd("\tlisten example.com:8080;"));
	webserv::ServerConfig* server = this->impl->server.back();
	ASSERT_EQ(server->port, 8080);
	ASSERT_EQ(server->host, "example.com");
}

TEST_F(test_Config, location_cmd) {
	ASSERT_FALSE(this->impl->location_cmd("\tfastcgi_pass unix"));
	ASSERT_FALSE(this->impl->location_cmd("\trot /var/www/html"));
	ASSERT_TRUE(this->impl->location_cmd("\troot /html;"));
	webserv::LocationConfig* location = this->impl->server.back()->location.back();
	ASSERT_EQ(location->root, "/html");
}

TEST_F(test_Config, cmd_listen) {
	this->impl->server.push_back(new webserv::ServerConfig());
	webserv::ServerConfig* server = this->impl->server.back();
    ASSERT_EQ(server->port, 80);
    ASSERT_EQ(server->host, "localhost");
	std::vector<std::string> cmd(1, "listen");
    ASSERT_FALSE(this->impl->cmd_listen(cmd));
	cmd.push_back("80:example");
    ASSERT_FALSE(this->impl->cmd_listen(cmd));
	cmd[1] = "example:example";
    ASSERT_FALSE(this->impl->cmd_listen(cmd));
	cmd[1] = "10000000";
    ASSERT_FALSE(this->impl->cmd_listen(cmd));
	cmd[1] = "8080";
    ASSERT_TRUE(this->impl->cmd_listen(cmd));
	ASSERT_EQ(server->port, 8080);
	cmd[1] = "example.com:443";
	ASSERT_TRUE(this->impl->cmd_listen(cmd));
	ASSERT_EQ(server->port, 443);
	ASSERT_EQ(server->host, "example.com");
}

TEST_F(test_Config, cmd_root) {
	this->impl->server.push_back(new webserv::ServerConfig());
	webserv::ServerConfig* server = this->impl->server.back();
    ASSERT_EQ(server->root, "/var/www/html");
	std::vector<std::string> cmd(1, "root");
    ASSERT_FALSE(this->impl->cmd_root(cmd));
	cmd.push_back("/var/www/html");
    ASSERT_TRUE(this->impl->cmd_root(cmd));
	ASSERT_EQ(server->root, "/var/www/html");
}

TEST_F(test_Config, InvalidFileException) {
	char*	argv[2] = {"webserv", "default1"};
	ASSERT_THROW(new webserv::Config(2, argv),
			  webserv::Config::impl::InvalidFileException);
	argv[1] = "./default";
	ASSERT_THROW(new webserv::Config(2, argv),
			  webserv::Config::impl::InvalidFileException);
}

TEST_F(test_Config, InvalidSyntaxException) {
	char*	argv[2] = {"webserv", "../webserv/test_files/fail_autoindex0"};
	ASSERT_THROW(new webserv::Config(2, argv),
			  webserv::Config::impl::InvalidSyntaxException);
	argv[1] = "../webserv/test_files/fail_autoindex1";
	ASSERT_THROW(new webserv::Config(2, argv),
			  webserv::Config::impl::InvalidSyntaxException);
	argv[1] = "../webserv/test_files/fail_autoindex2";
	ASSERT_THROW(new webserv::Config(2, argv),
			  webserv::Config::impl::InvalidSyntaxException);
	argv[1] = "../webserv/test_files/fail_comma0";
	ASSERT_THROW(new webserv::Config(2, argv),
			  webserv::Config::impl::InvalidSyntaxException);
	argv[1] = "../webserv/test_files/fail_comma1";
	ASSERT_THROW(new webserv::Config(2, argv),
			  webserv::Config::impl::InvalidSyntaxException);
	argv[1] = "../webserv/test_files/fail_comma2";
	ASSERT_THROW(new webserv::Config(2, argv),
			  webserv::Config::impl::InvalidSyntaxException);
	argv[1] = "../webserv/test_files/fail_comma0";
	ASSERT_THROW(new webserv::Config(2, argv),
			  webserv::Config::impl::InvalidSyntaxException);
	argv[1] = "../webserv/test_files/fail_command1";
	ASSERT_THROW(new webserv::Config(2, argv),
			  webserv::Config::impl::InvalidSyntaxException);
	argv[1] = "../webserv/test_files/fail_error_page0";
	ASSERT_THROW(new webserv::Config(2, argv),
			  webserv::Config::impl::InvalidSyntaxException);
	argv[1] = "../webserv/test_files/fail_error_page1";
	ASSERT_THROW(new webserv::Config(2, argv),
			  webserv::Config::impl::InvalidSyntaxException);
	argv[1] = "../webserv/test_files/fail_error_page2";
	ASSERT_THROW(new webserv::Config(2, argv),
			  webserv::Config::impl::InvalidSyntaxException);
	argv[1] = "../webserv/test_files/fail_index0";
	ASSERT_THROW(new webserv::Config(2, argv),
			  webserv::Config::impl::InvalidSyntaxException);
	argv[1] = "../webserv/test_files/fail_listen0";
	ASSERT_THROW(new webserv::Config(2, argv),
			  webserv::Config::impl::InvalidSyntaxException);
	argv[1] = "../webserv/test_files/fail_listen1";
	ASSERT_THROW(new webserv::Config(2, argv),
			  webserv::Config::impl::InvalidSyntaxException);
	argv[1] = "../webserv/test_files/fail_listen2";
	ASSERT_THROW(new webserv::Config(2, argv),
			  webserv::Config::impl::InvalidSyntaxException);
	argv[1] = "../webserv/test_files/fail_max_size0";
	ASSERT_THROW(new webserv::Config(2, argv),
			  webserv::Config::impl::InvalidSyntaxException);
	argv[1] = "../webserv/test_files/fail_max_size1";
	ASSERT_THROW(new webserv::Config(2, argv),
			  webserv::Config::impl::InvalidSyntaxException);
	argv[1] = "../webserv/test_files/fail_root0";
	ASSERT_THROW(new webserv::Config(2, argv),
			  webserv::Config::impl::InvalidSyntaxException);
	argv[1] = "../webserv/test_files/fail_server0";
	ASSERT_THROW(new webserv::Config(2, argv),
			  webserv::Config::impl::InvalidSyntaxException);
	argv[1] = "../webserv/test_files/fail_server1";
	ASSERT_THROW(new webserv::Config(2, argv),
			  webserv::Config::impl::InvalidSyntaxException);
	argv[1] = "../webserv/test_files/fail_server_name0";
	ASSERT_THROW(new webserv::Config(2, argv),
			  webserv::Config::impl::InvalidSyntaxException);
}
