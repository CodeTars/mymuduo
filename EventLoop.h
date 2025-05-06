#pragma once
#include <atomic>
#include <functional>
#include <memory>
#include <mutex>
#include <vector>
#include "CurrentThread.h"
#include "Timestamp.h"
#include "noncopyable.h"

class Channel;
class Poller;

// 事件循环，事件分发器
class EventLoop : noncopyable {
public:
    using Functor = std::function<void()>;
    EventLoop();
    ~EventLoop();

    // 开启事件循环
    void loop();
    // 退出事件循环
    void quit();

    // 执行cb
    void runInLoop();
    void queueInLoop();

    // 主要是mainLoop唤醒阻塞的其它subLoop
    void wakeup();

    void updateChannel(Channel* channel);
    void removeChannel(Channel* channel);
    void hasChannel(Channel* channel);

    // 判断EventLoop对象是否在当前线程
    bool isInLoopThread() const { return threadId_ == CurrentThread::tid(); }

private:
    // 处理eventfd事件
    void handleRead();
    // 执行回调
    void doPendingFunctors();

    std::atomic<bool> looping_;
    std::atomic<bool> quit_;

    const pid_t threadId_;

    Timestamp pollReturnTime_;

    std::unique_ptr<Poller> poller_;

    int wakeupFd_;
    std::unique_ptr<Channel> wakeupChannel_;

    std::vector<Channel*> activeChannels_;

    std::atomic<bool> callingPendingFunctors_;
    std::vector<Functor> pendingFunctors_;
    std::mutex mutex_;
};
