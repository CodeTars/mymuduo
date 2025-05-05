#include "logger.h"
#include <iostream>
#include "Timestamp.h"

Logger& Logger::getInstance() {
    static Logger logger;
    return logger;
}

void Logger::setLogLevel(LogLevel logLevel) {
    logLevel_ = logLevel;
}

void Logger::log(const std::string& msg) {
    switch (logLevel_) {
        case INFO:
            std::cout << "[INFO]";
            break;
        case ERROR:
            std::cout << "[ERROR]";
            break;
        case FATAL:
            std::cout << "[FATAL]";
            break;
        case DEBUG:
            std::cout << "[DEBUG]";
            break;
        default:
            break;
    }

    std::cout << Timestamp::now().to_string() << msg << std::endl;
}