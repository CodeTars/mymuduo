#include "EpollPoller.h"
#include <string.h>
#include <unistd.h>
#include <cassert>
#include "Channel.h"
#include "logger.h"

const int kNew = -1;
const int kAdded = 1;
const int kDeleted = 2;

EpollPoller::EpollPoller(EventLoop* loop)
    : Poller(loop), epollfd_(epoll_create1(EPOLL_CLOEXEC)), events(kInitEventListSize) {
    if (epollfd_ < 0) {
        LOG_FATAL("epoll_create error:%d \n", errno);
    }
}

EpollPoller::~EpollPoller() {
    ::close(epollfd_);
}

Timestamp EpollPoller::poll(int timeoutMs, std::vector<Channel*>* activeChannels) {
    LOG_INFO("func = %s, fd total count: %lu\n", __FUNCTION__, channels_.size());

    int numEvents = ::epoll_wait(epollfd_, &*events.begin(), events.size(), timeoutMs);
    int saveErrno = errno;
    Timestamp t = Timestamp::now();

    if (numEvents > 0) {
        LOG_INFO("%d events happened\n", numEvents);
        fillActiveChannels(numEvents, activeChannels);
        if (numEvents == events.size()) {
            events.resize(2 * events.size());
        }
    } else if (numEvents == 0) {
        LOG_DEBUG("%s timeout!\n", __FUNCTION__);
    } else {
        if (saveErrno != EINTR) {
            errno = saveErrno;
            LOG_ERROR("EPollPoller::poll() err!");
        }
    }
    return t;
}

void EpollPoller::updateChannel(Channel* channel) {
    LOG_DEBUG("func=%s => fd=%d events=%d index=%d\n", __FUNCTION__, channel->fd(),
              channel->events(), channel->index());

    if (channel->index() == kNew || channel->index() == kDeleted) {
        assert(!channel->isNoneEvent());
        if (channel->index() == kNew) {
            channels_[channel->fd()] = channel;
        }
        channel->set_index(kAdded);
        update(EPOLL_CTL_ADD, channel);
    } else {
        // kAdded
        if (channel->isNoneEvent()) {
            channel->set_index(kDeleted);
            update(EPOLL_CTL_DEL, channel);
        } else {
            update(EPOLL_CTL_MOD, channel);
        }
    }
}

void EpollPoller::removeChannel(Channel* channel) {
    channels_.erase(channel->fd());

    if (channel->index() == kAdded) {
        update(EPOLL_CTL_DEL, channel);
    }
    channel->set_index(kNew);
}

void EpollPoller::fillActiveChannels(int numEvents, std::vector<Channel*>* activeChannels) const {
    for (const auto& event : events) {
        Channel* channel = static_cast<Channel*>(event.data.ptr);
        channel->set_revents(event.events);
        activeChannels->push_back(channel);
    }
}

void EpollPoller::update(int operation, Channel* channel) {
    // 监视对象
    epoll_event event;
    memset(&event, 0, sizeof event);
    event.events = channel->events();
    event.data.ptr = static_cast<void*>(channel);

    // 注册
    if (::epoll_ctl(epollfd_, operation, channel->fd(), &event) < 0) {
        if (operation == EPOLL_CTL_DEL) {
            LOG_ERROR("epoll_ctl del error:%d\n", errno);
        } else {
            LOG_FATAL("epoll_ctl add/mod error:%d\n", errno);
        }
    }
}