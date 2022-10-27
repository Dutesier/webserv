
#include <gtest/gtest.h>
#include <iostream>
#include <vector>

#include "Error.hpp"
#include "Parser.hpp"

class test_parser : public ::testing::Test {

	public:
		void	SetUp() {};
		void	TearDown() {};
		void	Reset() {
			this->parser.set_listen(0);
			this->parser.set_error(nullptr);
		}
	protected:
		Parser	parser;
};

TEST_F(test_parser, test_listen_directive) {

	std::vector<std::string>	command;

	// Testing with empty command
	this->parser.listen_handler(command);
	EXPECT_NE(this->parser.get_error(), nullptr);

	// Testing with wrong command
	this->Reset();
	command.push_back("root");
	this->parser.listen_handler(command);
	EXPECT_NE(this->parser.get_error(), nullptr);

	// Testing with wrong number of arguments command
	this->Reset();
	command[0] = "listen";
	this->parser.listen_handler(command);
	EXPECT_NE(this->parser.get_error(), nullptr);

	// Testing with wrong arguments command
	command.push_back("wrong");
	this->parser.listen_handler(command);

	// Testing successfull case
	this->Reset();
	command[1] = "443";
	this->parser.listen_handler(command);
	EXPECT_EQ(this->parser.get_error(), nullptr);
	EXPECT_EQ(this->parser.get_listen(), 443);

	command.push_back("-1");
}
