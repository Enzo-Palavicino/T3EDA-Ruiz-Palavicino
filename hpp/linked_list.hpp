#ifndef EDACAL_LINKED_LIST_HPP
#define EDACAL_LINKED_LIST_HPP

#include <cstddef>
#include <iterator>
#include <stdexcept>
#include <utility>

namespace edacal {

template <typename T>
class LinkedList {
private:
    struct Node {
        T data;
        Node* next;

        explicit Node(const T& value) : data(value), next(nullptr) {}
        explicit Node(T&& value) : data(std::move(value)), next(nullptr) {}
    };

    Node* head_;
    Node* tail_;
    std::size_t size_;

    void copyFrom(const LinkedList& other) {
        for (const auto& value : other) {
            push_back(value);
        }
    }

    void moveFrom(LinkedList&& other) noexcept {
        head_ = other.head_;
        tail_ = other.tail_;
        size_ = other.size_;
        other.head_ = nullptr;
        other.tail_ = nullptr;
        other.size_ = 0;
    }

public:
    class Iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;

        Iterator(Node* node) : current_(node) {}

        reference operator*() const { return current_->data; }
        pointer operator->() const { return &current_->data; }

        Iterator& operator++() {
            if (current_) {
                current_ = current_->next;
            }
            return *this;
        }

        Iterator operator++(int) {
            Iterator tmp(*this);
            ++(*this);
            return tmp;
        }

        bool operator==(const Iterator& other) const { return current_ == other.current_; }
        bool operator!=(const Iterator& other) const { return current_ != other.current_; }

    private:
        Node* current_;
    };

    class ConstIterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = const T*;
        using reference = const T&;

        ConstIterator(const Node* node) : current_(node) {}
        reference operator*() const { return current_->data; }
        pointer operator->() const { return &current_->data; }

        ConstIterator& operator++() {
            if (current_) {
                current_ = current_->next;
            }
            return *this;
        }

        ConstIterator operator++(int) {
            ConstIterator tmp(*this);
            ++(*this);
            return tmp;
        }

        bool operator==(const ConstIterator& other) const { return current_ == other.current_; }
        bool operator!=(const ConstIterator& other) const { return current_ != other.current_; }

    private:
        const Node* current_;
    };

    LinkedList() : head_(nullptr), tail_(nullptr), size_(0) {}

    LinkedList(const LinkedList& other) : head_(nullptr), tail_(nullptr), size_(0) {
        copyFrom(other);
    }

    LinkedList(LinkedList&& other) noexcept : head_(nullptr), tail_(nullptr), size_(0) {
        moveFrom(std::move(other));
    }

    LinkedList& operator=(const LinkedList& other) {
        if (this != &other) {
            clear();
            copyFrom(other);
        }
        return *this;
    }

    LinkedList& operator=(LinkedList&& other) noexcept {
        if (this != &other) {
            clear();
            moveFrom(std::move(other));
        }
        return *this;
    }

    ~LinkedList() {
        clear();
    }

    void push_back(const T& value) {
        Node* node = new Node(value);
        if (!head_) {
            head_ = tail_ = node;
        } else {
            tail_->next = node;
            tail_ = node;
        }
        ++size_;
    }

    void push_back(T&& value) {
        Node* node = new Node(std::move(value));
        if (!head_) {
            head_ = tail_ = node;
        } else {
            tail_->next = node;
            tail_ = node;
        }
        ++size_;
    }

    void push_front(const T& value) {
        Node* node = new Node(value);
        node->next = head_;
        head_ = node;
        if (!tail_) {
            tail_ = head_;
        }
        ++size_;
    }

    void push_front(T&& value) {
        Node* node = new Node(std::move(value));
        node->next = head_;
        head_ = node;
        if (!tail_) {
            tail_ = head_;
        }
        ++size_;
    }

    void pop_front() {
        if (!head_) {
            throw std::out_of_range("LinkedList::pop_front on empty list");
        }
        Node* old = head_;
        head_ = head_->next;
        if (!head_) {
            tail_ = nullptr;
        }
        delete old;
        --size_;
    }

    T& front() {
        if (!head_) {
            throw std::out_of_range("LinkedList::front on empty list");
        }
        return head_->data;
    }

    const T& front() const {
        if (!head_) {
            throw std::out_of_range("LinkedList::front on empty list");
        }
        return head_->data;
    }

    bool empty() const { return size_ == 0; }
    std::size_t size() const { return size_; }

    void clear() {
        Node* current = head_;
        while (current) {
            Node* next = current->next;
            delete current;
            current = next;
        }
        head_ = nullptr;
        tail_ = nullptr;
        size_ = 0;
    }

    Iterator begin() { return Iterator(head_); }
    Iterator end() { return Iterator(nullptr); }
    ConstIterator begin() const { return ConstIterator(head_); }
    ConstIterator end() const { return ConstIterator(nullptr); }
    ConstIterator cbegin() const { return ConstIterator(head_); }
    ConstIterator cend() const { return ConstIterator(nullptr); }
};

} // namespace edacal

#endif
