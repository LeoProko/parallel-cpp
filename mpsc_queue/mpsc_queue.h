#pragma once

#include <atomic>
#include <optional>

template <typename T>
struct Node {
    T value;
    Node* next;
};


template <typename T>
class MPSCQueue {
private:
    std::atomic<Node<T>*> head_ = nullptr;

public:
    MPSCQueue() = default;

    ~MPSCQueue() {
    }

    void Push(const T& value) {
        Node<T>* node = new Node<T>;
        node->value = value;
        for (;;) {
            node->next = head_;
            if (head_.compare_exchange_weak(node->next, node)) {
                return;
            }
        }
    }

    std::optional<T> Pop() {
        Node<T>* node_to_remove;
        for (;;) {
            node_to_remove = head_.load();
            if (!head_ || (head_ && head_.compare_exchange_weak(node_to_remove, head_.load()->next))) {
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

