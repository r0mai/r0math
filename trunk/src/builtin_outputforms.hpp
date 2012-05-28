
#ifndef BUILTIN_OUTPUTFORMS_HPP_
#define BUILTIN_OUTPUTFORMS_HPP_

#include "expression_tree.hpp"

namespace r0 { namespace builtin {

expression_tree InputForm(const expression_tree::operands_t& ops, enviroment& env);
expression_tree FullForm(const expression_tree::operands_t& ops, enviroment& env);
expression_tree CodeForm(const expression_tree::operands_t& ops, enviroment& env);

}} //namespace r0::builtin

#endif //BUILTIN_OUTPUTFORMS_HPP_