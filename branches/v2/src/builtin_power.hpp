
#ifndef BUILTIN_POWER_HPP_
#define BUILTIN_POWER_HPP_

#include "expression_tree.hpp"

namespace r0 { namespace builtin {

expression_tree Power(const expression_tree::operands_t& ops, enviroment& env);
expression_tree numeric_Power(const expression_tree::operands_t& ops, enviroment& env);

}} //namespace r0::builtin

#endif //BUILTIN_POWER_HPP_