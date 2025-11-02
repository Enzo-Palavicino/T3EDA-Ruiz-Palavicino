#include "parser.hpp"

namespace edacal {

namespace {

Token makeUnaryMinusToken() {
    return Token(TokenType::UNARY_MINUS, "neg");
}

bool isValue(const Token& token) {
    return token.type == TokenType::NUMBER ||
           token.type == TokenType::IDENT ||
           token.type == TokenType::ANS;
}

} // namespace

LinkedList<Token> Parser::toPostfix(const LinkedList<Token>& tokens) const {
    LinkedList<Token> output;
    Stack<Token> opStack;
    bool expectOperand = true;

    for (auto it = tokens.begin(); it != tokens.end(); ++it) {
        const Token& token = *it;
        if (token.type == TokenType::END) {
            break;
        }

        if (isValue(token)) {
            output.push_back(token);
            expectOperand = false;
            continue;
        }

        switch (token.type) {
            case TokenType::SQRT:
                opStack.push(token);
                expectOperand = true;
                break;
            case TokenType::MINUS:
                if (expectOperand) {
                    opStack.push(makeUnaryMinusToken());
                } else {
                    auto handleOperator = [&]() {
                        while (!opStack.empty()) {
                            Token top = opStack.top();
                            if (top.type == TokenType::LPAREN) {
                                break;
                            }
                            if (isFunction(top.type)) {
                                output.push_back(top);
                                opStack.pop();
                                continue;
                            }
                            int topPrec = precedence(top.type);
                            int curPrec = precedence(token.type);
                            if (topPrec > curPrec || (topPrec == curPrec && !isRightAssociative(token.type))) {
                                output.push_back(top);
                                opStack.pop();
                            } else {
                                break;
                            }
                        }
                    };
                    handleOperator();
                    opStack.push(token);
                    expectOperand = true;
                }
                break;
            case TokenType::PLUS:
            case TokenType::MUL:
            case TokenType::DIV:
            case TokenType::POW: {
                if (expectOperand) {
                    throw EdaError("operando esperado antes del operador '" + token.lexeme + "'");
                }
                while (!opStack.empty()) {
                    Token top = opStack.top();
                    if (top.type == TokenType::LPAREN) {
                        break;
                    }
                    if (isFunction(top.type)) {
                        output.push_back(top);
                        opStack.pop();
                        continue;
                    }
                    int topPrec = precedence(top.type);
                    int curPrec = precedence(token.type);
                    bool rightAssoc = isRightAssociative(token.type);
                    if (topPrec > curPrec || (topPrec == curPrec && !rightAssoc)) {
                        output.push_back(top);
                        opStack.pop();
                    } else {
                        break;
                    }
                }
                opStack.push(token);
                expectOperand = true;
                break;
            }
            case TokenType::LPAREN:
                opStack.push(token);
                expectOperand = true;
                break;
            case TokenType::RPAREN: {
                bool found = false;
                while (!opStack.empty()) {
                    Token top = opStack.top();
                    if (top.type == TokenType::LPAREN) {
                        opStack.pop();
                        found = true;
                        break;
                    }
                    output.push_back(top);
                    opStack.pop();
                }
                if (!found) {
                    throw EdaError("parentesis desbalanceados");
                }
                if (!opStack.empty() && isFunction(opStack.top().type)) {
                    output.push_back(opStack.top());
                    opStack.pop();
                }
                expectOperand = false;
                break;
            }
            case TokenType::ASSIGN:
                throw EdaError("asignacion inesperada dentro de la expresion");
            default:
                throw EdaError("token inesperado: " + token.lexeme);
        }
    }

    if (expectOperand && !output.empty()) {
        throw EdaError("expresion incompleta");
    }

    while (!opStack.empty()) {
        Token top = opStack.top();
        opStack.pop();
        if (top.type == TokenType::LPAREN || top.type == TokenType::RPAREN) {
            throw EdaError("parentesis desbalanceados");
        }
        output.push_back(top);
    }

    output.push_back(Token(TokenType::END, ""));
    return output;
}

Tree Parser::buildTreeFromPostfix(const LinkedList<Token>& postfix) const {
    Stack<Tree::Node*> nodeStack;

    auto cleanup = [&]() {
        while (!nodeStack.empty()) {
            Tree::Node* node = nodeStack.top();
            nodeStack.pop();
            delete node;
        }
    };

    for (auto it = postfix.begin(); it != postfix.end(); ++it) {
        const Token& token = *it;
        if (token.type == TokenType::END) {
            break;
        }

        if (isValue(token)) {
            nodeStack.push(new Tree::Node(token));
            continue;
        }

        if (token.type == TokenType::UNARY_MINUS || token.type == TokenType::SQRT) {
            if (nodeStack.empty()) {
                cleanup();
                throw EdaError("falta operando para operador '" + token.lexeme + "'");
            }
            Tree::Node* operand = nodeStack.top();
            nodeStack.pop();
            Tree::Node* node = new Tree::Node(token);
            node->left = operand;
            nodeStack.push(node);
            continue;
        }

        if (token.type == TokenType::PLUS ||
            token.type == TokenType::MINUS ||
            token.type == TokenType::MUL ||
            token.type == TokenType::DIV ||
            token.type == TokenType::POW) {
            if (nodeStack.size() < 2) {
                cleanup();
                throw EdaError("falta operando para operador '" + token.lexeme + "'");
            }
            Tree::Node* right = nodeStack.top();
            nodeStack.pop();
            Tree::Node* left = nodeStack.top();
            nodeStack.pop();
            Tree::Node* node = new Tree::Node(token);
            node->left = left;
            node->right = right;
            nodeStack.push(node);
            continue;
        }

        cleanup();
        throw EdaError("token no manejado en arbol: " + token.lexeme);
    }

    if (nodeStack.size() != 1) {
        cleanup();
        throw EdaError("expresion invalida");
    }

    Tree::Node* root = nodeStack.top();
    nodeStack.pop();
    Tree tree;
    tree.setRoot(root);
    return tree;
}

int Parser::precedence(TokenType type) {
    switch (type) {
        case TokenType::UNARY_MINUS:
        case TokenType::SQRT:
            return 4;
        case TokenType::POW:
            return 3;
        case TokenType::MUL:
        case TokenType::DIV:
            return 2;
        case TokenType::PLUS:
        case TokenType::MINUS:
            return 1;
        default:
            return 0;
    }
}

bool Parser::isRightAssociative(TokenType type) {
    return type == TokenType::POW || type == TokenType::UNARY_MINUS || type == TokenType::SQRT;
}

bool Parser::isFunction(TokenType type) {
    return type == TokenType::SQRT;
}

} // namespace edacal
