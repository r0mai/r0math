
#ifndef BUILTIN_ACCUMULATE_HPP_
#define BUILTIN_ACCUMULATE_HPP_

#include "expression_tree.hpp"

namespace r0 { namespace builtin {

expression_tree Accumulate(const expression_tree::operands_t& ops, enviroment& env);

}} //namespace r0::builtin

#endif //BUILTIN_ACCUMLATE_HPP_