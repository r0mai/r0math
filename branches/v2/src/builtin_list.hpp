
#ifndef BUILTIN_LIST_HPP_
#define BUILTIN_LIST_HPP_

#include "expression_tree.hpp"

namespace r0 { namespace builtin {

expression_tree List(const expression_tree::operands_t& ops, enviroment& env);

}} //namespace r0::builtin

#endif //BUILTIN_LIST_HPP_