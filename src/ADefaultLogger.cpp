/*
** EPITECH PROJECT, 2025
** Tek 2 B-CCP-400-LIL-4-1-theplazza-martin.delebecque
** File description:
** ADefaultLogger.cpp
*/

#include "../includes/ADefaultLogger.hpp"

Plazza::ADefaultLogger::ADefaultLogger() : _fileLogger("plazza.log") {}
void Plazza::ADefaultLogger::logInfo(const std::string &message)
{
    std::cout << "[INFO] " << message << std::endl;
    _fileLogger.logInfo(message);
}

void Plazza::ADefaultLogger::logWarning(const std::string &message)
{
    std::cout << "[WARNING] " << message << std::endl;
    _fileLogger.logWarning(message);
}

void Plazza::ADefaultLogger::logError(const std::string &message)
{
    std::cerr << "[ERROR] " << message << std::endl;
    _fileLogger.logError(message);
}
void Plazza::ADefaultLogger::log(const std::string &message)
{
    std::cout << "[LOG] " << message << std::endl;
    _fileLogger.log(message);
}
