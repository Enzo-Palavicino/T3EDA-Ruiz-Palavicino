#include "evaluator.hpp"

#include <cmath>

namespace edacal {

double Evaluator::evalPostfix(const LinkedList<Token>& postfix, SymbolTable& symbols) const {
    Stack<double> values;

    auto popValue = [&]() -> double {
        if (values.empty()) {
            throw EdaError("faltan operandos");
        }
        double v = values.top();
        values.pop();
        return v;
    };

    for (auto it = postfix.begin(); it != postfix.end(); ++it) {
        const Token& token = *it;
        if (token.type == TokenType::END) {
            break;
        }

        switch (token.type) {
            case TokenType::NUMBER:
                values.push(token.value);
                break;
            case TokenType::ANS:
                values.push(symbols.get("ans"));
                break;
            case TokenType::IDENT:
                values.push(symbols.get(token.lexeme));
                break;
            case TokenType::UNARY_MINUS: {
                double operand = popValue();
                values.push(-operand);
                break;
            }
            case TokenType::SQRT: {
                double operand = popValue();
                if (operand < 0.0) {
                    throw EdaError("sqrt con argumento negativo");
                }
                values.push(std::sqrt(operand));
                break;
            }
            case TokenType::PLUS: {
                double right = popValue();
                double left = popValue();
                values.push(left + right);
                break;
            }
            case TokenType::MINUS: {
                double right = popValue();
                double left = popValue();
                values.push(left - right);
                break;
            }
            case TokenType::MUL: {
                double right = popValue();
                double left = popValue();
                values.push(left * right);
                break;
            }
            case TokenType::DIV: {
                double right = popValue();
                if (right == 0.0) {
                    throw EdaError("division por cero");
                }
                double left = popValue();
                values.push(left / right);
                break;
            }
            case TokenType::POW: {
                double right = popValue();
                double left = popValue();
                values.push(std::pow(left, right));
                break;
            }
            default:
                throw EdaError("token inesperado en evaluacion: " + token.lexeme);
        }
    }

    if (values.size() != 1) {
        throw EdaError("expresion invalida");
    }

    double result = values.top();
    values.pop();
    return result;
}

} // namespace edacal
