cmake_minimum_required(VERSION 2.8)

set(PROJECT_NAME webserv)
project(${PROJECT_NAME})

option(test "Build gtests" OFF)


add_subdirectory(srcs)
if (test)
	add_subdirectory(tests)
	enable_testing()
    find_package(PkgConfig)
    pkg_check_modules(GTEST "gtest" REQUIRED)
    pkg_check_modules(GMOCK "gmock" REQUIRED)
endif()

set(CMAKE_CXX_FLAGS "-g -Wall -Werror -Wextra -std=c++98")

include_directories(${CMAKE_SOURCE_DIR}/includes)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} 42::webserv_lib)
