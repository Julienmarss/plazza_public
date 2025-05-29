/*
** EPITECH PROJECT, 2025
** Tek 2 B-CCP-400-LIL-4-1-theplazza-martin.delebecque
** File description:
** Parser.cpp
*/

#include "../includes/Parser.hpp"
#include "../includes/Order.hpp"
#include <iostream>
#include <sstream>
#include <regex>
#include <algorithm>

Plazza::PlazzaArguments Plazza::Parser::parseArgument(int ac, char **av) {
    PlazzaArguments args;
    if (ac < 4) {
        throw Plazza::Parser::ParserException("Not enough arguments");
    }
    try {
        args.cookingTime = std::stoi(av[1]);
    } catch (const std::invalid_argument&) {
        throw Plazza::Parser::ParserException("Invalid cooking time argument");
    }
    try {
        args.maxCooks = std::stoi(av[2]);
    } catch (const std::invalid_argument&) {
        throw Plazza::Parser::ParserException("Invalid max cooks argument");
    }
    try {
        args.timeToWait = std::stoi(av[3]);
    } catch (const std::invalid_argument&) {
        throw Plazza::Parser::ParserException("Invalid time to wait argument");
    }
    if (args.cookingTime <= 0 || args.maxCooks <= 0 || args.timeToWait <= 0) {
        throw Plazza::Parser::ParserException("Arguments must be positive integers");
    }
    if (ac > 4) {
        std::string loggerTypeStr = av[4];
        std::transform(loggerTypeStr.begin(), loggerTypeStr.end(), loggerTypeStr.begin(), ::tolower);
        if (loggerTypeStr == "console") {
            args.loggerType = Plazza::LoggerType::Console;
        } else if (loggerTypeStr == "file") {
            args.loggerType = Plazza::LoggerType::File;
        } else {
            throw Plazza::Parser::ParserException("Invalid logger type argument, must be 'console' or 'file'");
        }
    } else {
        args.loggerType = Plazza::LoggerType::Console;
    }
    return args;
}

Plazza::PizzaOrder Plazza::Parser::parseOrder(const std::string& str) {
    std::regex regex(R"(([a-zA-Z]+)\s+(S|M|L|XL|XXL)\s+x(\d+))");
    std::smatch match;
    if (std::regex_match(str, match, regex)) {
        PizzaType type = parseType(match[1]);
        PizzaSize size = parseSize(match[2]);
        int quantity = parseQuantity(match[3]);
        return {type, size, quantity};
    }
    return {PizzaType::Unknown, PizzaSize::Unknown, 0};
}

std::vector<Plazza::PizzaOrder> Plazza::Parser::parse(const std::string& str) {
    std::vector<Plazza::PizzaOrder> orders;
    std::istringstream stream(str);
    std::string line;
    while (std::getline(stream, line)) {
        if (line.empty()) continue;
        Plazza::PizzaOrder order = parseOrder(line);
        if (order.type != Plazza::PizzaType::Unknown && order.size != Plazza::PizzaSize::Unknown && order.quantity > 0) {
            orders.push_back(order);
        }
    }
    return orders;
}

Plazza::PizzaType Plazza::Parser::parseType(const std::string& str) {
    std::string lower = str;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    if (lower == "regina") return PizzaType::Regina;
    if (lower == "margarita") return PizzaType::Margarita;
    if (lower == "americana") return PizzaType::Americana;
    if (lower == "fantasia") return PizzaType::Fantasia;
    return PizzaType::Unknown;
}

Plazza::PizzaSize Plazza::Parser::parseSize(const std::string& str) {
    if (str == "S") return PizzaSize::S;
    if (str == "M") return PizzaSize::M;
    if (str == "L") return PizzaSize::L;
    if (str == "XL") return PizzaSize::XL;
    if (str == "XXL") return PizzaSize::XXL;
    return PizzaSize::Unknown;
}

int Plazza::Parser::parseQuantity(const std::string& str) {
    try {
        int quantity = std::stoi(str);
        return (quantity > 0) ? quantity : 0;
    } catch (const std::invalid_argument&) {
        return 0;
    } catch (const std::out_of_range&) {
        return 0;
    }
}