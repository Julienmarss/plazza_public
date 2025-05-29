/*
** EPITECH PROJECT, 2025
** Tek 2 B-CCP-400-LIL-4-1-theplazza-martin.delebecque
** File description:
** Main.cpp
*/

#include "../includes/Parser.hpp"
#include "../includes/Reception.hpp"
#include <iostream>
#include "../includes/AFileLogger.hpp"
#include "../includes/AConsoleLogger.hpp"

int main(int ac, char** av)
{

    try {
        try {
            Plazza::Parser parser;
            Plazza::PlazzaArguments args = parser.parseArgument(ac, av);
            if (args.loggerType == Plazza::LoggerType::Console) {
                Plazza::AConsoleLogger logger;
                Plazza::Reception reception(args.cookingTime, args.maxCooks, args.timeToWait, logger);
                reception.run();
            } else if (args.loggerType == Plazza::LoggerType::File) {
                Plazza::AFileLogger logger("plazza.log");
                Plazza::Reception reception(args.cookingTime, args.maxCooks, args.timeToWait, logger);
                reception.run();
            }
            } catch (const Plazza::ILogger::LoggerException& e) {
                std::cerr << "Logger error: " << e.what() << std::endl;
                return 84;
            }
    } catch (const Plazza::Parser::ParserException& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 84;
    }
    return 0;
}