
#include <gtest/gtest.h>
#include <chrono>
#include <cerrno>
#include <cstdio>

#include <thread>
#include <arpa/inet.h>

#include "Socket.hpp"
#include "SocketListener.hpp"

# define HTTP_REQ "GET / HTTP/1.1\r\nHost:x\r\n\r\n"
# define HTTP_REQ_LEN 27
# define HTTP_RES "HTTP/1.1 404\r\nContent-Length: 0\r\n"
# define HTTP_RES_LEN 34

class SocketFixture: public ::testing::Test {
public:
	void SetUp(){
		// This runs on TEST_F start
		listener = new SocketListener(AF_INET, 8042);
	}

	void TearDown(){
		// This runs on TEST_F end
		delete listener;
	}
protected:
	SocketListener* listener;
};

class SocketListenerFixture: public ::testing::Test {
public:
	void SetUp(){
		// This runs on TEST_F start
		listener = new SocketListener(AF_INET, 8096);
		listener->bind_to_address();
		listener->start_listening();

		client = new SocketConnection();
		setup_client(8096);
		listener->accept_connections();

		// NOTE: on setup a functional TCP connection is created.
	}

	void TearDown(){
		// This runs on TEST_F end
		delete listener;
		delete client;
	}

	// Setup helper for the TCP connection (populates client and connects to listener)
	void setup_client(int port) {
		client->set_fd(socket(AF_INET, SOCK_STREAM, 0));
		if (client->get_fd() == -1)
			perror("Socket");

		struct sockaddr_in* temp = new struct sockaddr_in;

		temp->sin_family = AF_INET;
		temp->sin_port = htons(port);
		client->set_address(reinterpret_cast<struct sockaddr *>(temp));

		if (inet_pton(AF_INET, "127.0.0.1", &temp->sin_addr) <= 0)
			perror("Inet_pton");
		
		if (connect(client->get_fd(), client->get_address(), sizeof(*client->get_address())) < 0)
			perror("Connect");
	}

protected:
	SocketListener* listener;
	SocketConnection* client;
};



TEST_F(SocketFixture, SocketListenerCanInitialize) {
	EXPECT_TRUE(listener->get_address() != NULL);
	ASSERT_GT(listener->get_fd(), -1);
}

TEST_F(SocketFixture, SocketListenerCanBind) {
	ASSERT_TRUE(listener->bind_to_address());
}

TEST_F(SocketFixture, BoundSocketListenerCanStartListening) {
	EXPECT_TRUE(listener->bind_to_address());
	ASSERT_TRUE(listener->start_listening());
}

TEST_F(SocketFixture, ListeningSocketTimesoutIfNoConnections) {
	ASSERT_TRUE(listener->bind_to_address());
	ASSERT_TRUE(listener->start_listening());
	std::chrono::_V2::system_clock::time_point starttime = std::chrono::system_clock::now();
	listener->accept_connections();
	std::chrono::_V2::system_clock::time_point endtime = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed = endtime - starttime;
	ASSERT_GT(elapsed.count(), 9);
}

// The following tests test an already established connection between a listener and a client
TEST_F(SocketListenerFixture, CanReadFromConnection) {
	EXPECT_TRUE(client->write_connection(HTTP_REQ));

	ASSERT_STREQ(HTTP_REQ, listener->read_from_connection(NULL).c_str());
}

TEST_F(SocketListenerFixture, CanWriteToConnection) {
	EXPECT_TRUE(listener->write_to_connection(NULL, HTTP_RES));

	ASSERT_STREQ(HTTP_RES, client->read_connection().c_str());
}
