
#include <gtest/gtest.h>
#include <chrono>
#include <cerrno>
#include <cstdio>

#include <thread>
#include <arpa/inet.h>

#include "Socket.hpp"
#include "SocketListener.hpp"

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
	//SocketConnection* connection;
};

class SocketListenerFixture: public ::testing::Test {
public:
	void SetUp(){
		// This runs on TEST_F start
		listener = new SocketListener(AF_INET, 8096);
		listener->bind_to_address();
		listener->start_listening();
	}

	void TearDown(){
		// This runs on TEST_F end
		delete listener;
	}

	void sendRequestToListener(int port){
		int client_fd = socket(AF_INET, SOCK_STREAM, 0);
		if (client_fd == -1)
			perror("Socket");

		struct sockaddr_in address;
		address.sin_family = AF_INET;
		address.sin_port = htons(port);

		if (inet_pton(AF_INET, "127.0.0.1", &address.sin_addr) <= 0)
			perror("Inet_pton");
		
		if (connect(client_fd, (struct sockaddr*)(&address), sizeof(address)) < 0)
			perror("Connect");
		
		send(client_fd, "Hello, world!", 14, 0);
		close(client_fd);
	}

protected:
	SocketListener* listener;
	//SocketConnection* connection;
};



TEST_F(SocketFixture, SocketListenerCanInitialize) {
	EXPECT_TRUE(listener->getAddress() != NULL);
	ASSERT_GT(listener->getFD(), -1);
}

TEST_F(SocketFixture, SocketListenerCanBind) {
	ASSERT_TRUE(listener->bind_to_address());
}

TEST_F(SocketFixture, BoundSocketListenerCanStartListening) {
	EXPECT_TRUE(listener->bind_to_address());
	ASSERT_TRUE(listener->start_listening());
}

TEST_F(SocketListenerFixture, CanAccept_TimesOutAfter10Sec) {
	std::chrono::_V2::system_clock::time_point starttime = std::chrono::system_clock::now();
	std::thread server (&SocketListener::accept_connections, this->listener);
	std::thread client (&SocketListenerFixture::sendRequestToListener, this, 8096);
	server.join();
	client.join();
	std::chrono::_V2::system_clock::time_point endtime = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed = endtime - starttime;

	ASSERT_LT(elapsed.count(), 11);
}

TEST_F(SocketListenerFixture, CanReadIncomingRequest) {

}


// TEST( SocketUnitTest, CanCreateSocketIpv4 ) {

// 	SocketListener	socket(AF_INET, 8081);

// 	ASSERT_GT(socket.getFD(), 0);
// }

// TEST( SocketUnitTest, CanCreateSocketIpv6 ) {

// 	SocketListener	socket(AF_INET6, 8081);

// 	ASSERT_GT(socket.getFD(), 0);
// }

// TEST( SocketUnitTest, FailedCreateSocket ) {

// 	Socket	socket(0, 8081);

// 	ASSERT_LT(socket.getFD(), 0);
// }

// TEST( SocketUnitTest, CanBind ) {
// 	SocketListener	socket(AF_INET, 8081);

// 	ASSERT_TRUE(socket.bind_to_port());
// }

// TEST( SocketUnitTest, BoundSocketCanListen ) {
// 	SocketListener socket(AF_INET, 8082);

// 	EXPECT_TRUE(socket.bind_to_port());
// 	EXPECT_TRUE(socket.start_listening());
// }

// TEST ( SocketUnitTest, BoundSocketCantBind) {
// 	SocketListener socket(AF_INET, 8083);

// 	EXPECT_TRUE(socket.bind_to_port());
// 	ASSERT_FALSE(socket.bind_to_port());
// }

int	main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
