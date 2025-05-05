#include <iostream>
#include <mutex>
#include <thread>
std::mutex cout_mutex;  //方便多线程打印， 加锁指示为了方便多线程打印

thread_local int x = 1;

void thread_func(const std::string& thread_name) {
    for (int i = 0; i < 50; i++) {
        x++;
        std::lock_guard<std::mutex> lock(cout_mutex);
        std::cout << "thread[" << thread_name << "]: x = " << x << std::endl;
    }
}

int main() {
    std::thread t2(thread_func, "t2");
    std::thread t1(thread_func, "t1");
    t1.join();
    t2.join();
    return 0;
}
