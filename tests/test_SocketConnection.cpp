#include <gtest/gtest.h>
#include <cstdio>

#include "Socket.hpp"
#include "SocketAddress.hpp"
#include "SocketListener.hpp"
#include "SocketConnection.hpp"

class test_SocketConnection : public ::testing::Test {

	public:

		void SetUp(void) {

			this->sock = new webserv::SocketListener(8080);
			// setting socket options
			const int	e = 1;
			this->sock->setsockopt(SOL_SOCKET, SO_REUSEADDR, &e, sizeof(int));
			this->sock->setsockopt(SOL_SOCKET, SO_REUSEPORT, &e, sizeof(int));
			struct timeval	timeout;
			timeout.tv_sec = 10;
			timeout.tv_usec = 0;
			this->sock->setsockopt(SOL_SOCKET, SO_RCVTIMEO, &timeout,
											   sizeof(struct timeval));
			// calling bind listen and accept
			// @dutesier in here we can make client make a request to sock
			// I'm using curl
			this->sock->bind();
			this->sock->listen();
			this->sock->accept();
			auto connections = this->sock->get_connections();
			this->connection = connections[0];
		}
		void TearDown(void) { delete this->sock; }

	protected:

		webserv::SocketListener*	sock;
		webserv::SocketConnection*	connection;
};

TEST_F(test_SocketConnection, constructor) {}

TEST_F(test_SocketConnection, destructor) {}

TEST_F(test_SocketConnection, close) {
	ASSERT_TRUE(this->connection->close()) << errno;
	ASSERT_EQ(this->connection->sockfd(), -1);
	std::string message = "HTTP/1.1 404\r\nContent-Length: 0\r\n";
	ASSERT_FALSE(this->connection->send(message)) << errno;
}

TEST_F(test_SocketConnection, recv) {
	auto str = this->connection->recv();
	ASSERT_NE(str, "");
}

TEST_F(test_SocketConnection, send) {
	std::string message = "HTTP/1.1 404\r\nContent-Length: 0\r\n";
	ASSERT_TRUE(this->connection->send(message)) << errno;
}
