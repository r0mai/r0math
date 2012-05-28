
#ifndef BUILTIN_RANGE_HPP_
#define BUILTIN_RANGE_HPP_

#include "expression_tree.hpp"

namespace r0 { namespace builtin {

expression_tree Range(const expression_tree::operands_t& ops, enviroment& env);

}} //namespace r0::builtin

#endif //BUILTIN_RANGE_HPP_