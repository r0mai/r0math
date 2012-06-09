
#ifndef BUILTIN_PLUS_HPP_
#define BUILTIN_PLUS_HPP_

#include "expression_tree.hpp"

namespace r0 { namespace builtin {

expression_tree Plus(const expression_tree::operands_t& ops, enviroment& env);
expression_tree numeric_Plus(const expression_tree::operands_t& ops, enviroment& env);

namespace plus_impl {

expression_tree Plus(expression_tree::operands_t ops, enviroment& env);
expression_tree numeric_Plus(expression_tree::operands_t ops, enviroment& env);

expression_tree non_numeric_only_numericq_plus(const expression_tree::operands_t& ops, enviroment& env);

} //namespace plus_impl

}} //namespace r0::builtin

#endif //BUILTIN_PLUS_HPP_
