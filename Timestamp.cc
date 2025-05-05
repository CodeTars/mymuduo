#include "Timestamp.h"
#include <iomanip>
#include <iostream>
#include <sstream>
#include "time.h"

Timestamp Timestamp::now() {
    return Timestamp(time(nullptr));
}

std::string Timestamp::to_string() {
    tm* timeInfo = localtime(&t_);

    std::ostringstream oss;
    oss << std::put_time(timeInfo, "[%Y-%m-%d %H:%M:%S]");
    return oss.str();
}

// int main() {
//     std::cout << Timestamp::now().to_string() << std::endl;
//     return 0;
// }