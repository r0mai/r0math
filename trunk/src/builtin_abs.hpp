
#ifndef BUILTIN_ABS_HPP_
#define BUILTIN_ABS_HPP_

#include "expression_tree.hpp"

namespace r0 { namespace builtin {

expression_tree Abs(const expression_tree::operands_t& ops, enviroment& env);
expression_tree numeric_Abs(const expression_tree::operands_t& ops, enviroment& env);

}} //namespace r0::builtin

#endif //BUILTIN_ABS_HPP_