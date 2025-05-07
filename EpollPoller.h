#pragma once

#include <sys/epoll.h>
#include "Poller.h"

class EpollPoller : public Poller {
public:
    EpollPoller(EventLoop* loop);
    ~EpollPoller() override;

    Timestamp poll(int timeoutMs, std::vector<Channel*>* activeChannels);
    void updateChannel(Channel* channel);
    void removeChannel(Channel* channel);

private:
    static const int kInitEventListSize = 16;

    // 填写活跃事件
    void fillActiveChannels(int numEvents, std::vector<Channel*>* activeChannels) const;
    // 注册监视fd
    void update(int operation, Channel* channel);

    int epollfd_;
    std::vector<epoll_event> events;
};