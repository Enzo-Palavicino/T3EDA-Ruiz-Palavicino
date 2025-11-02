#include "tokenizer.hpp"

#include <cctype>
#include <stdexcept>

namespace edacal {

LinkedList<Token> Tokenizer::tokenize(const std::string& input) const {
    LinkedList<Token> tokens;
    std::size_t i = 0;

    while (i < input.size()) {
        char c = input[i];
        if (std::isspace(static_cast<unsigned char>(c))) {
            ++i;
            continue;
        }

        if (std::isdigit(static_cast<unsigned char>(c)) || c == '.') {
            std::size_t start = i;
            bool dotSeen = (c == '.');
            ++i;
            while (i < input.size()) {
                char nc = input[i];
                if (std::isdigit(static_cast<unsigned char>(nc))) {
                    ++i;
                } else if (nc == '.' && !dotSeen) {
                    dotSeen = true;
                    ++i;
                } else {
                    break;
                }
            }
            const std::string lexeme = input.substr(start, i - start);
            try {
                double value = std::stod(lexeme);
                tokens.push_back(Token(TokenType::NUMBER, lexeme, value));
            } catch (const std::exception&) {
                throw EdaError("numero invalido: " + lexeme);
            }
            continue;
        }

        if (std::isalpha(static_cast<unsigned char>(c)) || c == '_') {
            std::size_t start = i;
            ++i;
            while (i < input.size()) {
                char nc = input[i];
                if (std::isalnum(static_cast<unsigned char>(nc)) || nc == '_') {
                    ++i;
                } else {
                    break;
                }
            }
            const std::string lexeme = input.substr(start, i - start);
            if (lexeme == "sqrt") {
                tokens.push_back(Token(TokenType::SQRT, lexeme));
            } else if (lexeme == "ans") {
                tokens.push_back(Token(TokenType::ANS, lexeme));
            } else {
                tokens.push_back(Token(TokenType::IDENT, lexeme));
            }
            continue;
        }

        switch (c) {
            case '+':
                tokens.push_back(Token(TokenType::PLUS, "+"));
                ++i;
                break;
            case '-':
                tokens.push_back(Token(TokenType::MINUS, "-"));
                ++i;
                break;
            case '*':
                tokens.push_back(Token(TokenType::MUL, "*"));
                ++i;
                break;
            case '/':
                tokens.push_back(Token(TokenType::DIV, "/"));
                ++i;
                break;
            case '^':
                tokens.push_back(Token(TokenType::POW, "^"));
                ++i;
                break;
            case '(':
                tokens.push_back(Token(TokenType::LPAREN, "("));
                ++i;
                break;
            case ')':
                tokens.push_back(Token(TokenType::RPAREN, ")"));
                ++i;
                break;
            case '=':
                tokens.push_back(Token(TokenType::ASSIGN, "="));
                ++i;
                break;
            default:
                throw EdaError(std::string("token no reconocido: ") + c);
        }
    }

    tokens.push_back(Token(TokenType::END, ""));
    return tokens;
}

} // namespace edacal
