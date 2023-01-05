project(webserv_lib)

add_library(${PROJECT_NAME} STATIC
	webserv.cpp
	HTTPServer.cpp
	HTTPHandler.cpp
	HTTPRequest.cpp
	HTTPResponse.cpp
	Config.cpp
	ServerConfig.cpp
	Logger.cpp
	Error.cpp
	Socket.cpp
	SocketAddress.cpp
	SocketConnection.cpp
	TCPSocket.cpp
	ServerSocket.cpp
)

add_library(42::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/includes)
