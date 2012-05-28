
#ifndef BUILTIN_SQRT_HPP_
#define BUILTIN_SQRT_HPP_

#include "expression_tree.hpp"

namespace r0 { namespace builtin {

expression_tree Sqrt(const expression_tree::operands_t& ops, enviroment& env);

}} //namespace r0::builtin

#endif //BUILTIN_SQRT_HPP_