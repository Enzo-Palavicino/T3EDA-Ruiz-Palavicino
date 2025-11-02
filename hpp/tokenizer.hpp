#ifndef EDACAL_TOKENIZER_HPP
#define EDACAL_TOKENIZER_HPP

#include "errors.hpp"
#include "linked_list.hpp"
#include "token.hpp"

#include <string>

namespace edacal {

class Tokenizer {
public:
    Tokenizer() = default;
    LinkedList<Token> tokenize(const std::string& input) const;
};

} // namespace edacal

#endif
