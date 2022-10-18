if (test)
    add_executable(webserv_test logger_test.cpp)
    add_test(webserv_gtests webserv_test)

    target_link_libraries(webserv_test
        PRIVATE my_project::my_srcs
        PRIVATE gtest
        PRIVATE gmock
        )

endif()