#include "stack.hpp"
#include "token.hpp"
#include "tree.hpp"

namespace edacal {

template class Stack<Token>;
template class Stack<Tree::Node*>;
template class Stack<double>;

} // namespace edacal
