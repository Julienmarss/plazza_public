/*
** EPITECH PROJECT, 2025
** Tek 2 B-CCP-400-LIL-4-1-theplazza-martin.delebecque
** File description:
** Parser.hpp
*/
#ifndef PARSER_HPP_
#define PARSER_HPP_
#include <string>
#include "Order.hpp"
namespace Plazza
{
    enum LoggerType {
        Console,
        File,
        Default
    };
    struct PlazzaArguments {

        int cookingTime;
        int maxCooks;
        int timeToWait;
        LoggerType loggerType;
    };
    class Parser {
        public:
            PlazzaArguments parseArgument(int ac, char **av);
            PizzaOrder parseOrder(const std::string& str);
            std::vector<PizzaOrder> parse(const std::string& str);
            class ParserException : public std::exception {
                public:
                    ParserException(const std::string& message) : msg(message) {}
                    const char* what() const noexcept override {
                        return msg.c_str();
                    }
                private:
                    std::string msg;
            };
        private:
            PizzaType parseType(const std::string& str);
            PizzaSize parseSize(const std::string& str);
            int parseQuantity(const std::string& str);
    };
}
#endif /* !PARSER_HPP_ */