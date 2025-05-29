/*
** EPITECH PROJECT, 2025
** B-CCP-400-LIL-4-1-theplazza-martin.delebecque
** File description:
** KitchenWrapper
*/

#ifndef KITCHENWRAPPER_HPP_
#define KITCHENWRAPPER_HPP_

#include "Kitchen.hpp"
#include "ILogger.hpp"
#include <memory>

namespace Plazza {
    class KitchenWrapper {
        public:
            KitchenWrapper(int cooks, float multiplier, int refillTime, ILogger &logger);
            ~KitchenWrapper() = default;
            KitchenWrapper(const KitchenWrapper&) = delete;
            KitchenWrapper& operator=(const KitchenWrapper&) = delete;
            KitchenWrapper(KitchenWrapper&&) = default;
            KitchenWrapper& operator=(KitchenWrapper&&) = default;
            bool sendOrder(const Plazza::PizzaOrder& order);
            bool canAcceptMore() const;
            size_t getQueueSize() const;
            void printStatus() const;
            bool isRunning() const;
        private:
            std::unique_ptr<Kitchen> _kitchen;
    };
}

#endif /* !KITCHENWRAPPER_HPP_ */