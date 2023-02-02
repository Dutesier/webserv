if (test)
    add_executable(webserv_test
		test.cpp
		test_Config.cpp
		test_Logger.cpp
		test_smt.cpp
		test_socket.cpp
		test_SocketAddress.cpp
		test_SocketConnection.cpp
		test_TCPSocket.cpp
		test_HTTPParser.cpp
		test_HTTPResponse.cpp
		Client.cpp
		# test_HTTPHandler.cpp # took this out of the way because the config
		                       # thats going to be received is going to change,
							   # and so will this in a future commit
    )
    add_test(webserv_gtest webserv_test)

    target_link_libraries(webserv_test
		PRIVATE pthread
        PRIVATE 42::webserv_lib
        PRIVATE gtest
        PRIVATE gmock
    )

endif()
