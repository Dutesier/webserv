#include <gtest/gtest.h>
#include "Logger.hpp"

// Initializing instance to avoid linker errors
Logger* Logger::instance = NULL;

TEST(LoggerTests, CanInstantiate) {
    LOG("LoggerTest");
}

TEST(LoggerTests, CanWriteToLogFile) {
    Logger::getLogger()->setLogFile("webserv.log");
    LOG("I'm writing to file");
}
