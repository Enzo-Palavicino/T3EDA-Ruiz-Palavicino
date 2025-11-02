#ifndef EDACAL_TREE_HPP
#define EDACAL_TREE_HPP

#include "token.hpp"

namespace edacal {

class Tree {
public:
    struct Node {
        Token token;
        Node* left;
        Node* right;

        explicit Node(const Token& tok) : token(tok), left(nullptr), right(nullptr) {}
    };

    Tree();
    ~Tree();

    Tree(Tree&& other) noexcept;
    Tree& operator=(Tree&& other) noexcept;

    Tree(const Tree&) = delete;
    Tree& operator=(const Tree&) = delete;

    Node* getRoot() const;
    void setRoot(Node* node);
    bool empty() const;
    void clear();

private:
    Node* root_;

    void deleteSubtree(Node* node);
};

} // namespace edacal

#endif
