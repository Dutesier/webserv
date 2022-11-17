#include <gtest/gtest.h>
#include <chrono>
#include <cstdio>

#include "Socket.hpp"
#include "SocketAddress.hpp"
#include "SocketConnection.hpp"
#include "TCPSocket.hpp"
#include "Client.hpp"

# define HTTP_REQ "GET / HTTP/1.1\r\nHost:x\r\n\r\n"
# define HTTP_REQ_LEN 27
# define HTTP_RES "HTTP/1.1 404\r\nContent-Length: 0\r\n"
# define HTTP_RES_LEN 34

class test_TCPSocket : public ::testing::Test {

	public:

		void SetUp(void) { this->sock = new webserv::TCPSocket(8080); }
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

		webserv::TCPSocket*	sock;
};

TEST_F(test_TCPSocket, constructor) {}

TEST_F(test_TCPSocket, destructor) {}

TEST_F(test_TCPSocket, bind) {
	this->set_options();
	ASSERT_TRUE(this->sock->bind()) << errno;
}

TEST_F(test_TCPSocket, listen) {
	this->set_options();
	ASSERT_TRUE(this->sock->bind()) << errno;
	ASSERT_TRUE(this->sock->listen()) << errno;
}

TEST_F(test_TCPSocket, setsockopt) {
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

TEST_F(test_TCPSocket, accept) {
	this->set_options();
	ASSERT_TRUE(this->sock->bind()) << errno;
	ASSERT_TRUE(this->sock->listen()) << errno;
	Client client(8080);
	ASSERT_TRUE(this->sock->accept()) << errno;
}


TEST_F(test_TCPSocket, shutdown) {

	this->set_options();
	ASSERT_TRUE(this->sock->bind()) << errno;
	ASSERT_TRUE(this->sock->listen()) << errno;
	ASSERT_TRUE(this->sock->shutdown(0)) << errno;
	ASSERT_FALSE(this->sock->bind()) << errno;
}

TEST_F(test_TCPSocket, close) {

	ASSERT_TRUE(this->sock->close()) << errno;
	ASSERT_EQ(this->sock->sockfd(), -1);

	ASSERT_FALSE(this->sock->bind()) << errno;
	ASSERT_FALSE(this->sock->listen()) << errno;
	ASSERT_FALSE(this->sock->close()) << errno;
}

TEST_F(test_TCPSocket, recv) {
	this->set_options();
	ASSERT_TRUE(this->sock->bind()) << errno;
	ASSERT_TRUE(this->sock->listen()) << errno;
	Client client(8080);
	ASSERT_TRUE(this->sock->accept()) << errno;
	auto	connections = this->sock->get_connections();
	client.send_message(HTTP_REQ);
	auto	str = this->sock->recv(connections[0]);
	ASSERT_NE(str, "") << str;
}

TEST_F(test_TCPSocket, send) {
	this->set_options();
	ASSERT_TRUE(this->sock->bind()) << errno;
	ASSERT_TRUE(this->sock->listen()) << errno;
	Client client(8080);
	ASSERT_TRUE(this->sock->accept()) << errno;
	auto	connections = this->sock->get_connections();
	ASSERT_TRUE(this->sock->send(connections[0], HTTP_RES)) << errno;
	ASSERT_STREQ(client.receive_message().c_str(), HTTP_RES) << errno;
}

TEST_F(test_TCPSocket, ListeningSocketTimesoutIfNoConnections) {
	this->set_options();
	ASSERT_TRUE(sock->bind());
	ASSERT_TRUE(sock->listen());
	std::chrono::_V2::system_clock::time_point starttime = std::chrono::system_clock::now();
	sock->accept();
	std::chrono::_V2::system_clock::time_point endtime = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed = endtime - starttime;
	ASSERT_GT(elapsed.count(), 9);
}
