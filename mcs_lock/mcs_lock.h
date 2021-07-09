#pragma once

#include <atomic>
#include <thread>
#include <iostream>

struct Node {
    Node* next{nullptr};
    bool is_locked{false};
};

class MCSLock {
private:
    std::atomic<Node*> tail{nullptr};
    thread_local static inline Node node_;

public:
    MCSLock() = default;

    void Lock() {
        Node* prev = tail.exchange(&node_);
        if (prev) {
            prev->next = &node_;
            node_.is_locked = true;
        } else {
            return;
        }
        while (node_.is_locked) {
            std::this_thread::yield();
        }
    }

    void Unlock() {
        Node* node_pointer = &node_;
        if (tail.compare_exchange_weak(node_pointer, nullptr)) {
            return;
        }
        while (!node_.next) {
            std::this_thread::yield();
        }
        node_.next->is_locked = false;
    }
};
