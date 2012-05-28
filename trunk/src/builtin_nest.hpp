
#ifndef BUILTIN_NEST_HPP_
#define BUILTIN_NEST_HPP_

#include "expression_tree.hpp"

namespace r0 { namespace builtin {

expression_tree Nest(const expression_tree::operands_t& ops, enviroment& env);

}} //namespace r0::builtin

#endif //BUILTIN_NEST_HPP_