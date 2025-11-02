#ifndef EDACAL_STACK_HPP
#define EDACAL_STACK_HPP

#include "linked_list.hpp"
#include <utility>

namespace edacal {

template <typename T>
class Stack {
public:
    Stack() = default;

    void push(const T& value) {
        data_.push_front(value);
    }

    void push(T&& value) {
        data_.push_front(std::move(value));
    }

    void pop() {
        data_.pop_front();
    }

    T& top() {
        return data_.front();
    }

    const T& top() const {
        return data_.front();
    }

    bool empty() const {
        return data_.empty();
    }

    std::size_t size() const {
        return data_.size();
    }

    void clear() {
        data_.clear();
    }

private:
    LinkedList<T> data_;
};

} // namespace edacal

#endif
