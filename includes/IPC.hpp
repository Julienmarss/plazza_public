/*
** EPITECH PROJECT, 2025
** B-CCP-400-LIL-4-1-theplazza-martin.delebecque
** File description:
** IPC.hpp
*/

#ifndef IPC_HPP_
#define IPC_HPP_

#include "Order.hpp"
#include <string>
#include <sys/types.h>

namespace Plazza {
    enum class MessageType {
        ORDER = 1,
        STATUS_REQUEST = 2,
        STATUS_RESPONSE = 3,
        PIZZA_READY = 4,
        SHUTDOWN = 5
    };

    struct Message {
        MessageType type;
        PizzaOrder order;
        std::string content;
        pid_t sender_pid;
        int kitchen_id;

        std::string serialize() const;
        void deserialize(const std::string& data);
    };

    class IPC {
        public:
            IPC();
            virtual ~IPC();
            virtual bool send(const Message& msg) = 0;
            virtual bool receive(Message& msg) = 0;
            virtual bool isReady() const = 0;
            IPC& operator<<(const Message& msg);
            IPC& operator>>(Message& msg);

        protected:
            int _fd_read;
            int _fd_write;
    };

    class NamedPipeIPC : public IPC {
        public:
            NamedPipeIPC(const std::string& pipe_name, bool is_server = false);
            ~NamedPipeIPC() override;

            bool send(const Message& msg) override;
            bool receive(Message& msg) override;
            bool isReady() const override;
            
        private:
            std::string _pipe_name_in;
            std::string _pipe_name_out;
            bool _is_server;
            bool _ready;
            void createPipes();
            void openPipes();
    };
}

#endif /* !IPC_HPP_ */