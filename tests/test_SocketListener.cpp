#include <gtest/gtest.h>
#include <cstdio>

#include "Socket.hpp"
#include "SocketAddress.hpp"
#include "SocketConnection.hpp"
#include "SocketListener.hpp"

class test_SocketListener : public ::testing::Test {

	public:

		void SetUp(void) { this->sock = new webserv::SocketListener(8080); }
		void TearDown(void) { delete this->sock; }
		void set_options(void) {
			const int	enable = 1;
			this->sock->setsockopt(SOL_SOCKET, SO_REUSEADDR, &enable,
											   sizeof(int));
			this->sock->setsockopt(SOL_SOCKET, SO_REUSEPORT, &enable,
											   sizeof(int));
			struct timeval	timeout;
			timeout.tv_sec = 10;
			timeout.tv_usec = 0;
			this->sock->setsockopt(SOL_SOCKET, SO_RCVTIMEO, &timeout,
											   sizeof(struct timeval));
		}

	protected:

		webserv::SocketListener*	sock;
};

TEST_F(test_SocketListener, constructor) {}

TEST_F(test_SocketListener, destructor) {}

TEST_F(test_SocketListener, bind) {
	this->set_options();
	ASSERT_TRUE(this->sock->bind()) << errno;
}

TEST_F(test_SocketListener, listen) {
	this->set_options();
	ASSERT_TRUE(this->sock->bind()) << errno;
	ASSERT_TRUE(this->sock->listen()) << errno;
}

TEST_F(test_SocketListener, setsockopt) {
	// setting options
	const int	enable = 1;
	ASSERT_TRUE(this->sock->setsockopt(SOL_SOCKET, SO_REUSEADDR, &enable,
									   sizeof(int))) << errno;
	ASSERT_TRUE(this->sock->setsockopt(SOL_SOCKET, SO_REUSEPORT, &enable,
									   sizeof(int))) << errno;
	struct timeval	timeout;
	timeout.tv_sec = 10;
	timeout.tv_usec = 0;
	ASSERT_TRUE(this->sock->setsockopt(SOL_SOCKET, SO_RCVTIMEO, &timeout,
									   sizeof(struct timeval))) << errno;
	// checking if socket can bind and listen with set options
	this->set_options();
	ASSERT_TRUE(this->sock->bind()) << errno;
	ASSERT_TRUE(this->sock->listen()) << errno;
}

TEST_F(test_SocketListener, accept) {
	this->set_options();
	ASSERT_TRUE(this->sock->bind()) << errno;
	ASSERT_TRUE(this->sock->listen()) << errno;
	ASSERT_TRUE(this->sock->accept()) << errno;
}


TEST_F(test_SocketListener, shutdown) {

	this->set_options();
	ASSERT_TRUE(this->sock->bind()) << errno;
	ASSERT_TRUE(this->sock->listen()) << errno;
	ASSERT_TRUE(this->sock->shutdown(0)) << errno;
	ASSERT_FALSE(this->sock->bind()) << errno;
}

TEST_F(test_SocketListener, close) {

	ASSERT_TRUE(this->sock->close()) << errno;
	ASSERT_EQ(this->sock->sockfd(), -1);

	ASSERT_FALSE(this->sock->bind()) << errno;
	ASSERT_FALSE(this->sock->listen()) << errno;
	ASSERT_FALSE(this->sock->close()) << errno;
}

TEST_F(test_SocketListener, recv) {
	this->set_options();
	ASSERT_TRUE(this->sock->bind()) << errno;
	ASSERT_TRUE(this->sock->listen()) << errno;
	// @dutesier - client makes a request here if you'd like
	// i've been using curl to test
	ASSERT_TRUE(this->sock->accept()) << errno;
	auto	connections = this->sock->get_connections();
	auto	str = this->sock->recv(connections[0]);
	ASSERT_NE(str, "") << str;
}

TEST_F(test_SocketListener, send) {
	this->set_options();
	ASSERT_TRUE(this->sock->bind()) << errno;
	ASSERT_TRUE(this->sock->listen()) << errno;
	ASSERT_TRUE(this->sock->accept()) << errno;
	// @dutesier - client could maybe check if the received message is the same
	// I'm using curl to test that
	auto	connections = this->sock->get_connections();
	auto	str = this->sock->recv(connections[0]);
	ASSERT_NE(str, "") << str;
	std::string message = "HTTP/1.1 404\r\nContent-Length: 0\r\n";
	ASSERT_TRUE(this->sock->send(connections[0], message)) << errno;
}
