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
    std::queue<T> queue;
    std::mutex mutex;
    std::condition_variable condition_variable;

public:
    ThreadSafeQueue() {
    }

    void Push(const T& value) {
        std::unique_lock<std::mutex> lock(mutex);
        queue.push(value);
        condition_variable.notify_one();
    }

    T Pop() {
        std::unique_lock<std::mutex> lock(mutex);
        while (queue.empty()) {
            condition_variable.wait(lock);
        }
        auto value = queue.front();
        queue.pop();
        return value;
    }

    std::optional<T> TryPop() {
        std::unique_lock<std::mutex> lock(mutex);
        if (queue.empty()) {
          return std::nullopt;
        }
        auto value = queue.front();
        queue.pop();
        return value;
    }
};

