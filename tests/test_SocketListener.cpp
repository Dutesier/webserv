#include <gtest/gtest.h>
#include "Socket.hpp"
#include "SocketAddress.hpp"
#include "SocketListener.hpp"
#include <cstdio>

class test_SocketListener : public ::testing::Test {

	public:

		void SetUp(void) { this->sock = new webserv::SocketListener(8080); }
		void TearDown(void) { delete this->sock; }

	protected:

		webserv::SocketListener*	sock;
};

TEST_F(test_SocketListener, constructor) {}

TEST_F(test_SocketListener, destructor) {}

TEST_F(test_SocketListener, bind) {
	EXPECT_TRUE(this->sock->bind());
	perror("bind");
	std::cout << errno << std::endl;
}
