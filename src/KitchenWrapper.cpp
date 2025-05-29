/*
** EPITECH PROJECT, 2025
** B-CCP-400-LIL-4-1-theplazza-martin.delebecque
** File description:
** KitchenWrapper
*/


#include "../includes/KitchenWrapper.hpp"
#include "../includes/ILogger.hpp"

Plazza::KitchenWrapper::KitchenWrapper(int cooks, float multiplier, int refillTime, ILogger &logger)
    : _kitchen(std::make_unique<Kitchen>(cooks, multiplier, refillTime, logger)) {}

bool Plazza::KitchenWrapper::sendOrder(const Plazza::PizzaOrder& order)
{
    if (!_kitchen) {
        return false;
    }
    return _kitchen->pushOrder(order);
}

bool Plazza::KitchenWrapper::canAcceptMore() const
{
    if (!_kitchen) {
        return false;
    }
    return _kitchen->canAcceptMore();
}

size_t Plazza::KitchenWrapper::getQueueSize() const
{
    if (!_kitchen) {
        return 0;
    }
    return _kitchen->getQueueSize();
}

void Plazza::KitchenWrapper::printStatus() const
{
    if (_kitchen) {
        _kitchen->printStatus();
    }
}

bool Plazza::KitchenWrapper::isRunning() const
{
    if (!_kitchen) {
        return false;
    }
    return _kitchen->isRunning();
}