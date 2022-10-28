
#include <gtest/gtest.h>
#include <iostream>
#include <vector>

#include "Error.hpp"
#include "Parser.hpp"

class test_parser : public ::testing::Test {

	public:
		void	SetUp() { this->parser = new Parser(); }
		void	TearDown() { if (this->parser) delete this->parser; };
		void	Reset() { this->TearDown(); this->SetUp(); }

	protected:
		Parser*	parser;
};

TEST_F(test_parser, test_listen_directive) {

	std::vector<std::string>	command;

	// Testing with empty command
	EXPECT_FALSE(this->parser->listen_handler(command));
	EXPECT_NE(this->parser->get_error(), nullptr);

	// Testing with wrong command
	this->Reset();
	command.push_back("root");

	EXPECT_FALSE(this->parser->listen_handler(command));
	EXPECT_NE(this->parser->get_error(), nullptr);

	// Testing with wrong number of arguments command
	this->Reset();
	command[0] = "listen";

	EXPECT_FALSE(this->parser->listen_handler(command));
	EXPECT_NE(this->parser->get_error(), nullptr);

	// Testing with wrong arguments command
	command.push_back("wrong");

	EXPECT_FALSE(this->parser->listen_handler(command));
	EXPECT_NE(this->parser->get_error(), nullptr);

	command[1] = "-1";

	EXPECT_FALSE(this->parser->listen_handler(command));
	EXPECT_NE(this->parser->get_error(), nullptr);

	// Testing successfull case
	this->Reset();
	command[1] = "443";

	EXPECT_TRUE(this->parser->listen_handler(command));
	EXPECT_EQ(this->parser->get_error(), nullptr);
	EXPECT_EQ(this->parser->get_listen(), 443);

	// Testing default value
	this->Reset();
	EXPECT_EQ(this->parser->get_listen(), 80);
}


TEST_F(test_parser, test_root_directive) {

	std::vector<std::string>	command;

	// Testing with empty command;
	EXPECT_FALSE(this->parser->root_handler(command));
	EXPECT_NE(this->parser->get_error(), nullptr);

	// Testing with wrong command
	this->Reset();
	command.push_back("wrong");
	EXPECT_FALSE(this->parser->root_handler(command));
	EXPECT_NE(this->parser->get_error(), nullptr);
	
	// Testing with wrong number of commands
	this->Reset();
	command[0] = "root";
	EXPECT_FALSE(this->parser->root_handler(command));
	EXPECT_NE(this->parser->get_error(), nullptr);
	
	// Testing with successful command
	this->Reset();
	command.push_back("html");
	EXPECT_TRUE(this->parser->root_handler(command));
	EXPECT_EQ(this->parser->get_error(), nullptr); 
}
