
#ifndef BUILTIN_TIMES_HPP_
#define BUILTIN_TIMES_HPP_

#include "expression_tree.hpp"

namespace r0 { namespace builtin {

expression_tree Times(const expression_tree::operands_t& ops, enviroment& env);
expression_tree numeric_Times(const expression_tree::operands_t& ops, enviroment& env);


}} //namespace r0::builtin

#endif //BUILTIN_TIMES_HPP_