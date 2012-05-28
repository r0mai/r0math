

#ifndef BUILTIN_COMPARSION_HPP_
#define BUILTIN_COMPARSION_HPP_

#include "expression_tree.hpp"

namespace r0 { namespace builtin {

expression_tree Less(const expression_tree::operands_t& ops, enviroment& env);
expression_tree Greater(const expression_tree::operands_t& ops, enviroment& env);
expression_tree LessEqual(const expression_tree::operands_t& ops, enviroment& env);
expression_tree GreaterEqual(const expression_tree::operands_t& ops, enviroment& env);

}} //namespace r0::builtin

#endif //BUILTIN_COMPARSION_HPP_