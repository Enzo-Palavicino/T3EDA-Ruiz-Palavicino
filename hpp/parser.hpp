#ifndef EDACAL_PARSER_HPP
#define EDACAL_PARSER_HPP

#include "linked_list.hpp"
#include "stack.hpp"
#include "token.hpp"
#include "tree.hpp"
#include "errors.hpp"

namespace edacal {

class Parser {
public:
    Parser() = default;

    LinkedList<Token> toPostfix(const LinkedList<Token>& tokens) const;
    Tree buildTreeFromPostfix(const LinkedList<Token>& postfix) const;

private:
    static int precedence(TokenType type);
    static bool isRightAssociative(TokenType type);
    static bool isFunction(TokenType type);
};

} // namespace edacal

#endif
