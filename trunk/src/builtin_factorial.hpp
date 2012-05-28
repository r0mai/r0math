
#ifndef BUILTIN_FACTORIAL_HPP_
#define BUILTIN_FACTORIAL_HPP_

#include "expression_tree.hpp"

namespace r0 { namespace builtin {

expression_tree Factorial(const expression_tree::operands_t& ops, enviroment& env);

}} //namespace r0::builtin

#endif //BUILTIN_FACTORIAL_HPP_