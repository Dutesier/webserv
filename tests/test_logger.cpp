#include <gtest/gtest.h>
#include "Logger.hpp"

TEST(LoggerTests, CanInstantiate) {
    LOG("LoggerTest");
}

TEST(LoggerTests, CanLogDebug) {
    LOG_D("LoggerTest");
}

TEST(LoggerTests, CanLogInfo) {
    LOG_I("LoggerTest");
}

TEST(LoggerTests, CanLogWarning) {
    LOG_W("LoggerTest");
}

TEST(LoggerTests, CanLogError) {
    LOG_E("LoggerTest");
}

TEST(LoggerTests, CanLogFatal) {
    LOG_F("LoggerTest");
}


TEST(FileLoggerTests, CanInstantiate) {
    FLOG("I'm writing to file");
}

TEST(FileLoggerTests, CanLogDebug) {
    FLOG_D("I'm writing to file");
}

TEST(FileLoggerTests, CanLogInfo) {
    FLOG_I("I'm writing to file");
}

TEST(FileLoggerTests, CanLogWarning) {
    FLOG_W("I'm writing to file");
}

TEST(FileLoggerTests, CanLogError) {
    FLOG_E("I'm writing to file");
}

TEST(FileLoggerTests, CanLogFatal) {
    FLOG_F("I'm writing to file");
}

TEST(FileLoggerTests, CanDoMultipleLogs){
    FLOG_D("multiple logs");
    FLOG_I("multiple logs");
    FLOG_W("multiple logs");
    FLOG_E("multiple logs");
    FLOG_F("multiple logs");
}