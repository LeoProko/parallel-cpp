#pragma once

#include <atomic>
#include <thread>

class SpinLock {
private:
    std::atomic<bool> is_lock_{false};

public:
    SpinLock() = default;

    void Lock() {
        while (is_lock_.load() || is_lock_.exchange(true)) {
            std::this_thread::yield();
        }
    }

    void Unlock() {
        is_lock_.store(false);
    }
};

