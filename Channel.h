#pragma once
#include <sys/epoll.h>
#include <functional>
#include <memory>
#include "Timestamp.h"
#include "noncopyable.h"

class EventLoop;

// 事件处理器Handler
class Channel : noncopyable {
public:
    using EventCallback = std::function<void()>;
    using ReadEventCallback = std::function<void(Timestamp)>;

    Channel(EventLoop* loop, int fd);
    ~Channel();

    // 处理事件
    void handleEvent();

    // 设置回调函数
    void setReadCallback(ReadEventCallback cb) { readCallback_ = std::move(cb); }
    void setWriteCallback(EventCallback cb) { writeCallback_ = std::move(cb); }
    void setCloseCallback(EventCallback cb) { closeCallback_ = std::move(cb); }
    void setErrorCallback(EventCallback cb) { errorCallback_ = std::move(cb); }

    void tie(const std::shared_ptr<void>& obj) {
        tie_ = obj;
        tied_ = true;
    }

    int fd() const { return fd_; }
    int events() const { return events_; }
    void set_revents(int revt) { revents_ = revt; }

    // 设置fd感兴趣的事件类型
    void enableReading() {
        events_ |= kReadEvent;
        update();
    }
    void disableReading() {
        events_ &= ~kReadEvent;
        update();
    }
    void enableWriting() {
        events_ |= kWriteEvent;
        update();
    }
    void disableWriting() {
        events_ &= ~kWriteEvent;
        update();
    }
    void disableAll() {
        events_ = kNoneEvent;
        update();
    }

    // 返回fd当前的事件状态
    bool isWriting() const { return events_ & kWriteEvent; }
    bool isReading() const { return events_ & kReadEvent; }
    bool isNoneEvent() const { return events_ == kNoneEvent; }

    int index() const { return index_; }
    void set_index(int idx) { index_ = idx; }

    EventLoop* ownerLoop() { return loop_; }
    void remove();  // 从epoll中删除fd

private:
    void update();
    void handleEventWithGuard(Timestamp receiveTime);

    static const int kNoneEvent = 0;
    static const int kReadEvent = EPOLLIN | EPOLLPRI;
    static const int kWriteEvent = EPOLLOUT;

    EventLoop* loop_;  // 所属的事件循环
    const int fd_;     // 文件描述符
    int events_;       // 事件类型
    int revents_;      // 触发的事件
    int index_;        // 在epoll中的索引

    std::weak_ptr<void> tie_;  // 用于防止回调函数访问已释放的对象
    bool tied_;                // 是否绑定了对象

    EventCallback readCallback_;   // 读事件回调
    EventCallback writeCallback_;  // 写事件回调
    EventCallback closeCallback_;  // 关闭事件回调
    EventCallback errorCallback_;  // 错误事件回调
};