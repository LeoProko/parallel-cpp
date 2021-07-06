#pragma once

#include <optional>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>


template <typename T>
class ThreadSafeQueue {
private:
    // Your code
    std::queue<T> queue_;
    std::mutex queue_mutex_;
    std::condition_variable condition_variable;

public:
    ThreadSafeQueue() = default;

    void Push(const T& value) {
        std::unique_lock<std::mutex> lock(queue_mutex_);
        queue_.push(value);
        condition_variable.notify_one();
    }

    T Pop() {
        std::unique_lock<std::mutex> lock(queue_mutex_);
        while (queue_.empty()) {
            condition_variable.wait(lock);
        }
        auto value = queue_.front();
        queue_.pop();
        return value;
    }

    std::optional<T> TryPop() {
        std::unique_lock<std::mutex> lock(queue_mutex_);
        if (queue_.empty()) {
          return std::nullopt;
        }
        auto value = queue_.front();
        queue_.pop();
        return value;
    }
};

