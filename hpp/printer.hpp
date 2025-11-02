#ifndef EDACAL_PRINTER_HPP
#define EDACAL_PRINTER_HPP

#include "linked_list.hpp"
#include "token.hpp"
#include "tree.hpp"

#include <iosfwd>
#include <string>

namespace edacal {

std::string formatNumber(double value);
std::string tokenToString(const Token& token);

class Printer {
public:
    Printer() = default;

    void printTree(const Tree& tree, std::ostream& os) const;
    void printPostfix(const LinkedList<Token>& tokens, std::ostream& os) const;
    void printPrefix(const Tree& tree, std::ostream& os) const;

private:
    void printTreeNode(const Tree::Node* node, const std::string& prefix, bool isLeft, std::ostream& os) const;
    void collectPrefix(const Tree::Node* node, LinkedList<std::string>& output) const;
};

} // namespace edacal

#endif
