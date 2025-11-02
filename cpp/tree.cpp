#include "tree.hpp"

namespace edacal {

Tree::Tree() : root_(nullptr) {}

Tree::~Tree() {
    clear();
}

Tree::Tree(Tree&& other) noexcept : root_(nullptr) {
    root_ = other.root_;
    other.root_ = nullptr;
}

Tree& Tree::operator=(Tree&& other) noexcept {
    if (this != &other) {
        clear();
        root_ = other.root_;
        other.root_ = nullptr;
    }
    return *this;
}

Tree::Node* Tree::getRoot() const {
    return root_;
}

void Tree::setRoot(Node* node) {
    clear();
    root_ = node;
}

bool Tree::empty() const {
    return root_ == nullptr;
}

void Tree::clear() {
    deleteSubtree(root_);
    root_ = nullptr;
}

void Tree::deleteSubtree(Node* node) {
    if (!node) {
        return;
    }
    deleteSubtree(node->left);
    deleteSubtree(node->right);
    delete node;
}

} // namespace edacal
