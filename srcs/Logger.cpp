#include "Logger.hpp"

Logger* Logger::getLogger(){
    if (instance == NULL) {
        instance = new Logger;
    }
    return instance;
}

Logger::~Logger(){
    if (logFile)
        logFile.close();
}

Logger::Logger(): logFile(NULL){
    std::cout << "Created logger instance" << std::endl;
}

void Logger::Log(std::string msg, LogLevel level) {
    mtx.lock();
    switch (level){
        case DEBUG:
            msg = "[DEBUG]: " + msg;
            break;
        case INFO:
            msg = "[INFO]: " + msg;
            break;
        case WARNING:
            msg = "[WARNING]: " + msg;
            break;
        case ERROR:
            msg = "[ERROR]: " + msg;
            break;
        case FATAL:
            msg = "[FATAL]: " + msg;
            break;
    }
    if (!logFile) {
        std::cerr << msg << std::endl;
    } else {
        logFile << msg << std::endl;
    }
    mtx.unlock();
}

void Logger::setLogFile(std::string filename) {
    if (filename != "")
        logFile.open(filename, std::ios_base::app);
}