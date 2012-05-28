
#ifndef BUILTIN_SET_HPP_
#define BUILTIN_SET_HPP_

#include "expression_tree.hpp"

namespace r0 { namespace builtin {

expression_tree Set(const expression_tree::operands_t& ops, enviroment& env);

}} //namespace r0::builtin

#endif //BUILTIN_SET_HPP_