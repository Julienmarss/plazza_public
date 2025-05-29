/*
** EPITECH PROJECT, 2025
** Tek 2 B-CCP-400-LIL-4-1-theplazza-martin.delebecque
** File description:
** Order.hpp
*/
#ifndef ORDER_HPP_
#define ORDER_HPP_
#include <string>
#include <vector>

namespace Plazza {
    enum class PizzaType {
        Regina = 1,
        Margarita = 2,
        Americana = 4,
        Fantasia = 8,
        Unknown = 0
    };

    enum class PizzaSize {
        S = 1,
        M = 2,
        L = 4,
        XL = 8,
        XXL = 16,
        Unknown = 0
    };

    struct PizzaOrder {
        PizzaType type;
        PizzaSize size;
        int quantity;
    };
}
#endif /* !ORDER_HPP_ */