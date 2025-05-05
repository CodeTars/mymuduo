#pragma once
#include <sys/epoll.h>
#include <functional>
#include "Timestamp.h"
#include "noncopyable.h"

class EventLoop;

// Channel是一个类，表示文件描述符和事件的关联
class Channel : noncopyable {
public:
    Channel(EventLoop* loop, int fd);
    ~Channel();

    void handleEvent();
    // void setReadCallback(const std::function<void()>& cb) { readCallback_ = cb; }
    // void setWriteCallback(const std::function<void()>& cb) { writeCallback_ = cb; }
    // void setErrorCallback(const std::function<void()>& cb) { errorCallback_ = cb; }

private:
    void update();
    void handleEventWithGuard(Timestamp receiveTime);

    static const int kNoneEvent = 0;
    static const int kReadEvent = EPOLLIN | EPOLLPRI;
    static const int kWriteEvent = EPOLLOUT;

    EventLoop* loop_;  // 所属的事件循环
    const int fd_;     // 文件描述符
    int events_;       // 事件类型
};