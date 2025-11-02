#include "printer.hpp"

#include <cmath>
#include <iomanip>
#include <sstream>

namespace edacal {

std::string formatNumber(double value) {
    if (std::fabs(value) < 1e-12) {
        return "0";
    }
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(12) << value;
    std::string text = oss.str();
    std::size_t pos = text.find('.');
    if (pos != std::string::npos) {
        while (!text.empty() && text.back() == '0') {
            text.pop_back();
        }
        if (!text.empty() && text.back() == '.') {
            text.pop_back();
        }
    }
    if (text.empty()) {
        text = "0";
    }
    return text;
}

std::string tokenToString(const Token& token) {
    switch (token.type) {
        case TokenType::NUMBER:
            return formatNumber(token.value);
        case TokenType::IDENT:
        case TokenType::ANS:
        case TokenType::PLUS:
        case TokenType::MINUS:
        case TokenType::MUL:
        case TokenType::DIV:
        case TokenType::POW:
        case TokenType::LPAREN:
        case TokenType::RPAREN:
        case TokenType::ASSIGN:
            return token.lexeme;
        case TokenType::SQRT:
            return "sqrt";
        case TokenType::UNARY_MINUS:
            return "neg";
        case TokenType::END:
            return "";
    }
    return token.lexeme;
}

void Printer::printTree(const Tree& tree, std::ostream& os) const {
    if (tree.empty()) {
        os << "(arbol vacio)" << std::endl;
        return;
    }
    printTreeNode(tree.getRoot(), "", false, os);
}

void Printer::printPostfix(const LinkedList<Token>& tokens, std::ostream& os) const {
    bool first = true;
    for (auto it = tokens.begin(); it != tokens.end(); ++it) {
        const Token& token = *it;
        if (token.type == TokenType::END) {
            break;
        }
        if (!first) {
            os << ' ';
        }
        os << tokenToString(token);
        first = false;
    }
    os << std::endl;
}

void Printer::printPrefix(const Tree& tree, std::ostream& os) const {
    if (tree.empty()) {
        os << "(arbol vacio)" << std::endl;
        return;
    }
    LinkedList<std::string> items;
    collectPrefix(tree.getRoot(), items);
    bool first = true;
    for (auto it = items.begin(); it != items.end(); ++it) {
        if (!first) {
            os << ' ';
        }
        os << *it;
        first = false;
    }
    os << std::endl;
}

void Printer::printTreeNode(const Tree::Node* node, const std::string& prefix, bool isLeft, std::ostream& os) const {
    if (!node) {
        return;
    }
    if (node->right) {
        std::string nextPrefix = prefix + (isLeft ? "|   " : "    ");
        printTreeNode(node->right, nextPrefix, false, os);
    }
    os << prefix;
    if (!prefix.empty()) {
        os << (isLeft ? "|-- " : "\\-- ");
    }
    os << tokenToString(node->token) << std::endl;
    if (node->left) {
        std::string nextPrefix = prefix + (isLeft ? "|   " : "    ");
        printTreeNode(node->left, nextPrefix, true, os);
    }
}

void Printer::collectPrefix(const Tree::Node* node, LinkedList<std::string>& output) const {
    if (!node) {
        return;
    }
    output.push_back(tokenToString(node->token));
    collectPrefix(node->left, output);
    collectPrefix(node->right, output);
}

} // namespace edacal
