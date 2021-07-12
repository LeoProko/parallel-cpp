#pragma once

#include <atomic>
#include <optional>

template <typename T>
class MPSCQueue {
private:
    struct Node {
        T value;
        Node* next;
    };
    
    std::atomic<Node*> head_ = nullptr;

public:
    MPSCQueue() = default;

    ~MPSCQueue() {
        while (Pop());
    }

    void Push(const T& value) {
        Node* node = new Node;
        node->value = value;
        do {
            node->next = head_;
        } while (!head_.compare_exchange_weak(node->next, node));
    }

    std::optional<T> Pop() {
        Node* node_to_remove;
        do {
            node_to_remove = head_.load();
        } while (node_to_remove && !head_.compare_exchange_weak(node_to_remove, node_to_remove->next));
        if (node_to_remove) {
            T return_value = node_to_remove->value;
            delete node_to_remove;
            return return_value;
        }
        return std::nullopt;
    }
};

