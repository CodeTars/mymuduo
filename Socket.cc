#include "Socket.h"
#include <netinet/tcp.h>
#include <string.h>
#include <unistd.h>
#include "InteAddress.h"
#include "logger.h"

Socket::~Socket() {
    ::close(sockfd_);
}

void Socket::bindAddress(const InteAddress& localaddr) {
    if (0 != ::bind(sockfd_, localaddr.get_sockaddr(), sizeof(sockaddr))) {
        LOG_FATAL("Fail to bind address: %s\n", localaddr.ip_port().c_str());
    }
}

void Socket::listen() {
    if (0 != ::listen(sockfd_, 1024)) {
        LOG_FATAL("Fail to listen: %d\n", sockfd_);
    }
}

int Socket::accept(InteAddress* peeraddr) {
    sockaddr_in addr;
    socklen_t len = sizeof(addr);
    memset(&addr, 0, sizeof addr);

    int connfd =
        accept4(sockfd_, reinterpret_cast<sockaddr*>(&addr), &len, SOCK_NONBLOCK | SOCK_CLOEXEC);
    if (connfd >= 0) {
        peeraddr->set_sockaddr(addr);
    } else {
        LOG_FATAL("Fail to accept!");
    }

    return connfd;
}

void Socket::shutdownWrite() {
    if (::shutdown(sockfd_, SHUT_WR) < 0) {
        LOG_ERROR("shutdownWrite error");
    }
}

void Socket::setTcpNoDelay(bool on) {
    int optval = on ? 1 : 0;
    ::setsockopt(sockfd_, IPPROTO_TCP, TCP_NODELAY, &optval, sizeof optval);
}

void Socket::setReuseAddr(bool on) {
    int optval = on ? 1 : 0;
    ::setsockopt(sockfd_, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);
}

void Socket::setReusePort(bool on) {
    int optval = on ? 1 : 0;
    ::setsockopt(sockfd_, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof optval);
}

void Socket::setKeepAlive(bool on) {
    int optval = on ? 1 : 0;
    ::setsockopt(sockfd_, SOL_SOCKET, SO_KEEPALIVE, &optval, sizeof optval);
}