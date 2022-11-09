#include <gtest/gtest.h>
#include <cstdio>

#include "Socket.hpp"
#include "SocketAddress.hpp"
#include "SocketListener.hpp"

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
	ASSERT_TRUE(this->sock->bind()) << errno;
}

TEST_F(test_SocketListener, listen) {

	ASSERT_TRUE(this->sock->bind()) << errno;
	ASSERT_TRUE(this->sock->listen()) << errno;
}

TEST_F(test_SocketListener, close) {

	ASSERT_TRUE(this->sock->close()) << errno;
	ASSERT_EQ(this->sock->sockfd(), -1);

	ASSERT_FALSE(this->sock->bind()) << errno;
	ASSERT_FALSE(this->sock->listen()) << errno;
	ASSERT_FALSE(this->sock->close()) << errno;
}

TEST_F(test_SocketListener, shutdown) {

	ASSERT_TRUE(this->sock->bind()) << errno;
	ASSERT_TRUE(this->sock->listen()) << errno;
	ASSERT_TRUE(this->sock->shutdown(0)) << errno;
	ASSERT_FALSE(this->sock->bind()) << errno;
}

TEST_F(test_SocketListener, recv) {}

TEST_F(test_SocketListener, send) {}
