if (test)
    add_executable(webserv_test
        logger_test.cpp
    )
    add_test(webserv_gtest webserv_test)

    target_link_libraries(webserv_test
        PRIVATE 42::webserv_lib
        PRIVATE gtest
        PRIVATE gmock
        )

endif()