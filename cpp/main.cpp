#include "evaluator.hpp"
#include "parser.hpp"
#include "printer.hpp"
#include "symbols.hpp"
#include "tokenizer.hpp"

#include <cctype>
#include <iostream>
#include <sstream>
#include <string>

namespace {

std::string trim(const std::string& text) {
    std::size_t start = 0;
    while (start < text.size() && std::isspace(static_cast<unsigned char>(text[start]))) {
        ++start;
    }
    std::size_t end = text.size();
    while (end > start && std::isspace(static_cast<unsigned char>(text[end - 1]))) {
        --end;
    }
    return text.substr(start, end - start);
}

} // namespace

int main() {
    using namespace edacal;

    std::cout << "Bienvenido a EdaCal" << std::endl;

    Tokenizer tokenizer;
    Parser parser;
    Evaluator evaluator;
    Printer printer;
    SymbolTable symbols;

    LinkedList<Token> lastPostfix;
    bool hasLastPostfix = false;
    Tree lastTree;
    bool hasLastTree = false;

    std::string line;

    while (true) {
        std::cout << ">> " << std::flush;
        if (!std::getline(std::cin, line)) {
            break;
        }

        std::string trimmed = trim(line);
        if (trimmed.empty()) {
            continue;
        }

        std::istringstream iss(trimmed);
        std::string command;
        iss >> command;

        if (command == "exit") {
            break;
        } else if (command == "show") {
            std::string var;
            if (!(iss >> var)) {
                std::cout << ">> error: falta nombre de variable" << std::endl;
                continue;
            }
            try {
                double value = symbols.get(var);
                std::cout << ">> " << var << " -> " << formatNumber(value) << std::endl;
            } catch (const EdaError& err) {
                std::cout << ">> error: " << err.what() << std::endl;
            }
            continue;
        } else if (command == "tree") {
            if (!hasLastTree) {
                std::cout << ">> error: no hay expresion evaluada" << std::endl;
            } else {
                printer.printTree(lastTree, std::cout);
            }
            continue;
        } else if (command == "posfix" || command == "postfix") {
            if (!hasLastPostfix) {
                std::cout << ">> error: no hay expresion evaluada" << std::endl;
            } else {
                printer.printPostfix(lastPostfix, std::cout);
            }
            continue;
        } else if (command == "prefix") {
            if (!hasLastTree) {
                std::cout << ">> error: no hay expresion evaluada" << std::endl;
            } else {
                printer.printPrefix(lastTree, std::cout);
            }
            continue;
        }

        try {
            LinkedList<Token> tokens = tokenizer.tokenize(trimmed);
            bool isAssignment = false;
            std::string targetVariable;

            auto it = tokens.begin();
            if (it != tokens.end() && it->type == TokenType::IDENT) {
                auto nextIt = it;
                ++nextIt;
                if (nextIt != tokens.end() && nextIt->type == TokenType::ASSIGN) {
                    isAssignment = true;
                    targetVariable = it->lexeme;
                    tokens.pop_front(); // remove identifier
                    tokens.pop_front(); // remove assignment
                }
            }

            bool hasContent = false;
            for (auto checkIt = tokens.begin(); checkIt != tokens.end(); ++checkIt) {
                if (checkIt->type != TokenType::END) {
                    hasContent = true;
                    break;
                }
            }
            if (!hasContent) {
                throw EdaError("expresion vacia");
            }

            LinkedList<Token> expressionTokens = tokens;
            bool hasEnd = false;
            for (auto expIt = expressionTokens.begin(); expIt != expressionTokens.end(); ++expIt) {
                if (expIt->type == TokenType::END) {
                    hasEnd = true;
                    break;
                }
            }
            if (!hasEnd) {
                expressionTokens.push_back(Token(TokenType::END, ""));
            }

            LinkedList<Token> postfix = parser.toPostfix(expressionTokens);
            double result = evaluator.evalPostfix(postfix, symbols);
            Tree tree = parser.buildTreeFromPostfix(postfix);

            symbols.set("ans", result);
            if (isAssignment) {
                symbols.set(targetVariable, result);
                std::cout << ">> " << targetVariable << " -> " << formatNumber(result) << std::endl;
            } else {
                std::cout << ">> ans -> " << formatNumber(result) << std::endl;
            }

            lastPostfix = postfix;
            hasLastPostfix = true;
            lastTree = std::move(tree);
            hasLastTree = true;
        } catch (const EdaError& err) {
            std::cout << ">> error: " << err.what() << std::endl;
        }
    }

    return 0;
}
