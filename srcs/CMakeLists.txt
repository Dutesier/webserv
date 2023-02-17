project(webserv_lib)

add_library(${PROJECT_NAME} STATIC
	webserv.cpp
	config/Blocks.cpp
	config/Config.cpp
	config/ConfigSocket.cpp
	http/HTTPHandler.cpp
	http/HTTPParser.cpp
	http/HTTPRequest.cpp
	http/HTTPResponse.cpp
	http/HTTPValidator.cpp
	server/HTTPServer.cpp
	utils/Logger.cpp
	socket/Socket.cpp
	socket/SocketAddress.cpp
	socket/SocketConnection.cpp
	socket/TCPSocket.cpp
	socket/ServerSocket.cpp
	cgi/CGIHandler.cpp
	cgi/CGIContext.cpp
)

add_library(42::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/includes)
