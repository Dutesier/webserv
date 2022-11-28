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

TEST_F(test_Config, server_name) {
	this->impl->server.push_back(new webserv::ServerConfig());
	webserv::ServerConfig* server = this->impl->server.back();
    ASSERT_TRUE(server->server_name.empty());
	std::vector<std::string> cmd(1, "server_name");
    ASSERT_FALSE(this->impl->cmd_server_name(cmd));
	cmd.push_back("example.com");
	cmd.push_back("www.example.com");
    ASSERT_TRUE(this->impl->cmd_server_name(cmd));
	cmd.erase(cmd.begin());
	ASSERT_EQ(server->server_name, cmd);
}

TEST_F(test_Config, error_page) {
	this->impl->server.push_back(new webserv::ServerConfig());
	webserv::ServerConfig* server = this->impl->server.back();
    ASSERT_TRUE(server->error_page.empty());
	std::vector<std::string> cmd(1, "error_page");
    ASSERT_FALSE(this->impl->cmd_error_page(cmd));
	cmd.push_back("../webserv/error_pages/500.html");
    ASSERT_FALSE(this->impl->cmd_error_page(cmd));
	cmd.push_back("500");
    ASSERT_FALSE(this->impl->cmd_error_page(cmd));
	cmd[1] = "99";
    ASSERT_FALSE(this->impl->cmd_error_page(cmd));
	cmd[1] = "500";
	cmd[2] = "../webserv/error_pages/500.html";
    ASSERT_TRUE(this->impl->cmd_error_page(cmd));
	cmd[1] = "501";
	cmd[2] = "502";
	cmd.push_back("503");
	cmd.push_back("../webserv/error_pages/50X.html");
    ASSERT_TRUE(this->impl->cmd_error_page(cmd));
	std::map<int, std::string> error_page;
	error_page[500] = "../webserv/error_pages/500.html";
	error_page[501] = "../webserv/error_pages/50X.html";
	error_page[502] = "../webserv/error_pages/50X.html";
	error_page[503] = "../webserv/error_pages/50X.html";
	ASSERT_EQ(server->error_page, error_page);
}

TEST_F(test_Config, autoindex) {
	this->impl->server.push_back(new webserv::ServerConfig());
	webserv::ServerConfig* server = this->impl->server.back();
    ASSERT_FALSE(server->autoindex);
	std::vector<std::string> cmd(1, "autoindex");
    ASSERT_FALSE(this->impl->cmd_autoindex(cmd));
	cmd.push_back("of");
    ASSERT_FALSE(this->impl->cmd_autoindex(cmd));
	cmd[1] = "on";
	cmd.push_back("off");
    ASSERT_FALSE(this->impl->cmd_autoindex(cmd));
	cmd.pop_back();
    ASSERT_TRUE(this->impl->cmd_autoindex(cmd));
	cmd[1] = "off";
    ASSERT_FALSE(this->impl->cmd_autoindex(cmd));
}

TEST_F(test_Config, index) {
	this->impl->server.push_back(new webserv::ServerConfig());
	webserv::ServerConfig* server = this->impl->server.back();
	std::vector<std::string> index(1, "index");
	index.push_back("index.html");
	ASSERT_EQ(server->index, index);
	std::vector<std::string> cmd(1, "index");
    ASSERT_FALSE(this->impl->cmd_index(cmd));
	index[1] = "index.php";
	index.push_back("index.js");
	cmd.push_back("index.php");
	cmd.push_back("index.js");
    ASSERT_TRUE(this->impl->cmd_index(cmd));
	ASSERT_EQ(server->index, index);
}

TEST_F(test_Config, client_max_body_size) {
	this->impl->server.push_back(new webserv::ServerConfig());
	webserv::ServerConfig* server = this->impl->server.back();
    ASSERT_EQ(server->body_size, 8000);
	std::vector<std::string> cmd(1, "client_max_body_size");
    ASSERT_FALSE(this->impl->cmd_max_size(cmd));
	cmd.push_back("something");
    ASSERT_FALSE(this->impl->cmd_max_size(cmd));
	cmd[1] = "10";
    ASSERT_TRUE(this->impl->cmd_max_size(cmd));
    ASSERT_EQ(server->body_size, 10);
}

TEST_F(test_Config, lroot) {
	this->impl->server.back()->location.push_back(new webserv::LocationConfig("/php"));
	webserv::LocationConfig* location = this->impl->server.back()->location.back();
    ASSERT_EQ(location->root, "");
	std::vector<std::string> cmd(1, "root");
    ASSERT_FALSE(this->impl->cmd_lroot(cmd));
	cmd.push_back("/var/www/html");
    ASSERT_TRUE(this->impl->cmd_lroot(cmd));
	ASSERT_EQ(location->root, "/var/www/html");
}

TEST_F(test_Config, fastcgi_pass) {
	this->impl->server.back()->location.push_back(new webserv::LocationConfig("/php"));
	webserv::LocationConfig* location = this->impl->server.back()->location.back();
    ASSERT_EQ(location->fastcgi_pass, "");
	std::vector<std::string> cmd(1, "fastcgi_pass");
    ASSERT_FALSE(this->impl->cmd_fastcgi_pass(cmd));
	cmd.push_back("php-fpm");
    ASSERT_TRUE(this->impl->cmd_fastcgi_pass(cmd));
    ASSERT_EQ(location->fastcgi_pass, "php-fpm");
}

TEST_F(test_Config, request_method) {
	this->impl->server.back()->location.push_back(new webserv::LocationConfig("/php"));
	webserv::LocationConfig* location = this->impl->server.back()->location.back();
	std::vector<std::string> request_method( 3, "GET");
	request_method[1] = "POST";
	request_method[2] = "DELETE";
    ASSERT_EQ(location->request_method, request_method);
	std::vector<std::string> cmd(1, "request_method");
    ASSERT_FALSE(this->impl->cmd_request_method(cmd));
	cmd.push_back("SET");
    ASSERT_FALSE(this->impl->cmd_request_method(cmd));
	cmd[1] = "GET";
    ASSERT_TRUE(this->impl->cmd_request_method(cmd));
	request_method.pop_back();
	request_method.pop_back();
    ASSERT_EQ(location->request_method, request_method);
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
