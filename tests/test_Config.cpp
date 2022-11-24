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
	ASSERT_FALSE(this->server_block->add_directive("listen 111111111"));
	ASSERT_EQ(this->server_block->port, 80);
	ASSERT_EQ(this->server_block->host, "localhost");
	ASSERT_TRUE(this->server_block->add_directive("listen www.example.com;"));
	ASSERT_EQ(this->server_block->port, 80);
	ASSERT_EQ(this->server_block->host, "www.example.com");
	ASSERT_TRUE(this->server_block->add_directive("listen example.com:8080;"));
	ASSERT_EQ(this->server_block->port, 8080);
	ASSERT_EQ(this->server_block->host, "example.com");
}

TEST_F(test_ServerBlock, root) {
	ASSERT_EQ(this->server_block->root, "/var/www/html");
	ASSERT_FALSE(this->server_block->add_directive("root /var/www/"));
	ASSERT_FALSE(this->server_block->add_directive("root ;"));
	ASSERT_EQ(this->server_block->root, "/var/www/html");
	ASSERT_TRUE(this->server_block->add_directive("root /var/www/;"));
	ASSERT_EQ(this->server_block->root, "/var/www/");
}

TEST_F(test_ServerBlock, server_name) {
	ASSERT_TRUE(this->server_block->server_name.empty());
	ASSERT_FALSE(this->server_block->add_directive("server_name ;"));
	ASSERT_FALSE(this->server_block->add_directive("server_name "));
	ASSERT_TRUE(this->server_block->server_name.empty());
	ASSERT_TRUE(this->server_block->add_directive("server_name example.com;"));
	ASSERT_TRUE(this->server_block->server_name.size() == 1);
	ASSERT_EQ(this->server_block->server_name[0], "example.com");
}

TEST_F(test_ServerBlock, error_page) {
	ASSERT_TRUE(this->server_block->error_page.empty());
	ASSERT_FALSE(this->server_block->add_directive("error_page ;"));
	ASSERT_FALSE(this->server_block->add_directive("error_page 500.html;"));
	ASSERT_FALSE(this->server_block->add_directive("error_page 500.html"));
	ASSERT_TRUE(this->server_block->error_page.empty());
	ASSERT_TRUE(this->server_block->add_directive("error_page 500 ../webserv/error_pages/500.html;"));
	ASSERT_TRUE(this->server_block->error_page.size() == 1);
	ASSERT_EQ(this->server_block->error_page[500], "../webserv/error_pages/500.html");
	ASSERT_TRUE(this->server_block->add_directive("error_page 500 501 502 ../webserv/error_pages/50X.html;"));
	ASSERT_EQ(this->server_block->error_page[500], "../webserv/error_pages/50X.html");
	ASSERT_EQ(this->server_block->error_page[501], "../webserv/error_pages/50X.html");
	ASSERT_EQ(this->server_block->error_page[502], "../webserv/error_pages/50X.html");
	ASSERT_TRUE(this->server_block->error_page.size() == 3);
}
