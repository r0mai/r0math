
#ifndef BUILTIN_RATIONAL_HPP_
#define BUILTIN_RATIONAL_HPP_

#include "expression_tree.hpp"

namespace r0 { namespace builtin {

expression_tree Rational(const expression_tree::operands_t& ops, enviroment& env);

}} //namespace r0::builtin

#endif //BUILTIN_RATIONAL_HPP_