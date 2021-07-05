#pragma once

#include <atomic>
#include <cstddef>
#include <vector>
#include <thread>
#include <iterator>
#include <numeric>

template <typename RandomAccessIterator, class T, class Func>
T parallel_reduce(RandomAccessIterator begin,
                  RandomAccessIterator end,
                  const T& initial_value,
                  Func func,
                  size_t n_threads) {
    std::vector<std::thread> threads;
    int64_t sub_vector_size = std::distance(begin, end) / n_threads;
    std::vector<T> answer(n_threads);

    for (int num_current_thread = 0;
            begin != end; begin += sub_vector_size, ++num_current_thread) {
        if (std::distance(begin, end) >= 2 * sub_vector_size) {
            threads.emplace_back([&answer, num_current_thread, begin, sub_vector_size, func, initial_value] {
                answer[num_current_thread] = std::reduce(begin, begin + sub_vector_size, initial_value, func);
            });
        } else {
            threads.emplace_back([&answer, num_current_thread, begin, end, func, initial_value] {
                answer[num_current_thread] = std::reduce(begin, end, initial_value, func);
            });
            break;
        }
    }

    for (auto& thread : threads) {
        thread.join();
    }

    return std::reduce(answer.begin(), answer.end(), initial_value, func);
}
