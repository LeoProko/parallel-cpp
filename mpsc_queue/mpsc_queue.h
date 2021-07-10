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
    }

    void Push(const T& value) {
        Node* node = new Node;
        node->value = value;
        for (;;) {
            node->next = head_;
            if (head_.compare_exchange_weak(node->next, node)) {
                return;
            }
        }
    }

    std::optional<T> Pop() {
        Node* node_to_remove;
        for (;;) {
            node_to_remove = head_.load();
            if (!node_to_remove || head_.compare_exchange_weak(node_to_remove, node_to_remove->next)) {
                break;
            }
        }
        if (node_to_remove) {
            T return_value = node_to_remove->value;
            delete node_to_remove;
            return return_value;
        }
        return std::nullopt;
    }
};

