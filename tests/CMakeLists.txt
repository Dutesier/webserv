if (test)
    add_executable(webserv_test
		test.cpp
		test_parser.cpp
		test_socket.cpp
        test_logger.cpp
    )
    add_test(webserv_gtest webserv_test)

    target_link_libraries(webserv_test
        PRIVATE 42::webserv_lib
        PRIVATE gtest
        PRIVATE gmock
    )

endif()
