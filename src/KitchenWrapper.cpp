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

Plazza::KitchenWrapper::~KitchenWrapper() {
    if (_process) {
        _process->stop();
        _logger.logInfo("Kitchen " + std::to_string(_kitchen_id) + " process stopped");
    }
}
bool Plazza::KitchenWrapper::sendOrder(const Plazza::PizzaOrder& order) {
    if (!_process || !_process->isRunning()) {
        return false;
    }
    
    if (_process->sendOrder(order)) {
        _estimated_queue_size++;
        _logger.logInfo("Order sent to kitchen " + std::to_string(_kitchen_id));
        return true;
    }
    
    _logger.logError("Failed to send order to kitchen " + std::to_string(_kitchen_id));
    return false;
}

bool Plazza::KitchenWrapper::canAcceptMore() const {
    if (!_process || !_process->isRunning()) {
        return false;
    }
    return _estimated_queue_size < static_cast<size_t>(_max_queue_size);
}

size_t Plazza::KitchenWrapper::getQueueSize() const {
    return _estimated_queue_size;
}

void Plazza::KitchenWrapper::printStatus() const {
    if (!_process || !_process->isRunning()) {
        _logger.logInfo("Kitchen " + std::to_string(_kitchen_id) + " [CLOSED]");
        return;
    }
    
    _logger.logInfo("Kitchen " + std::to_string(_kitchen_id) + " [PID: " + std::to_string(_process->getPid()) + "]");
    _logger.logInfo("  Estimated queue: " + std::to_string(_estimated_queue_size) + "/" + std::to_string(_max_queue_size));
    
    const_cast<KitchenWrapper*>(this)->_process->requestStatus();
}

bool Plazza::KitchenWrapper::isRunning() const {
    return _process && _process->isRunning();
}

pid_t Plazza::KitchenWrapper::getPid() const {
    return _process ? _process->getPid() : -1;
}

void Plazza::KitchenWrapper::processMessages() {
    if (!_process) return;
    
    Message msg;
    while (_process->receiveMessage(msg)) {
        switch (msg.type) {
            case MessageType::PIZZA_READY:
                _logger.logInfo("Pizza ready from kitchen " + std::to_string(_kitchen_id) + ": " + msg.content);
                if (_estimated_queue_size > 0) {
                    _estimated_queue_size--;
                }
                break;
                
            case MessageType::STATUS_RESPONSE:
                _logger.logInfo("Kitchen " + std::to_string(_kitchen_id) + " status: " + msg.content);
                break;
                
            default:
                break;
        }
    }
}