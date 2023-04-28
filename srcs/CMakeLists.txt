project(webserv_lib)

add_library(${PROJECT_NAME} STATIC
    webserv.cpp
    cgi/CGIContext.cpp
    cgi/CGIHandler.cpp
    config/Blocks.cpp
    config/Config.cpp
    config/ConfigSocket.cpp
    http/HTTPHandler.cpp
    http/HTTPParser.cpp
    http/HTTPRequest.cpp
    http/HTTPResponse.cpp
    http/HTTPValidator.cpp
    server/HTTPServer.cpp
    socket/ConnectionAddress.cpp
    socket/ServerAddress.cpp
    socket/ServerSocket.cpp
    socket/SocketConnection.cpp
    utils/Logger.cpp
)

add_library(42::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

# target_compile_options(${PROJECT_NAME} PUBLIC "-Werror" "-Wextra" "-Wall" "-std=c++98")
target_compile_options(${PROJECT_NAME} PUBLIC "-Werror" "-Wextra" "-Wall")

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/includes)
