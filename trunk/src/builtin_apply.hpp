
#ifndef BUILTIN_APPLY_HPP_
#define BUILTIN_APPLY_HPP_

#include "expression_tree.hpp"

namespace r0 { namespace builtin {

expression_tree Apply(const expression_tree::operands_t& ops, enviroment& env);

}} //namespace r0::builtin

#endif //BUILTIN_APPLY_HPP_