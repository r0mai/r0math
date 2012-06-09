
#ifndef BUILTIN_TOGETHER_HPP_
#define BUILTIN_TOGETHER_HPP_

#include "expression_tree.hpp"

namespace r0 { namespace builtin {

expression_tree Together(const expression_tree::operands_t& ops, enviroment& env);

namespace together_impl {

//returns true if expr has the form Power[a, -x] and sets result to Power[a,x]
bool to_negative_exponent(const expression_tree& expr, expression_tree& result);

} //namespace together_impl

}} //namespace r0::builtin

#endif //BUILTIN_TOGETHER_HPP_