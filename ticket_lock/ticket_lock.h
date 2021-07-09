#pragma once

#include <atomic>
#include <thread>

class TicketLock {
private:
    std::atomic<uint64_t> current_ticket_{0};
    std::atomic<uint64_t> next_ticket_{0};

public:
    TicketLock() = default;

    void Lock() {
        int my_ticket_ = next_ticket_.fetch_add(1);
        while (current_ticket_.load() != my_ticket_) {
            std::this_thread::yield();
        }
        
    }

    void Unlock() {
        current_ticket_.fetch_add(1);
    }
};

