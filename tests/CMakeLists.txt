if (test)
    add_executable(webserv_test
    test.cpp
    test_Blocks.cpp
    test_CGIContext.cpp
    test_CGIHandler.cpp
    test_Config.cpp
    test_ConfigSocket.cpp
    test_HTTPParser.cpp
    test_HTTPRequest.cpp
    test_HTTPResponse.cpp
    test_Logger.cpp
    test_ServerAddress.cpp
    test_smt.cpp
    test_socket.cpp
    test_SocketAddress.cpp
    test_SocketConnection.cpp
    test_TCPSocket.cpp
    # test_HTTPHandler.cpp # took this out of the way because the config
                           # thats going to be received is going to change,
                 # and so will this in a future commit
    )
    add_test(webserv_gtest webserv_test)

    target_link_libraries(webserv_test
    PRIVATE pthread
        PRIVATE 42::webserv_lib
        PRIVATE gtest
        PRIVATE gmock
    )

  SET(CGI_PATH          ${CMAKE_CURRENT_SOURCE_DIR}/../webserv/website/cgi)
  SET(CGI_TEST        ${CMAKE_CURRENT_BINARY_DIR})

  EXECUTE_PROCESS(
    COMMAND "${CMAKE_COMMAND}" -E make_directory "${CGI_TEST}"
    COMMAND "${CMAKE_COMMAND}" -E copy_directory "${CGI_PATH}" "${CGI_TEST}"
  )

endif()
