#ifndef EDACAL_TOKEN_HPP
#define EDACAL_TOKEN_HPP

#include <string>
#include <utility>

namespace edacal {

enum class TokenType {
    NUMBER,
    IDENT,
    PLUS,
    MINUS,
    MUL,
    DIV,
    POW,
    LPAREN,
    RPAREN,
    SQRT,
    ASSIGN,
    ANS,
    END,
    UNARY_MINUS
};

struct Token {
    TokenType type;
    std::string lexeme;
    double value;

    Token() : type(TokenType::END), lexeme(), value(0.0) {}
    Token(TokenType t, std::string lex, double val = 0.0)
        : type(t), lexeme(std::move(lex)), value(val) {}
};

inline bool isOperator(const Token& token) {
    switch (token.type) {
        case TokenType::PLUS:
        case TokenType::MINUS:
        case TokenType::MUL:
        case TokenType::DIV:
        case TokenType::POW:
        case TokenType::UNARY_MINUS:
            return true;
        default:
            return false;
    }
}

inline bool isValueToken(const Token& token) {
    switch (token.type) {
        case TokenType::NUMBER:
        case TokenType::IDENT:
        case TokenType::ANS:
            return true;
        default:
            return false;
    }
}

} // namespace edacal

#endif
