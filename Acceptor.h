#pragma once
#include "Channel.h"
#include "Socket.h"
#include "noncopyable.h"

#include <functional>

class EventLoop;
class InteAddress;

class Acceptor : noncopyable {
public:
    using NewConnectionCallback = std::function<void(int sockfd, const InteAddress&)>;
    Acceptor(EventLoop* loop, const InteAddress& listenAddr, bool reuseport);
    ~Acceptor();

    void setNewConnectionCallback(const NewConnectionCallback& cb) { newConnectionCallback_ = cb; }

    bool listenning() const { return listenning_; }
    void listen();

private:
    void handleRead();

    EventLoop* loop_;
    Socket acceptSocket_;
    Channel acceptChannel_;
    NewConnectionCallback newConnectionCallback_;
    bool listenning_;
};