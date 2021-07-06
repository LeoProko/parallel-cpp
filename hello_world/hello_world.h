#pragma once

#include <ostream>
#include <string_view>
#include <thread>
#include <mutex>
#include <vector>


class HelloWorld {
private:
    std::mutex mutex_;
    const size_t num_threads_;

    void hello_world_print(std::ostream& os) {
        std::unique_lock<std::mutex> lock(mutex_);
        os << kHelloPrefix << std::this_thread::get_id() << "\n";
    }

public:
    static constexpr std::string_view kHelloPrefix = "Hello, World! From thread ";

    HelloWorld(size_t num_threads)
        : num_threads_(num_threads) {}

    void SayHello(std::ostream& os) {
        std::vector<std::thread> threads;
        for (size_t i = 0; i != num_threads_; ++i) {
            threads.emplace_back([&] {
                        hello_world_print(os);
                    });
        }
        for (auto& thread : threads) {
            thread.join();
        }
    }
};

