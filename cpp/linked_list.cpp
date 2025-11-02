#include "linked_list.hpp"
#include "token.hpp"
#include "tree.hpp"

#include <string>

namespace edacal {

template class LinkedList<Token>;
template class LinkedList<Tree::Node*>;
template class LinkedList<std::string>;
template class LinkedList<double>;

} // namespace edacal
