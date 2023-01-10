#include "utils/Logger.hpp"

Logger::Logger(LogLevel l) : level(l) {}

Logger::~Logger() {}

void Logger::operator()(std::string const& message, char const* function,
                        char const* file, int line) {
    switch (level) {
        case DEBUG:
            std::cout << "[DEBUG]: "
                      << "(" << function << ":" << file << ":" << line << ") "
                      << message << std::endl;
            break;
        case INFO:
            std::cout << "[INFO]: "
                      << "(" << function << ":" << file << ":" << line << ") "
                      << message << std::endl;
            break;
        case WARNING:
            std::cout << "[WARNING]: "
                      << "(" << function << ":" << file << ":" << line << ") "
                      << message << std::endl;
            break;
        case ERROR:
            std::cout << "[ERROR]: "
                      << "(" << function << ":" << file << ":" << line << ") "
                      << message << std::endl;
            break;
        case FATAL:
            std::cout << "[FATAL]: "
                      << "(" << function << ":" << file << ":" << line << ") "
                      << message << std::endl;
            break;
    }
}

Logger& Debug() {
    static Logger logger(LogLevel::DEBUG);
    return logger;
}

Logger& Info() {
    static Logger logger(LogLevel::INFO);
    return logger;
}

Logger& Warning() {
    static Logger logger(LogLevel::WARNING);
    return logger;
}

Logger& Error() {
    static Logger logger(LogLevel::ERROR);
    return logger;
}

Logger& Fatal() {
    static Logger logger(LogLevel::FATAL);
    return logger;
}

// File Logger - FLOG
FileLogger::FileLogger(LogLevel l) : level(l) { setLogFile(LOGFILENAME); }

FileLogger::~FileLogger() {
    if (logFile) logFile.close();
}

void FileLogger::setLogFile(std::string filename) {
    remove(filename.c_str());
    logFile.open(filename, std::ios_base::app);
    if (!logFile.good()) {
        logFile.close();
        logToFile = false;
    }
    logToFile = true;
}

void FileLogger::operator()(std::string const& message, char const* function,
                            char const* file, int line) {
    if (!logToFile) {
        std::cerr << "Failed to open " << LOGFILENAME << " for writing."
                  << std::endl;
        return;
    }
    switch (level) {
        case DEBUG:
            logFile << "[DEBUG]: "
                    << "(" << function << ":" << file << ":" << line << ") "
                    << message << std::endl;
            break;
        case INFO:
            logFile << "[INFO]: "
                    << "(" << function << ":" << file << ":" << line << ") "
                    << message << std::endl;
            break;
        case WARNING:
            logFile << "[WARNING]: "
                    << "(" << function << ":" << file << ":" << line << ") "
                    << message << std::endl;
            break;
        case ERROR:
            logFile << "[ERROR]: "
                    << "(" << function << ":" << file << ":" << line << ") "
                    << message << std::endl;
            break;
        case FATAL:
            logFile << "[FATAL]: "
                    << "(" << function << ":" << file << ":" << line << ") "
                    << message << std::endl;
            break;
    }
}

FileLogger& FileDebug() {
    static FileLogger fileLogger(LogLevel::DEBUG);
    return fileLogger;
}

FileLogger& FileInfo() {
    static FileLogger fileLogger(LogLevel::INFO);
    return fileLogger;
}

FileLogger& FileWarning() {
    static FileLogger fileLogger(LogLevel::WARNING);
    return fileLogger;
}

FileLogger& FileError() {
    static FileLogger fileLogger(LogLevel::ERROR);
    return fileLogger;
}

FileLogger& FileFatal() {
    static FileLogger fileLogger(LogLevel::FATAL);
    return fileLogger;
}
