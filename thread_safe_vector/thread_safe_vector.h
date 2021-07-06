#pragma once

#include <atomic>
#include <mutex>
#include <shared_mutex>
#include <vector>


template <typename T>
class ThreadSafeVector {
private:
    mutable std::shared_mutex read_mutex;
    mutable std::mutex write_mutex;
    std::atomic<int> vector_size = 0;
    std::vector<T> vector_;
    bool is_reallocating = false;

public:
    ThreadSafeVector() = default;

    T operator[](size_t index) const {
        std::shared_lock<std::shared_mutex> read_lock(read_mutex);
        return vector_[index];
    }

    size_t Size() const {
        return vector_.size();
    }

    void PushBack(const T& value) {
        std::unique_lock<std::mutex> write_lock(write_mutex);
        if (vector_.size() == vector_.capacity()) {
            std::unique_lock<std::shared_mutex> read_lock(read_mutex);
            vector_.push_back(value);
        } else {
            vector_.push_back(value);
        }
    }
};
