#include "Acceptor.h"
#include <sys/socket.h>
#include <unistd.h>
#include "InteAddress.h"
#include "logger.h"

static int createNonblockFd() {
    int fd = ::socket(AF_INET, SOCK_STREAM | SOCK_CLOEXEC | SOCK_NONBLOCK, 0);
    if (fd < 0) {
        LOG_FATAL("listen socket create err: %d", errno);
    }
    return fd;
}

Acceptor::Acceptor(EventLoop* loop, const InteAddress& listenAddr, bool reuseport)
    : loop_(loop),
      acceptSocket_(createNonblockFd()),
      acceptChannel_(loop_, acceptSocket_.fd()),
      listenning_(false) {
    acceptSocket_.setReuseAddr(true);
    acceptSocket_.setReusePort(true);
    acceptSocket_.bindAddress(listenAddr);  // bind
    acceptChannel_.setReadCallback(std::bind(&Acceptor::handleRead, this));
}

Acceptor::~Acceptor() {
    acceptChannel_.disableAll();
    acceptChannel_.remove();
}

void Acceptor::listen() {
    listenning_ = true;
    acceptSocket_.listen();
    acceptChannel_.enableReading();
}

void Acceptor::handleRead() {
    InteAddress peerAddr;
    int connfd = acceptSocket_.accept(&peerAddr);
    if (connfd > 0) {
        if (newConnectionCallback_) {
            newConnectionCallback_(connfd, peerAddr);
        } else {
            ::close(connfd);
        }
    } else {
        LOG_ERROR("%s:%s:%d accept err:%d \n", __FILE__, __FUNCTION__, __LINE__, errno);
        if (errno == EMFILE) {
            LOG_ERROR("%s:%s:%d sockfd reached limit! \n", __FILE__, __FUNCTION__, __LINE__);
        }
    }
}