/*
** EPITECH PROJECT, 2025
** B-CCP-400-LIL-4-1-theplazza-martin.delebecque
** File description:
** IPC.cpp
*/

#include "../includes/IPC.hpp"
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sstream>
#include <iostream>
#include <cstring>
#include <errno.h>

std::string Plazza::Message::serialize() const
{
    std::ostringstream oss;
    oss << static_cast<int>(type) << "|"
        << static_cast<int>(order.type) << "|"
        << static_cast<int>(order.size) << "|"
        << order.quantity << "|"
        << sender_pid << "|"
        << kitchen_id << "|"
        << content.length() << "|"
        << content;
    return oss.str();
}

void Plazza::Message::deserialize(const std::string& data)
{
    std::istringstream iss(data);
    std::string token;
    
    if (std::getline(iss, token, '|')) {
        type = static_cast<MessageType>(std::stoi(token));
    }
    if (std::getline(iss, token, '|')) {
        order.type = static_cast<PizzaType>(std::stoi(token));
    }
    if (std::getline(iss, token, '|')) {
        order.size = static_cast<PizzaSize>(std::stoi(token));
    }
    if (std::getline(iss, token, '|')) {
        order.quantity = std::stoi(token);
    }
    if (std::getline(iss, token, '|')) {
        sender_pid = std::stoi(token);
    }
    if (std::getline(iss, token, '|')) {
        kitchen_id = std::stoi(token);
    }
    if (std::getline(iss, token, '|')) {
        int content_length = std::stoi(token);
        content.resize(content_length);
        if (content_length > 0) {
            iss.read(&content[0], content_length);
        }
    }
}

Plazza::IPC::IPC() : _fd_read(-1), _fd_write(-1) {}

Plazza::IPC::~IPC() {
    if (_fd_read != -1) close(_fd_read);
    if (_fd_write != -1) close(_fd_write);
}

Plazza::IPC& Plazza::IPC::operator<<(const Message& msg)
{
    send(msg);
    return *this;
}

Plazza::IPC& Plazza::IPC::operator>>(Message& msg)
{
    receive(msg);
    return *this;
}

Plazza::NamedPipeIPC::NamedPipeIPC(const std::string& pipe_name, bool is_server)
    : _pipe_name_in(pipe_name + "_in"), _pipe_name_out(pipe_name + "_out"),
      _is_server(is_server), _ready(false) {
    if (_is_server) {
        createPipes();
    }
    openPipes();
}

Plazza::NamedPipeIPC::~NamedPipeIPC()
{
    if (_is_server) {
        unlink(_pipe_name_in.c_str());
        unlink(_pipe_name_out.c_str());
    }
}

void Plazza::NamedPipeIPC::createPipes()
{
    if (mkfifo(_pipe_name_in.c_str(), 0666) == -1) {
        if (errno != EEXIST) {
            std::cerr << "Error creating pipe " << _pipe_name_in << ": " << strerror(errno) << std::endl;
        }
    }
    if (mkfifo(_pipe_name_out.c_str(), 0666) == -1) {
        if (errno != EEXIST) {
            std::cerr << "Error creating pipe " << _pipe_name_out << ": " << strerror(errno) << std::endl;
        }
    }
}

void Plazza::NamedPipeIPC::openPipes()
{
    if (_is_server) {
        _fd_read = open(_pipe_name_in.c_str(), O_RDONLY | O_NONBLOCK);
        _fd_write = open(_pipe_name_out.c_str(), O_WRONLY);
    } else {
        _fd_write = open(_pipe_name_in.c_str(), O_WRONLY);
        _fd_read = open(_pipe_name_out.c_str(), O_RDONLY | O_NONBLOCK);
    }
    
    _ready = (_fd_read != -1 && _fd_write != -1);
    if (!_ready) {
        std::cerr << "Failed to open pipes: " << strerror(errno) << std::endl;
    }
}

bool Plazza::NamedPipeIPC::send(const Message& msg)
{
    if (!_ready || _fd_write == -1) 
        return false;
    
    std::string serialized = msg.serialize();
    uint32_t length = serialized.length();

    if (write(_fd_write, &length, sizeof(length)) != sizeof(length)) {
        return false;
    }
    if (write(_fd_write, serialized.c_str(), length) != static_cast<ssize_t>(length)) {
        return false;
    }
    return true;
}

bool Plazza::NamedPipeIPC::receive(Message& msg)
{
    if (!_ready || _fd_read == -1) 
        return false;

    uint32_t length;
    ssize_t bytes_read = read(_fd_read, &length, sizeof(length));
    if (bytes_read != sizeof(length)) {
        if (bytes_read == -1 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
            return false;
        }
        return false;
    }

    std::string buffer(length, '\0');
    bytes_read = read(_fd_read, &buffer[0], length);
    if (bytes_read != static_cast<ssize_t>(length)) {
        return false;
    }
    msg.deserialize(buffer);
    return true;
}

bool Plazza::NamedPipeIPC::isReady() const
{
    return _ready;
}