
#ifndef BUILTIN_LOG_HPP_
#define BUILTIN_LOG_HPP_

#include "expression_tree.hpp"

namespace r0 { namespace builtin {

expression_tree Log(const expression_tree::operands_t& ops, enviroment& env);

expression_tree numeric_Log(const expression_tree::operands_t& ops, enviroment& env);

}} //namespace r0::builtin

#endif //BUILTIN_LOG_HPP_