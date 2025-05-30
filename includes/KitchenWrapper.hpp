/*
** EPITECH PROJECT, 2025
** B-CCP-400-LIL-4-1-theplazza-martin.delebecque
** File description:
** KitchenWrapper
*/

#ifndef KITCHENWRAPPER_HPP_
#define KITCHENWRAPPER_HPP_

#include "KitchenProcess.hpp"
#include "Kitchen.hpp"
#include "ILogger.hpp"
#include <memory>
#include <sys/types.h>

namespace Plazza {
    class KitchenWrapper {
        public:
            KitchenWrapper(int kitchen_id, int cooks, float multiplier, int refillTime, ILogger &logger);
            ~KitchenWrapper();
            KitchenWrapper(const KitchenWrapper&) = delete;
            KitchenWrapper& operator=(const KitchenWrapper&) = delete;
            KitchenWrapper(KitchenWrapper&&) = default;
            KitchenWrapper& operator=(KitchenWrapper&&) = default;
            bool sendOrder(const Plazza::PizzaOrder& order);
            bool canAcceptMore() const;
            size_t getQueueSize() const;
            void printStatus() const;
            bool isRunning() const;
            pid_t getPid() const;
            void processMessages();
        private:
            int _kitchen_id;
            ILogger &_logger;
            std::unique_ptr<KitchenProcess> _process;
            mutable size_t _estimated_queue_size;
            mutable int _max_queue_size;
    };
}

#endif /* !KITCHENWRAPPER_HPP_ */