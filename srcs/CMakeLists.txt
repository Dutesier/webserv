project(webserv_lib)

add_library(${PROJECT_NAME} STATIC
	webserv.cpp
	http/HTTPServer.cpp
	http/HTTPHandler.cpp
	http/HTTPParser.cpp
	http/HTTPRequest.cpp
	server/Config.cpp
	server/ServerConfig.cpp
	utils/Logger.cpp
	utils/Error.cpp
	socket/Socket.cpp
	socket/SocketAddress.cpp
	socket/SocketConnection.cpp
	socket/TCPSocket.cpp
	socket/ServerSocket.cpp
)

add_library(42::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/includes)
