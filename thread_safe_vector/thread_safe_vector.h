#pragma once

#include <atomic>
#include <mutex>
#include <shared_mutex>
#include <vector>


template <typename T>
class ThreadSafeVector {
private:
    mutable std::shared_mutex shared_mutex;
    std::atomic<int> vector_size = 0;
    std::vector<T> vector;
    bool is_reallocating = false;

public:
    ThreadSafeVector() = default;

    T operator[](size_t index) const {
        std::shared_lock<std::shared_mutex> shared_lock(shared_mutex);
        while (is_reallocating);
        return vector[index];
    }

    size_t Size() const {
        std::shared_lock<std::shared_mutex> shared_lock(shared_mutex);
        return vector_size;
    }

    void PushBack(const T& value) {
        std::shared_lock<std::shared_mutex> shared_lock(shared_mutex);
        if (vector.size() == vector.capacity()) {
            is_reallocating = true;
        }
        vector.push_back(value);
        ++vector_size;
        is_reallocating = false;
    }
};
