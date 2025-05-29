/*
** EPITECH PROJECT, 2025
** Tek 2 B-CCP-400-LIL-4-1-theplazza-martin.delebecque
** File description:
** AConsoleLogger.cpp
*/
#include "AConsoleLogger.hpp"
#include <iostream>

void Plazza::AConsoleLogger::log(const std::string& message) {
    std::cout << message << std::endl;
}
void Plazza::AConsoleLogger::logError(const std::string& message) {
    std::cerr << "Error: " << message << std::endl;
}
void Plazza::AConsoleLogger::logWarning(const std::string& message) {
    std::cerr << "Warning: " << message << std::endl;
}
void Plazza::AConsoleLogger::logInfo(const std::string& message) {
    std::cout << "Info: " << message << std::endl;
}