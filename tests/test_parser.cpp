#include <gtest/gtest.h>
#include <iostream>
#include <vector>

#include "Fail.hpp"
#include "Parser.hpp"
#include "ConfigParser.hpp"

class test_parser : public ::testing::Test {

	public:
		void	SetUp(void) { this->parser = new ConfigParser; }
		void	TearDown(void) { delete this->parser; }
		void	Reset(void) {
			this->TearDown();
			commands.clear();
			this->SetUp();
		}

	protected:

		ConfigParser*	parser;
		std::vector<std::string> commands;

};

TEST_F(test_parser, split_line) {

	std::string	line = "	root /var/www/html;";

	std::vector<std::string> commands;
	commands.push_back("root");
	commands.push_back("/var/www/html;");

	ASSERT_EQ(this->parser->split_line(line), commands);
}

TEST_F(test_parser, listen_handler) {

	// testing with empty commmand
	ASSERT_FALSE(this->parser->listen_handler(commands));

	// testing inside wrong block
	this->Reset();
	commands.push_back("listen");
	commands.push_back("80;");
	ASSERT_FALSE(this->parser->listen_handler(commands));


	// testing with incomplete commmand
	this->Reset();
	this->parser->get_config()->server = true;
	commands.push_back("root;");
	ASSERT_FALSE(this->parser->listen_handler(commands));
	commands[0] = "listen;";
	ASSERT_FALSE(this->parser->listen_handler(commands));
	commands[0] = "listen";
	commands.push_back("655555");
	ASSERT_FALSE(this->parser->listen_handler(commands));

	// testing with invalid ports
	this->Reset();
	this->parser->get_config()->server = true;
	commands.push_back("listen");
	commands.push_back("address:r;");
	ASSERT_FALSE(this->parser->listen_handler(commands));
	this->Reset();
	this->parser->get_config()->server = true;
	commands.push_back("listen");
	commands.push_back("address:randstring;");
	ASSERT_FALSE(this->parser->listen_handler(commands));
	commands[1] = "655555;";
	ASSERT_FALSE(this->parser->listen_handler(commands));
	commands[1] = "address:655555;";
	ASSERT_FALSE(this->parser->listen_handler(commands));

	// testing successfull cases
	this->Reset();
	this->parser->get_config()->server = true;
	commands.push_back("listen");
	commands.push_back("443;");
	ASSERT_TRUE(this->parser->listen_handler(commands));
	ASSERT_EQ(this->parser->get_config()->port, 443);

	this->Reset();
	this->parser->get_config()->server = true;
	commands.push_back("listen");
	commands.push_back("8080;");
	ASSERT_TRUE(this->parser->listen_handler(commands));
	ASSERT_EQ(this->parser->get_config()->port, 8080);

	this->Reset();
	this->parser->get_config()->server = true;
	commands.push_back("listen");
	commands.push_back("example.com:8080;");
	ASSERT_TRUE(this->parser->listen_handler(commands));
	ASSERT_EQ(this->parser->get_config()->address, "example.com");
	ASSERT_EQ(this->parser->get_config()->port, 8080);

	// testing default value
	this->Reset();
	this->parser->get_config()->server = true;
	ASSERT_EQ(this->parser->get_config()->address, "localhost");
	ASSERT_EQ(this->parser->get_config()->port, 80);
}

TEST_F(test_parser, root_handler) {

	// testing with empty commmand
	ASSERT_FALSE(this->parser->root_handler(commands));

	// testing inside wrong block
	this->Reset();
	commands.push_back("root");
	commands.push_back("./folder;");
	ASSERT_FALSE(this->parser->root_handler(commands));

	// testing with incomplete commmand
	this->Reset();
	this->parser->get_config()->server = true;
	commands.push_back("root;");
	ASSERT_FALSE(this->parser->root_handler(commands));
	commands[0] = "listen;";
	ASSERT_FALSE(this->parser->root_handler(commands));
	commands[0] = "root";
	commands.push_back("/var/www/html");
	ASSERT_FALSE(this->parser->root_handler(commands));

	// testing successfull cases
	this->Reset();
	this->parser->get_config()->server = true;
	commands.push_back("root");
	commands.push_back("/var/www/html;");
	ASSERT_TRUE(this->parser->root_handler(commands));
	ASSERT_EQ(this->parser->get_config()->root, "/var/www/html");

	this->Reset();
	this->parser->get_config()->server = true;
	commands.push_back("root");
	commands.push_back("test;");
	ASSERT_TRUE(this->parser->root_handler(commands));
	ASSERT_EQ(this->parser->get_config()->root, "test");


	this->Reset();
	this->parser->get_config()->server = true;
	commands.push_back("root");
	commands.push_back("./folder;");
	ASSERT_TRUE(this->parser->root_handler(commands));
	ASSERT_EQ(this->parser->get_config()->root, "./folder");

	// testing default value
	this->Reset();
	this->parser->get_config()->server = true;
	ASSERT_EQ(this->parser->get_config()->root, "/var/www/html");
}
