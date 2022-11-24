#include "Config.hpp"

#include <gtest/gtest.h>
#include <iostream>

TEST(test_Config, constructor) {
    char*            argv[2] = {"webserv", "./default"};
    webserv::Config* config;
    ASSERT_THROW(config = new webserv::Config(2, argv),
                 webserv::Config::InvalidFileException);
    ASSERT_NO_THROW(config = new webserv::Config(1, argv));
    delete config;
}

TEST(test_Config, destructor) {
    char*            argv[2] = {"webserv", "./default"};
    webserv::Config* config;
    ASSERT_THROW(config = new webserv::Config(2, argv),
                 webserv::Config::InvalidFileException);
}

class test_ServerBlock : public ::testing::Test {
	public:
		void SetUp(void) {
			this->server_block = new webserv::Config::ServerBlock();
		}
		void TearDown(void) {
			delete this->server_block;
		}

	protected:
		webserv::Config::ServerBlock* server_block;
};

TEST_F(test_ServerBlock, constructor) {
	ASSERT_FALSE(this->server_block->autoindex);
	ASSERT_EQ(this->server_block->port, 80);
	ASSERT_EQ(this->server_block->host, "localhost");
	ASSERT_EQ(this->server_block->root, "/var/www/html");
	ASSERT_EQ(this->server_block->access_log, "../webserv/webserv.log");
	ASSERT_EQ(this->server_block->body_size, 8000);
	std::vector<std::string> index;
	index.push_back("index");
	index.push_back("index.html");
	ASSERT_EQ(this->server_block->index, index);
}

TEST_F(test_ServerBlock, destructor) {}

TEST_F(test_ServerBlock, listen) {
	ASSERT_EQ(this->server_block->port, 80);
	ASSERT_EQ(this->server_block->host, "localhost");
	ASSERT_FALSE(this->server_block->add_directive("listen 443"));
	ASSERT_FALSE(this->server_block->add_directive("listen ;"));
	ASSERT_FALSE(this->server_block->add_directive("listen 8080:localhost;"));
	ASSERT_EQ(this->server_block->port, 80);
	ASSERT_EQ(this->server_block->host, "localhost");
}
