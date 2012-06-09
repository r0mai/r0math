
#ifndef BUILTIN_NUMERICQ_HPP_
#define BUILTIN_NUMERICQ_HPP_

#include "expression_tree.hpp"

namespace r0 { namespace builtin {

expression_tree NumericQ(const expression_tree::operands_t& ops, enviroment& env);

namespace numericq_impl {

bool recursive_numericq(const expression_tree& expr, enviroment& env);

} //namespace numericq_impl

}} //namespace r0::builtin

#endif //BUILTIN_NUMERICQ_HPP_