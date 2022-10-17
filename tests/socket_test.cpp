
#include <gtest/gtest.h>
#include "Socket.hpp"

TEST( SocketUnitTest, CanCreateSocketIpv4 ) {

	Socket	socket(AF_INET, 8081);

	ASSERT_GT(socket.getFD(), 0);
}

TEST( SocketUnitTest, CanCreateSocketIpv6 ) {

	Socket	socket(AF_INET6, 8081);

	ASSERT_GT(socket.getFD(), 0);
}

TEST( SocketUnitTest, FailedCreateSocket ) {

	Socket	socket(0, 8081);

	ASSERT_LT(socket.getFD(), 0);
}

TEST( SocketUnitTest, CanBind ) {
	Socket	socket(AF_INET, 8081);

	ASSERT_TRUE(socket.bind_to_port());
}

int	main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
