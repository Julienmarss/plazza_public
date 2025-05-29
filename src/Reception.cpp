/*
** EPITECH PROJECT, 2025
** B-CCP-400-LIL-4-1-theplazza-martin.delebecque
** File description:
** Reception
*/

#include "../includes/Reception.hpp"
#include "../includes/Parser.hpp"
#include "../includes/ILogger.hpp"
#include <iostream>
#include <algorithm>

Plazza::Reception::Reception(float multiplier, int cooks, int refillTime, ILogger &logger)
    : _multiplier(multiplier), _cooks(cooks), _refillTime(refillTime), _logger(logger)
{
}

void Plazza::Reception::run()
{
    std::string input;
    Parser parser;

    _logger.logInfo("=== Plazza ===");
    _logger.logInfo("Commands:");
    _logger.logInfo("  - Order pizzas: 'regina XXL x2; margarita M x1'");
    _logger.logInfo("  - Check status: 'status'");
    _logger.logInfo("  - Exit: 'exit'");
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, input);
        if (input == "exit" || std::cin.eof()) {
            _logger.logInfo("Shutting down restaurant...");
            break;
        }
        if (input == "status") {
            cleanupClosedKitchens();
            printStatus();
            continue;
        }
        if (input.empty()) {
            continue;
        }
        try {
            std::vector<PizzaOrder> orders = parser.parse(input);
            if (orders.empty()) {
                _logger.logWarning("No valid pizza orders found in input.");
                _logger.logInfo("Invalid order format. Example: 'regina M x2; margarita L x1'");
                _logger.logInfo("Please try again.");
                continue;
            }
            _logger.logInfo("Processing orders: " + std::to_string(orders.size()));
            for (const auto& order : orders) {
                handleOrder(order);
            }
        } catch (const std::exception& e) {
            _logger.logError("Error processing order: " + std::string(e.what()));
        }
    }
}

void Plazza::Reception::cleanupClosedKitchens()
{
    _kitchens.erase(
        std::remove_if(_kitchens.begin(), _kitchens.end(),
            [](const std::unique_ptr<KitchenWrapper>& kitchen) {
                return !kitchen->isRunning();
            }),
        _kitchens.end()
    );
}

void Plazza::Reception::handleOrder(const PizzaOrder& order)
{
    cleanupClosedKitchens();
    for (int i = 0; i < order.quantity; ++i) {
        bool assigned = false;
        for (auto& kitchen : _kitchens) {
            if (kitchen->canAcceptMore()) {
                if (kitchen->sendOrder({order.type, order.size, 1})) {
                    assigned = true;
                    break;
                }
            }
        }
        if (!assigned) {
            createKitchen();
            if (!_kitchens.back()->sendOrder({order.type, order.size, 1})) {
                _logger.logError("[ERROR] Failed to assign pizza to new kitchen");
            }
        }
    }
    _logger.logInfo("Order received: " + std::to_string(order.quantity) + " pizza(s)");
}

void Plazza::Reception::createKitchen()
{
    auto kitchen = std::make_unique<KitchenWrapper>(_cooks, _multiplier, _refillTime, _logger);
    _kitchens.push_back(std::move(kitchen));
    _logger.logInfo("Created new kitchen (total kitchens = " + std::to_string(_kitchens.size()) + ")");
}

void Plazza::Reception::printStatus()
{
    if (_kitchens.empty()) {
        _logger.logInfo("No kitchens currently active.");
        return;
    }
    _logger.logInfo("Current kitchen status:");
    for (size_t i = 0; i < _kitchens.size(); ++i) {
        _logger.logInfo("Kitchen #" + std::to_string(i) + ":");
        _kitchens[i]->printStatus();
    }
    _logger.logInfo("---");
}