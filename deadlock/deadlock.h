#pragma once

#include <chrono>
#include <thread>
#include <mutex>

class Deadlock {
private:
    std::mutex mutex_1;
    std::mutex mutex_2;
public:
    Deadlock() {
    }

    void ThreadOne() {
        std::unique_lock<std::mutex> lock_1(mutex_1);
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        std::unique_lock<std::mutex> lock_2(mutex_2);
    }

    void ThreadTwo() {
        std::unique_lock<std::mutex> lock_2(mutex_2);
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        std::unique_lock<std::mutex> lock_1(mutex_1);
    }
};

