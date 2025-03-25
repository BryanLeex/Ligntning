#pragma once

#include <string>
#include <memory>

namespace Lightning {

class LogLevel {
public:
    enum class Level {
        DEBUG = 1,
        INFO = 2,
        WARN = 3,
        ERROR = 4,
        FATAL = 5
    };
};

class LogEvent {
public:
    typedef std::shared_ptr<LogEvent> ptr;
    LogEvent();
    virtual ~LogEvent();

private:
    const char *m_file = nullptr; // file name
    int32_t m_line = 0;           // line number
    uint32_t m_elapse = 0;        // ms from prog begin to now
    uint32_t m_threadID = 0;      // thread number
    uint32_t m_fiberID = 0;       // fiber number

    uint64_t m_time; // time stump

    std::string m_content;
};

class Logger {
    // Logger
public:
    typedef std::shared_ptr<Logger> ptr;

    Logger(const std::string &name = "root");

    void log(LogLevel::Level level, const LogEvent &event);

private:
    std::string m_name;
    LogLevel::Level m_level;
    LogAppender::ptr m_dist;
};

class LogAppender {
    // distinate position of log
public:
    typedef std::shared_ptr<LogAppender> ptr;

    LogAppender();
    virtual ~LogAppender();

    void log(LogLevel::Level level, const LogEvent &event);

private:
    LogLevel::Level m_level;
};

class LogFormatter {
public:
    typedef std::shared_ptr<LogFormatter> ptr;

    std::string format(LogEvent &event);

private:
};

class STDLogAppender : public LogAppender {
};

class FileLogAppender : public LogAppender {
};

} // namespace Lightning