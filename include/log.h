#pragma once

#include <string>
#include <memory>

#include <list>

#include <sstream>
#include <fstream>
#include <iostream>

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
    uint64_t m_time;              // time stump

    std::string m_content;
};

class Logger {
    // Logger
public:
    typedef std::shared_ptr<Logger> ptr;

    Logger(const std::string &name = "root");

    void log(LogLevel::Level level, LogEvent &event);

    void debug(LogEvent &event);
    void info(LogEvent &event);
    void warn(LogEvent &event);
    void error(LogEvent &event);
    void fatal(LogEvent &event);

    void addAppender(LogAppender::ptr appender);
    void delAppender(LogAppender::ptr appender);

    inline LogLevel::Level getLevel() const {
        return m_level;
    }

    inline void setLevel(LogLevel::Level level) {
        m_level = level;
    }

private:
    std::string m_name;                  // log name
    LogLevel::Level m_level;             // log level
    std::list<LogAppender::ptr> m_dests; // log destinations
};

class LogAppender {
    // destination position of log
public:
    typedef std::shared_ptr<LogAppender> ptr;

    LogAppender();
    virtual ~LogAppender();

    virtual void log(LogLevel::Level level, LogEvent &event) = 0;

    inline void setFormatter(LogFormatter::ptr formatter) {
        m_formatter = formatter;
    }

    inline LogFormatter::ptr getFormatter() {
        return m_formatter;
    }

protected:
    LogLevel::Level m_level;
    LogFormatter::ptr m_formatter;
};

class LogFormatter {
public:
    typedef std::shared_ptr<LogFormatter> ptr;

    std::string format(LogEvent &event);

private:
};

class STDLogAppender : public LogAppender {
public:
    typedef std::shared_ptr<STDLogAppender> ptr;

    STDLogAppender();

    virtual void log(LogLevel::Level level, LogEvent &event) override final;

private:
};

class FileLogAppender : public LogAppender {
public:
    typedef std::shared_ptr<FileLogAppender> ptr;

    FileLogAppender(const std::string &file);

    virtual void log(LogLevel::Level level, LogEvent &event) override;

    // return true when succes to open file;
    bool reopen();

private:
    std::string m_filename;
    std::ofstream m_filestream;
};

} // namespace Lightning