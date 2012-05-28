
#ifndef BUILTIN_FACTOR_HPP_
#define BUILTIN_FACTOR_HPP_

#include "expression_tree.hpp"

namespace r0 { namespace builtin {

expression_tree Factor(const expression_tree::operands_t& ops, enviroment& env);

}} //namespace r0::builtin

#endif //BUILTIN_FACTOR_HPP_
