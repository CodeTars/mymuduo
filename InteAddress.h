#pragma once
#include <netinet/in.h>
#include <string>

// 封装socket地址
class InteAddress {
public:
    explicit InteAddress(uint16_t port = 0, const std::string& ip = "127.0.0.1");
    explicit InteAddress(const sockaddr_in& addr) : addr_(addr) {}
    // 获取ip
    std::string ip() const;
    // 获取端口
    uint16_t port() const;
    // 获取ip和端口
    std::string ip_port() const;
    // 获取sockaddr
    const sockaddr* get_sockaddr() const { return reinterpret_cast<const sockaddr*>(&addr_); }
    // 设置sockaddr
    void set_sockaddr(const sockaddr_in& addr) { addr_ = addr; }

private:
    sockaddr_in addr_;
};