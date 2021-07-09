#pragma once

#include <atomic>
#include <thread>

struct Node {
    std::atomic<Node*> next;
    std::atomic<bool> acq;
};

class MCSLock {
private:
    std::atomic<Node*> tail{nullptr};
    thread_local static Node node_;

public:
    MCSLock() = default;

    void Lock() {
        Node* prev = tail.exchange(&node_);
        prev->next = &node_;
        while (!node_.acq) {
            std::this_thread::yield();
        }
    }

    void Unlock() {
        if (tail.load() != nullptr) {
            node_.next.load()->acq = true;
            return;
        }
        Node* node_pointer = &node_;
        if (tail.compare_exchange_weak(node_pointer, nullptr)) {
            return;
        }
        while (!node_.next.load()) {
            std::this_thread::yield();
        }
        node_.next.load()->acq = true;
    }
};
