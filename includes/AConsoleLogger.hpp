/*
** EPITECH PROJECT, 2025
** Tek 2 B-CCP-400-LIL-4-1-theplazza-martin.delebecque
** File description:
** AConsoleLogger.hpp
*/

#ifndef ACONSOLELOGGER_HPP_
#define ACONSOLELOGGER_HPP_
#include "ILogger.hpp"
#include <iostream>

namespace Plazza {
    class AConsoleLogger : public ILogger {
        public:
            AConsoleLogger() = default;
            ~AConsoleLogger() override = default;
            void log(const std::string& message);
            void logError(const std::string& message);
            void logWarning(const std::string& message);
            void logInfo(const std::string& message);
    };
}

#endif /* !ACONSOLELOGGER_HPP_ */