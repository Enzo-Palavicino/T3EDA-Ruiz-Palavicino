#ifndef EDACAL_EVALUATOR_HPP
#define EDACAL_EVALUATOR_HPP

#include "errors.hpp"
#include "linked_list.hpp"
#include "stack.hpp"
#include "symbols.hpp"
#include "token.hpp"

namespace edacal {

class Evaluator {
public:
    Evaluator() = default;

    double evalPostfix(const LinkedList<Token>& postfix, SymbolTable& symbols) const;
};

} // namespace edacal

#endif
