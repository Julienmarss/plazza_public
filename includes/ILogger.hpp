/*
** EPITECH PROJECT, 2025
** Tek 2 B-CCP-400-LIL-4-1-theplazza-martin.delebecque
** File description:
** ILogger.hpp
*/

#ifndef ILOGGER_HPP_
#define ILOGGER_HPP_
#include <string>

namespace Plazza {
    class ILogger {
        public:
            virtual ~ILogger() = default;
            virtual void log(const std::string& message) = 0;
            virtual void logError(const std::string& message) = 0;
            virtual void logWarning(const std::string& message) = 0;
            virtual void logInfo(const std::string& message) = 0;
            class LoggerException : public std::exception {
                public:
                    explicit LoggerException(const std::string& message) : _message(message) {}
                    virtual const char* what() const noexcept override {
                        return _message.c_str();
                    }
                private:
                    std::string _message;
            };
    };
}
#endif /* !LOGGER_HPP_ */