project(my_srcs)

add_library(${PROJECT_NAME} STATIC
    my_code.cpp
    Logger.cpp
)

add_library(my_project::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/includes)