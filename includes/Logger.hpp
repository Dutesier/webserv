#ifndef LOGGER_HPP
# define LOGGER_HPP

# include <iostream>
# include <fstream>

enum LogLevel {
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    FATAL
};

class Logger{
public:
    Logger(LogLevel l);
    ~Logger();

    void operator()(std::string const& message, char const *function, char const* file, int line);
private:
    LogLevel        level;
};

#define LOGGER(Logger_, Message_)\
Logger_(Message_,\
    __FUNCTION__,\
    __FILE__,\
    __LINE__\
);

Logger& Debug();
Logger& Info();
Logger& Warning();
Logger& Error();
Logger& Fatal();


#define LOG(Message_) LOGGER(Debug(), Message_)
#define LOG_D(Message_) LOGGER(Debug(), Message_)
#define LOG_I(Message_) LOGGER(Info(), Message_)
#define LOG_W(Message_) LOGGER(Warning(), Message_)
#define LOG_E(Message_) LOGGER(Error(), Message_)
#define LOG_F(Message_) LOGGER(Fatal(), Message_)

#define LOGFILENAME "webserv.log"

class FileLogger{
public:
    FileLogger(LogLevel l);
    ~FileLogger();

    void operator()(std::string const& message, char const *function, char const * file, int line);
    void setLogFile(std::string filename);

private:
    std::ofstream   logFile;
    bool            logToFile;
    LogLevel        level;
};

#define FLOGGER(FileLogger_, Message_)\
FileLogger_(Message_,\
    __FUNCTION__,\
    __FILE__,\
    __LINE__\
);

FileLogger& FileDebug() ;
FileLogger& FileInfo() ;
FileLogger& FileWarning() ;
FileLogger& FileError() ;
FileLogger& FileFatal() ;


#define FLOG(Message_) FLOGGER(FileDebug(), Message_)
#define FLOG_D(Message_) FLOGGER(FileDebug(), Message_)
#define FLOG_I(Message_) FLOGGER(FileInfo(), Message_)
#define FLOG_W(Message_) FLOGGER(FileWarning(), Message_)
#define FLOG_E(Message_) FLOGGER(FileError(), Message_)
#define FLOG_F(Message_) FLOGGER(FileFatal(), Message_)

#endif
