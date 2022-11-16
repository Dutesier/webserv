#include <gtest/gtest.h>
#include <cstdio>

#include "Socket.hpp"
#include "SocketAddress.hpp"
#include "SocketListener.hpp"
#include "SocketConnection.hpp"

class test_SocketConnection : public ::testing::Test {

	public:

		void SetUp(void) { this->sock = new webserv::SocketListener(8080); }
		void TearDown(void) { delete this->sock; }

	protected:

		webserv::SocketListener*	sock;
};

TEST_F(test_SocketConnection, constructor) {}

TEST_F(test_SocketConnection, destructor) {}

