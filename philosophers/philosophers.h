#pragma once

#include <mutex>


class Fork {
public:
    Fork(size_t id)
        : id_(id) {}

    size_t Id() const {
        return id_;
    }

    void Get() {
        mutex.lock();
    }

    bool TryGet() {
        return mutex.try_lock();
    }

    void Put() {
        mutex.unlock();
    }

private:
    size_t id_;
    std::mutex mutex;
};

class Philosopher {
public:
    Philosopher(size_t id_, Fork* left_fork_, Fork* right_fork_)
        : id(id_)
        , left_fork(left_fork_)
        , right_fork(right_fork_) {}

    size_t Id() const {
        return id;
    }

    void Eat() {
        if (id == 1) {
            while (!right_fork->TryGet());
            while (!left_fork->TryGet());
            return;
        }
        while (!left_fork->TryGet());
        while (!right_fork->TryGet());
    }

    void Think() {
        left_fork->Put();
        right_fork->Put();
    }

private:
    size_t id;
    Fork* left_fork;
    Fork* right_fork;
};

