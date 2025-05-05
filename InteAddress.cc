#include "InteAddress.h"
#include <arpa/inet.h>

InteAddress::InteAddress(uint16_t port, const std::string& ip) {
    addr_.sin_family = AF_INET;
    addr_.sin_port = htons(port);
    addr_.sin_addr.s_addr = inet_addr(ip.c_str());
}

std::string InteAddress::ip() const {
    char ip[16];
    inet_ntop(AF_INET, &addr_.sin_addr, ip, sizeof(ip));
    return std::string(ip);
}

uint16_t InteAddress::port() const {
    return ntohs(addr_.sin_port);
}

std::string InteAddress::ip_port() const {
    return ip() + ":" + std::to_string(port());
}

// #include <iostream>

// int main() {
//     InteAddress address(8080, "192.168.10.1");
//     std::cout << address.ip_port() << std::endl;
//     return 0;
// }