project(webserv_lib)

add_library(${PROJECT_NAME} STATIC
	Socket.cpp
	SocketListener.cpp
	Logger.cpp
	Parser.cpp
	Config.cpp
	ConfigSetup.cpp
	ConfigParser.cpp
	Fail.cpp
	SocketAddress.cpp
	SocketConnection.cpp
	# Client.cpp
	# ConfigSetup.cpp
)

add_library(42::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/includes)
