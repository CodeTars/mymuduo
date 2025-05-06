#pragma once

#include <unordered_map>
#include <vector>
#include "Timestamp.h"
#include "noncopyable.h"

class EventLoop;
class Channel;

// IO多路复用器
class Poller : noncopyable {
public:
    Poller(EventLoop* loop);
    virtual ~Poller() = default;

    // 为所有IO复用提供统一接口
    virtual Timestamp poll(int timeoutMs, std::vector<Channel*>* activeChannels) = 0;
    virtual void updateChannel(Channel* channel) = 0;
    virtual void removeChannel(Channel* channel) = 0;

    bool hasChannel(Channel* channel) const;

    static Poller* newDefaultPoller(EventLoop* loop);

protected:
    std::unordered_map<int, Channel*> channels_;

private:
    EventLoop* ownerLoop_;
};
