#pragma once
#include <string>
#include "noncopyable.h"

// 日志级别
enum LogLevel { INFO, ERROR, FATAL, DEBUG };

class Logger : noncopyable {
public:
    // 单例模式
    static Logger& getInstance();
    // 设置日志级别
    void setLogLevel(LogLevel logLevel);
    // 记录日志
    void log(const std::string& msg);

private:
    Logger() = default;
    int logLevel_;
};

#define LOG_INFO(fmt, ...)                       \
    do {                                         \
        Logger& logger = Logger::getInstance();  \
        logger.setLogLevel(INFO);                \
        char buf[1024] = {0};                    \
        snprintf(buf, 1024, fmt, ##__VA_ARGS__); \
        logger.log(buf);                         \
    } while (0);

#define LOG_ERROR(fmt, ...)                      \
    do {                                         \
        Logger& logger = Logger::getInstance();  \
        logger.setLogLevel(ERROR);               \
        char buf[1024] = {0};                    \
        snprintf(buf, 1024, fmt, ##__VA_ARGS__); \
        logger.log(buf);                         \
    } while (0);

#define LOG_FATAL(fmt, ...)                      \
    do {                                         \
        Logger& logger = Logger::getInstance();  \
        logger.setLogLevel(FATAL);               \
        char buf[1024] = {0};                    \
        snprintf(buf, 1024, fmt, ##__VA_ARGS__); \
        logger.log(buf);                         \
        exit(EXIT_FAILURE);                      \
    } while (0);

#ifdef MDEBUG
#define LOG_DEBUG(fmt, ...)                      \
    do {                                         \
        Logger& logger = Logger::getInstance();  \
        logger.setLogLevel(DEBUG);               \
        char buf[1024] = {0};                    \
        snprintf(buf, 1024, fmt, ##__VA_ARGS__); \
        logger.log(buf);                         \
    } while (0);
#else
#define LOG_DEBUG(fmt, ...)
#endif