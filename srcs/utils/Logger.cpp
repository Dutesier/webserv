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

smt::shared_ptr<Logger>& Debug() {
    static smt::shared_ptr<Logger> logger(new Logger(Logger::DEBUG));
    return logger;
}

smt::shared_ptr<Logger>& Info() {
    static smt::shared_ptr<Logger> logger(new Logger(Logger::INFO));
    return logger;
}

smt::shared_ptr<Logger>& Warning() {
    static smt::shared_ptr<Logger> logger(new Logger(Logger::WARNING));
    return logger;
}

smt::shared_ptr<Logger>& Error() {
    static smt::shared_ptr<Logger> logger(new Logger(Logger::ERROR));
    return logger;
}

smt::shared_ptr<Logger>& Fatal() {
    static smt::shared_ptr<Logger> logger(new Logger(Logger::Logger::FATAL));
    return logger;
}

// File Logger - FLOG
FileLogger::FileLogger(FLogLevel l) : level(l) { setLogFile(LOGFILENAME); }

FileLogger::~FileLogger() {
    if (logFile) logFile.close();
}

void FileLogger::setLogFile(std::string filename) {
    remove(filename.c_str());
    logFile.open(filename.c_str(), std::ios_base::app);
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

smt::shared_ptr<FileLogger>& FileDebug() {
    static smt::shared_ptr<FileLogger> fileLogger(new FileLogger(FileLogger::DEBUG));
    return (fileLogger);
}

smt::shared_ptr<FileLogger>& FileInfo() {
    static smt::shared_ptr<FileLogger> fileLogger(new FileLogger(FileLogger::INFO));
    return (fileLogger);
}

smt::shared_ptr<FileLogger>& FileWarning() {
    static smt::shared_ptr<FileLogger> fileLogger(new FileLogger(FileLogger::WARNING));
    return (fileLogger);
}

smt::shared_ptr<FileLogger>& FileError() {
    static smt::shared_ptr<FileLogger> fileLogger(new FileLogger(FileLogger::ERROR));
    return (fileLogger);
}

smt::shared_ptr<FileLogger>& FileFatal() {
    static smt::shared_ptr<FileLogger> fileLogger(new FileLogger(FileLogger::FATAL));
    return (fileLogger);
}
