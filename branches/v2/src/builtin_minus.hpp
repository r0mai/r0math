
#ifndef BUILTIN_MINUS_HPP_
#define BUILTIN_MINUS_HPP_

#include "expression_tree.hpp"

namespace r0 { namespace builtin {

expression_tree Minus(const expression_tree::operands_t& ops, enviroment& env);

}} //namespace r0::builtin

#endif //BUILTIN_MINUS_HPP_