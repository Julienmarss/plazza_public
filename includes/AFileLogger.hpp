/*
** EPITECH PROJECT, 2025
** Tek 2 B-CCP-400-LIL-4-1-theplazza-martin.delebecque
** File description:
** AFileLogger.hpp
*/
#ifndef AFILELOGGER_HPP_
#define AFILELOGGER_HPP_
#include "ILogger.hpp"
#include <string>
namespace Plazza {
    class AFileLogger : public ILogger {
        public:
            AFileLogger(const std::string& filename);
            virtual ~AFileLogger() = default;
            void log(const std::string& message) override;
            void logError(const std::string& message) override;
            void logWarning(const std::string& message) override;
            void logInfo(const std::string& message) override;

        protected:
            std::string _filename;
    };
}
#endif /* !AFILELOGGER_HPP_ */