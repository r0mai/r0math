
#ifndef BUILTIN_BOOLEAN_HPP_
#define BUILTIN_BOOLEAN_HPP_

#include "expression_tree.hpp"

namespace r0 { namespace builtin {

expression_tree Not(const expression_tree::operands_t& ops, enviroment& env);

expression_tree And(const expression_tree::operands_t& ops, enviroment& env);
expression_tree Or(const expression_tree::operands_t& ops, enviroment& env);

}} //namespace r0::builtin

#endif //BUILTIN_BOOLEAN_HPP_