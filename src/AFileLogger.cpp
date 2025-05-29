/*
** EPITECH PROJECT, 2025
** Tek 2 B-CCP-400-LIL-4-1-theplazza-martin.delebecque
** File description:
** AFileLogger.cpp
*/

#include "AFileLogger.hpp"

#include <fstream>

Plazza::AFileLogger::AFileLogger(const std::string& filename) {
    _filename = filename;
    std::ofstream file(_filename, std::ios::app);
    if (!file.is_open()) {
        throw LoggerException("Failed to open log file: " + _filename);
    }
    file << "=== Log started ===" << std::endl;
    file.close();
}
void Plazza::AFileLogger::log(const std::string& message) {
    std::ofstream file(_filename, std::ios::app);
    if (file.is_open()) {
        file << "[LOG] " << message << std::endl;
        file.close();
    }
}
void Plazza::AFileLogger::logError(const std::string& message) {
    std::ofstream file(_filename, std::ios::app);
    if (file.is_open()) {
        file << "[ERROR] " << message << std::endl;
        file.close();
    }
}
void Plazza::AFileLogger::logWarning(const std::string& message) {
    std::ofstream file(_filename, std::ios::app);
    if (file.is_open()) {
        file << "[WARNING] " << message << std::endl;
        file.close();
    }
}
void Plazza::AFileLogger::logInfo(const std::string& message) {
    std::ofstream   file(_filename, std::ios::app);
    if (file.is_open()) {
        file << "[INFO] " << message << std::endl;
        file.close();
    }
}