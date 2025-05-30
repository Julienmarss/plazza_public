/*
** EPITECH PROJECT, 2025
** Tek 2 B-CCP-400-LIL-4-1-theplazza-martin.delebecque
** File description:
** ADefaultLogger.hpp
*/

#ifndef ADEFAULTLOGGER_HPP
#define ADEFAULTLOGGER_HPP

#include "ILogger.hpp"
#include "AFileLogger.hpp"
#include <iostream>
#include <string>
namespace Plazza {
    class ADefaultLogger : public ILogger {
    public:
        ADefaultLogger();
        virtual ~ADefaultLogger() = default;

        void logInfo(const std::string &message);
        void logWarning(const std::string &message);
        void logError(const std::string &message);
        void log(const std::string &message);
    private:
        AFileLogger _fileLogger;
    };
}
#endif