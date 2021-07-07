#pragma once

#include <atomic>
#include <thread>

struct Node {
    std::atomic<Node*> next{nullptr};
    std::atomic<bool> is_lock{false};
};

class MCSLock {
private:
    std::atomic<Node*> tail_{nullptr};
    Node node_;

public:
    MCSLock() = default;

    void Lock() {
        Node* prev_node_ = tail_.exchange(&node_);
        prev_node_->next = &node_;
        while(!node_.is_lock) {
            std::this_thread::yield();
        }
    }

    void Unlock() {
        Node* node_link = &node_;
        if (tail_.load()) {
            node_.next.load()->is_lock.store(true);
            return;
        }
        if (tail_.compare_exchange_weak(node_link, nullptr)) {
            return;
        }
        while (node_.next.load() == nullptr);
        node_.next.load()->is_lock.store(true);
    }
};
