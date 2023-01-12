project(webserv_lib)

add_library(${PROJECT_NAME} STATIC
	webserv.cpp
	http/HTTPHandler.cpp
	http/HTTPParser.cpp
	http/HTTPRequest.cpp
	http/HTTPResponse.cpp
	http/HTTPValidator.cpp
	server/HTTPServer.cpp
	server/Config.cpp
	server/Blocks.cpp
	utils/Logger.cpp
	socket/Socket.cpp
	socket/SocketAddress.cpp
	socket/SocketConnection.cpp
	socket/TCPSocket.cpp
	socket/ServerSocket.cpp
)

add_library(42::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/includes)
