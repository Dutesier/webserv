if (test)
    add_executable(my_code_test my_code_test.cpp)
    add_test(my_code_gtests my_code_test)

    target_link_libraries(my_code_test
        PRIVATE my_project::my_srcs
        PRIVATE gtest
        PRIVATE gmock
        )

endif()