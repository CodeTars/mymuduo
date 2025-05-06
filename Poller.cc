#include "Poller.h"
#include <cassert>
#include "Channel.h"

Poller::Poller(EventLoop* loop) : ownerLoop_(loop) {}

bool Poller::hasChannel(Channel* channel) const {
    return channels_.count(channel->fd()) && channels_.at(channel->fd()) == channel;
}
