#pragma once

#include <chrono>
#include <thread>
#include <mutex>

class Deadlock {
private:
    std::mutex mutex_1_;
    std::mutex mutex_2_;
public:
    Deadlock() {
    }

    void ThreadOne() {
        std::unique_lock<std::mutex> lock_1(mutex_1_);
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        std::unique_lock<std::mutex> lock_2(mutex_2_);
    }

    void ThreadTwo() {
        std::unique_lock<std::mutex> lock_2(mutex_2_);
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        std::unique_lock<std::mutex> lock_1(mutex_1_);
    }
};

