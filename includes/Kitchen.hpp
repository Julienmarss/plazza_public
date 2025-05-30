/*
** EPITECH PROJECT, 2025
** B-CCP-400-LIL-4-1-theplazza-martin.delebecque
** File description:
** Kitchen.hpp - Modified with callback support
*/

#ifndef KITCHEN_HPP_
#define KITCHEN_HPP_

#include "Order.hpp"
#include <queue>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <chrono>
#include <unordered_map>
#include <string>
#include <functional>
#include "ILogger.hpp"

namespace Plazza {
    class Kitchen {
        public:
            using PizzaReadyCallback = std::function<void(const std::string&)>;
            Kitchen(int cooks, float multiplier, int refillTime, ILogger &logger);
            Kitchen(int cooks, float multiplier, int refillTime, ILogger &logger, PizzaReadyCallback callback);
            ~Kitchen();
            bool canAcceptMore() const;
            bool pushOrder(const Plazza::PizzaOrder& order);
            size_t getQueueSize() const;
            void printStatus() const;
            bool isRunning() const;
        private:
            void cookLoop(int id);
            void ingredientRefillLoop();
            bool hasIngredientsFor(const Plazza::PizzaType& type) const;
            bool consumeIngredients(const Plazza::PizzaType& type);
            int _cooks;
            float _multiplier;
            int _refillTime;
            ILogger &_logger;
            PizzaReadyCallback _pizzaReadyCallback;
            size_t _maxQueue;
            std::queue<Plazza::PizzaOrder> _orders;
            std::vector<std::thread> _threads;
            std::vector<bool> _cookStatus;
            std::unordered_map<std::string, int> _ingredients;
            mutable std::mutex _mutex;
            std::condition_variable _cv;
            std::atomic<bool> _running;
            std::chrono::steady_clock::time_point _lastActive;
    };
}

#endif /* !KITCHEN_HPP_ */