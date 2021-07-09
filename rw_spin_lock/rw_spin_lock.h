#pragma once

#include <atomic>
#include <thread>

class RWSpinLock {
private:
    std::atomic<uint64_t> lock_counter_{0};

public:
    RWSpinLock() = default;

    void LockRead() {
        for (;;) {
            uint64_t curent_lock = lock_counter_.load();
            if (curent_lock & 1) {
                std::this_thread::yield();
                continue;
            }
            if (lock_counter_.compare_exchange_weak(curent_lock, curent_lock + 2)) {
                break;
            }
        }
    }

    void UnlockRead() {
        lock_counter_ -= 2;
    }

    void LockWrite() {
        uint64_t curent_lock;
        for (;;) {
            curent_lock = lock_counter_.load();
            if (curent_lock & 1) {
                std::this_thread::yield();
                continue;
            }
            if (lock_counter_.compare_exchange_weak(curent_lock, curent_lock + 1)) {
                break;
            }
        }
        while (lock_counter_.load() != 1) {
            std::this_thread::yield();
        }
    }

    void UnlockWrite() {
        lock_counter_ -= 1;
    }
};
