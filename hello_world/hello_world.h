#pragma once

#include <ostream>
#include <string_view>
#include <thread>
#include <mutex>
#include <vector>


class HelloWorld {
private:
    std::vector<std::thread> threads;
    std::mutex mutex;

    const size_t num_threads;

    void hello_world_print(std::ostream& os) {
        std::unique_lock<std::mutex> lock(mutex);
        os << kHelloPrefix << std::this_thread::get_id() << "\n";
    }

public:
    static constexpr std::string_view kHelloPrefix = "Hello, World! From thread ";

    HelloWorld(size_t n_threads)
        : num_threads(n_threads) {}

    void SayHello(std::ostream& os) {
        for (size_t i = 0; i != num_threads; ++i) {
            threads.emplace_back([&] {
                        hello_world_print(os);
                    });
        }
        for (auto& thread : threads) {
            thread.join();
        }
    }
};

