#pragma once

#include <atomic>
#include <mutex>
#include <shared_mutex>
#include <vector>


template <typename T>
class ThreadSafeVector {
private:
    mutable std::shared_mutex read_mutex_;
    mutable std::mutex write_mutex_;
    std::vector<T> vector_;
    int vector_size_ = 0;

public:
    ThreadSafeVector() = default;

    T operator[](size_t index) const {
        std::shared_lock<std::shared_mutex> read_lock(read_mutex_);
        return vector_[index];
    }

    size_t Size() const {
        std::shared_lock<std::shared_mutex> read_lock(read_mutex_);
        return vector_size_;
    }

    void PushBack(const T& value) {
        std::unique_lock<std::mutex> write_lock(write_mutex_);
        if (vector_.size() == vector_.capacity()) {
            std::unique_lock<std::shared_mutex> read_lock(read_mutex_);
            vector_.push_back(value);
        } else {
            vector_.push_back(value);
        }
        ++vector_size_;
    }
};
