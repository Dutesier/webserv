
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

	protected:

		Parser*	parser;
};

TEST_F( test_parser, split_line) {

	std::string	line = "	root /var/www/html;";

	std::vector<std::string> commands;
	commands.push_back("root");
	commands.push_back("/var/www/html;");

	ASSERT_EQ(this->parser->split_line(line), commands);
}
