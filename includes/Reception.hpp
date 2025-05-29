/*
** EPITECH PROJECT, 2025
** B-CCP-400-LIL-4-1-theplazza-martin.delebecque
** File description:
** Reception
*/

#ifndef RECEPTION_HPP_
#define RECEPTION_HPP_

#include "Order.hpp"
#include <queue>
#include <vector>
#include <memory>
#include <string>
#include <iostream>
#include "KitchenWrapper.hpp"
#include "ILogger.hpp"

namespace Plazza {

    class Reception {
    public:
        Reception(float multiplier, int cooks, int refillTime, ILogger &logger);
        void run();
    private:
        float _multiplier;
        int _cooks;
        int _refillTime;
        ILogger &_logger;
        struct KitchenSim {
            std::vector<PizzaOrder> queue;
            size_t maxQueue;
        };
        std::vector<std::unique_ptr<KitchenWrapper>> _kitchens;
        void handleOrder(const PizzaOrder& order);
        void dispatchOrders();
        void printStatus();
        void createKitchen();
        void cleanupClosedKitchens();
    };
}

#endif /* !RECEPTION_HPP_ */