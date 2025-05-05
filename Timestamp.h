#pragma once
#include <stdint.h>
#include <string>

// 时间戳
class Timestamp {
public:
    explicit Timestamp(int64_t t = 0) : t_(t) {}
    static Timestamp now();
    std::string to_string();

private:
    int64_t t_;
};