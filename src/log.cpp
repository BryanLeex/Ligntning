#include "log.h"

namespace Lightning {

// impl for logger
Logger::Logger(const std::string &name = "root") :
    m_name(name) {
}

void Logger::log(LogLevel::Level level, LogEvent &event) {
    if (level >= m_level) {
        for (auto &app : m_dests) {
            app.get()->log(level, event);
        }
    }
}

void Logger::debug(LogEvent &event) {
    log(LogLevel::Level::DEBUG, event);
}
void Logger::info(LogEvent &event) {
    log(LogLevel::Level::INFO, event);
}
void Logger::warn(LogEvent &event) {
    log(LogLevel::Level::WARN, event);
}
void Logger::error(LogEvent &event) {
    log(LogLevel::Level::ERROR, event);
}
void Logger::fatal(LogEvent &event) {
    log(LogLevel::Level::FATAL, event);
}

void Logger::addAppender(LogAppender::ptr appender) {
    m_dests.push_back(appender);
}

void Logger::delAppender(LogAppender::ptr appender) {
    for (auto it = m_dests.begin(); it != m_dests.end(); it++) {
        if (*it == appender) {
            m_dests.erase(it);
            break;
        }
    }
}

// impl for appender

FileLogAppender::FileLogAppender(const std::string &name) :
    m_filename(name) {
}

void FileLogAppender::log(LogLevel::Level level, LogEvent &event) {
    if (level >= m_level) {
        m_filestream << m_formatter.get()->format(event);
    }
}

bool FileLogAppender::reopen() {
    if (m_filestream) {
        m_filestream.close();
    }
    m_filestream.open(m_filename);
    return m_filestream.is_open();
}

void STDLogAppender::log(LogLevel::Level level, LogEvent &event) {
    if (level >= m_level) {
        std::cout << m_formatter.get()->format(event);
    }
}

} // namespace Lightning