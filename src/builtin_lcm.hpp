
#ifndef BUILTIN_LCM_HPP_
#define BUILTIN_LCM_HPP_

#include "expression_tree.hpp"

namespace r0 { namespace builtin {

expression_tree LCM(const expression_tree::operands_t& ops, enviroment& env);

}} //namespace r0::builtin

#endif //BUILTIN_LCM_HPP_