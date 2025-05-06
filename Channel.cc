#include "Channel.h"
#include "EventLoop.h"
#include "logger.h"

Channel::Channel(EventLoop* loop, int fd)
    : loop_(loop), fd_(fd), events_(0), revents_(0), index_(-1), tied_(false) {}

void Channel::handleEvent(Timestamp receiveTime) {
    if (tied_) {
        // 增加TcpConnection对象的引用计数，防止对象销毁导致Channel对象销毁，带来未定义行为
        std::shared_ptr<void> guard = tie_.lock();
        if (guard) {
            handleEventWithGuard(receiveTime);
        }
    } else {
        handleEventWithGuard(receiveTime);
    }
}

void Channel::handleEventWithGuard(Timestamp receiveTime) {
    LOG_INFO("channel handleEvent revents:%d\n", revents_);

    // 触发关闭连接回调
    if ((revents_ & EPOLLHUP) && !(revents_ & EPOLLIN)) {
        closeCallback_();
    }

    // 触发读回调
    if ((revents_ & EPOLLIN) || (revents_ & EPOLLPRI)) {
        readCallback_(receiveTime);
    }

    // 触发错误事件处理回调
    if (revents_ & EPOLLERR) {
        errorCallback_();
    }

    // 触发写回调
    if (revents_ & EPOLLOUT) {
        writeCallback_();
    }
}

void Channel::update() {
    loop_->updateChannel(this);
}

void Channel::remove() {
    loop_->removeChannel(this);
}