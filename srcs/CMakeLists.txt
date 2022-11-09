project(webserv_lib)

add_library(${PROJECT_NAME} STATIC
	Socket.cpp
	SocketListener.cpp
	SocketConnection.cpp
	Client.cpp
	Logger.cpp
	Parser.cpp
	Config.cpp
	ConfigSetup.cpp
	ConfigParser.cpp
	Fail.cpp
)

add_library(42::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/includes)
