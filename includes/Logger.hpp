#ifndef LOGGER_HPP
# define LOGGER_HPP

// # include <thread>
# include <iostream>
# include <fstream>
// # include <mutex>

enum LogLevel {
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    FATAL
};

class Logger{
public:
    ~Logger();
    static Logger* getLogger();
    void Log(std::string msg, LogLevel level);
    void setLogFile(std::string filename);
    
private:
    Logger();
    std::ofstream   logFile;
    static Logger*  instance;
    // std::mutex      mtx;
};


#define LOG(x) Logger::getLogger()->Log(x, DEBUG)

#define LOG_D(x) Logger::getLogger()->Log(x, DEBUG)

#define LOG_I(x) Logger::getLogger()->Log(x, INFO)

#define LOG_W(x) Logger::getLogger()->Log(x, WARNING)

#define LOG_E(x) Logger::getLogger()->Log(x, ERROR)

#define LOG_F(x) Logger::getLogger()->Log(x, FATAL)


#endif