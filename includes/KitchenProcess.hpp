/*
** EPITECH PROJECT, 2025
** B-CCP-400-LIL-4-1-theplazza-martin.delebecque
** File description:
** KitchenProcess.hpp
*/

#ifndef KITCHENPROCESS_HPP_
#define KITCHENPROCESS_HPP_

#include "Order.hpp"
#include "IPC.hpp"
#include <sys/types.h>
#include <memory>

namespace Plazza {
    class KitchenProcess {
        public:
            KitchenProcess(int kitchen_id, int cooks, float multiplier, int refillTime);
            ~KitchenProcess();
            
            bool start();
            void stop();
            bool isRunning() const;
            pid_t getPid() const;
            bool sendOrder(const PizzaOrder& order);
            bool requestStatus();
            bool receiveMessage(Message& msg);
            
        private:
            int _kitchen_id;
            int _cooks;
            float _multiplier;
            int _refillTime;
            pid_t _pid;
            bool _running;
            
            std::unique_ptr<NamedPipeIPC> _ipc;
            std::string _pipe_name;
            
            void childProcess();
            void sendPizzaReadyMessage(NamedPipeIPC* ipc, const std::string& pizza_info);
            void sendStatusResponse(NamedPipeIPC* ipc, const std::string& status_info);
    };
}

#endif /* !KITCHENPROCESS_HPP_ */