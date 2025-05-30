/*
** EPITECH PROJECT, 2025
** B-CCP-400-LIL-4-1-theplazza-martin.delebecque
** File description:
** KitchenProcess.cpp
*/

#include "../includes/KitchenProcess.hpp"
#include "../includes/Kitchen.hpp"
#include "../includes/AConsoleLogger.hpp"
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <errno.h>
#include <cstring>

Plazza::KitchenProcess::KitchenProcess(int kitchen_id, int cooks, float multiplier, int refillTime)
    : _kitchen_id(kitchen_id), _cooks(cooks), _multiplier(multiplier), 
      _refillTime(refillTime), _pid(-1), _running(false) {
    _pipe_name = "/tmp/plazza_kitchen_" + std::to_string(kitchen_id);
}

Plazza::KitchenProcess::~KitchenProcess()
{
    stop();
}

bool Plazza::KitchenProcess::start()
{
    if (_running)
        return true;
    
    _ipc = std::make_unique<NamedPipeIPC>(_pipe_name, true);
    if (!_ipc->isReady()) {
        std::cerr << "Failed to create IPC for kitchen " << _kitchen_id << std::endl;
        return false;
    }
    
    _pid = fork();
    if (_pid == -1) {
        std::cerr << "Fork failed for kitchen " << _kitchen_id << ": " << strerror(errno) << std::endl;
        return false;
    }
    
    if (_pid == 0) {
        childProcess();
        exit(0);
    } else {
        _running = true;
        std::cout << "Kitchen " << _kitchen_id << " started with PID " << _pid << std::endl;
    }
    return true;
}

void Plazza::KitchenProcess::stop()
{
    if (!_running || _pid == -1)
        return;
    Message msg;
    msg.type = MessageType::SHUTDOWN;
    msg.sender_pid = getpid();
    msg.kitchen_id = _kitchen_id;
    _ipc->send(msg);
    
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    int status;
    pid_t result = waitpid(_pid, &status, WNOHANG);
    if (result == 0) {
        kill(_pid, SIGTERM);
        waitpid(_pid, &status, 0);
    }
    _running = false;
    _pid = -1;
}

bool Plazza::KitchenProcess::isRunning() const
{
    if (!_running || _pid == -1)
        return false;

    int status;
    pid_t result = waitpid(_pid, &status, WNOHANG);
    if (result == _pid) {
        const_cast<KitchenProcess*>(this)->_running = false;
        return false;
    }
    return true;
}

pid_t Plazza::KitchenProcess::getPid() const
{
    return _pid;
}

bool Plazza::KitchenProcess::sendOrder(const PizzaOrder& order)
{
    if (!_running || !_ipc)
        return false;
    Message msg;
    msg.type = MessageType::ORDER;
    msg.order = order;
    msg.sender_pid = getpid();
    msg.kitchen_id = _kitchen_id;
    return _ipc->send(msg);
}

bool Plazza::KitchenProcess::requestStatus()
{
    if (!_running || !_ipc)
        return false;
    Message msg;
    msg.type = MessageType::STATUS_REQUEST;
    msg.sender_pid = getpid();
    msg.kitchen_id = _kitchen_id;
    return _ipc->send(msg);
}

bool Plazza::KitchenProcess::receiveMessage(Message& msg)
{
    if (!_running || !_ipc)
        return false;
    return _ipc->receive(msg);
}

void Plazza::KitchenProcess::childProcess()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    
    auto child_ipc = std::make_unique<NamedPipeIPC>(_pipe_name, false);
    if (!child_ipc->isReady()) {
        std::cerr << "Child: Failed to create IPC for kitchen " << _kitchen_id << std::endl;
        return;
    }
    AConsoleLogger logger;
    auto pizzaReadyCallback = [&](const std::string& pizza_info) {
        sendPizzaReadyMessage(child_ipc.get(), pizza_info);
    };
    Kitchen kitchen(_cooks, _multiplier, _refillTime, logger, pizzaReadyCallback);
    logger.logInfo("Kitchen " + std::to_string(_kitchen_id) + " child process started (PID: " + std::to_string(getpid()) + ")");
    
    Message msg;
    while (true) {
        if (child_ipc->receive(msg)) {
            switch (msg.type) {
                case MessageType::ORDER:
                    if (kitchen.pushOrder(msg.order)) {
                        logger.logInfo("Kitchen " + std::to_string(_kitchen_id) + ": Order accepted");
                    } else {
                        logger.logWarning("Kitchen " + std::to_string(_kitchen_id) + ": Order refused (full or no ingredients)");
                    }
                    break;
                    
                case MessageType::STATUS_REQUEST:
                    {
                        std::string status_info = "Kitchen " + std::to_string(_kitchen_id) + 
                                                 " - Queue: " + std::to_string(kitchen.getQueueSize()) + 
                                                 " - Status: " + (kitchen.isRunning() ? "ACTIVE" : "CLOSED");
                        sendStatusResponse(child_ipc.get(), status_info);
                    }
                    break;
                    
                case MessageType::SHUTDOWN:
                    logger.logInfo("Kitchen " + std::to_string(_kitchen_id) + ": Shutdown requested");
                    return;
                    
                default:
                    break;
            }
        }
        if (!kitchen.isRunning()) {
            logger.logInfo("Kitchen " + std::to_string(_kitchen_id) + " shutting down due to inactivity");
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

void Plazza::KitchenProcess::sendPizzaReadyMessage(NamedPipeIPC* ipc, const std::string& pizza_info)
{
    if (!ipc) 
        return;

    Message msg;
    msg.type = MessageType::PIZZA_READY;
    msg.content = pizza_info;
    msg.sender_pid = getpid();
    msg.kitchen_id = _kitchen_id;
    ipc->send(msg);
}

void Plazza::KitchenProcess::sendStatusResponse(NamedPipeIPC* ipc, const std::string& status_info)
{
    if (!ipc) 
        return;
    Message msg;
    msg.type = MessageType::STATUS_RESPONSE;
    msg.content = status_info;
    msg.sender_pid = getpid();
    msg.kitchen_id = _kitchen_id;
    ipc->send(msg);
}