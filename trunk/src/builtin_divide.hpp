
#ifndef BUILTIN_DIVIDE_HPP_
#define BUILTIN_DIVIDE_HPP_

#include "expression_tree.hpp"

namespace r0 { namespace builtin {

expression_tree Divide(const expression_tree::operands_t& ops, enviroment& env);

}} //namespace r0::builtin

#endif //BUILTIN_DIVIDE_HPP_