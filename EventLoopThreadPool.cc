#include "EventLoopThreadPool.h"
#include "EventLoopThread.h"

EventLoopThreadPool::EventLoopThreadPool(EventLoop* baseLoop, const std::string& nameArg)
    : baseLoop_(baseLoop), name_(nameArg), started_(false), numThreads_(0), next_(0) {}

EventLoopThreadPool::~EventLoopThreadPool() {}

void EventLoopThreadPool::start(const ThreadInitCallback& cb) {
    started_ = true;
    for (int i = 0; i < numThreads_; i++) {
        std::string tname = name_ + std::to_string(i);
        auto t = std::make_unique<EventLoopThread>(cb, tname);
        loops_.push_back(t->startLoop());
        threads_.push_back(std::move(t));
    }

    if (numThreads_ == 0 && cb && baseLoop_) {
        cb(baseLoop_);
    }
}

EventLoop* EventLoopThreadPool::getNextLoop() {
    EventLoop* loop = baseLoop_;

    if (!loops_.empty()) {
        loop = loops_[next_++];
        if (next_ == loops_.size()) {
            next_ = 0;
        }
    }

    return loop;
}

std::vector<EventLoop*> EventLoopThreadPool::getAllLoops() {
    if (loops_.empty()) {
        return {baseLoop_};
    }

    return loops_;
}