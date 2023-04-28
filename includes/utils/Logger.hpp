#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <fstream>
#include <iostream>
#include <string>

#include "smt.hpp"

enum LogLevel { DEBUG, INFO, WARNING, ERROR, FATAL };

class Logger {
    public:

        Logger(LogLevel l);
        ~Logger(void);

        void operator()(std::string const& message, char const* function,
                        char const* file, int line);

    private:

        LogLevel level;
};

#define LOGGER(Logger_, Message_)                                              \
 Logger_(Message_, __FUNCTION__, __FILE__, __LINE__);

smt::shared_ptr<Logger> Debug();
smt::shared_ptr<Logger> Info();
smt::shared_ptr<Logger> Warning();
smt::shared_ptr<Logger> Error();
smt::shared_ptr<Logger> Fatal();

#define LOG(Message_)   LOGGER((Debug().operator*()), Message_)
#define LOG_D(Message_) LOGGER((Debug().operator*()), Message_)
#define LOG_I(Message_) LOGGER((Info().operator*()), Message_)
#define LOG_W(Message_) LOGGER((Warning().operator*()), Message_)
#define LOG_E(Message_) LOGGER((Error().operator*()), Message_)
#define LOG_F(Message_) LOGGER((Fatal().operator*()), Message_)

#define LOGFILENAME "../webserv/webserv.log"

class FileLogger {
    public:

        FileLogger(LogLevel l);
        ~FileLogger(void);

        void operator()(std::string const& message, char const* function,
                        char const* file, int line);
        void setLogFile(std::string filename);

    private:

        std::ofstream logFile;
        bool          logToFile;
        LogLevel      level;
};

#define FLOGGER(FileLogger_, Message_)                                         \
 FileLogger_(Message_, __FUNCTION__, __FILE__, __LINE__);

smt::shared_ptr<FileLogger> FileDebug();
smt::shared_ptr<FileLogger> FileInfo();
smt::shared_ptr<FileLogger> FileWarning();
smt::shared_ptr<FileLogger> FileError();
smt::shared_ptr<FileLogger> FileFatal();

#define FLOG(Message_)   FLOGGER((FileDebug().operator*()), Message_)
#define FLOG_D(Message_) FLOGGER((FileDebug().operator*()), Message_)
#define FLOG_I(Message_) FLOGGER((FileInfo().operator*()), Message_)
#define FLOG_W(Message_) FLOGGER((FileWarning().operator*()), Message_)
#define FLOG_E(Message_) FLOGGER((FileError().operator*()), Message_)
#define FLOG_F(Message_) FLOGGER((FileFatal().operator*()), Message_)

#endif
