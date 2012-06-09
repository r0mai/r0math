
#ifndef BUILTIN_FACTORINTEGER_HPP_
#define BUILTIN_FACTORINTEGER_HPP_

#include "expression_tree.hpp"

#include <map>

namespace r0 { namespace builtin {

expression_tree FactorInteger(const expression_tree::operands_t& ops, enviroment& env);

namespace factorinteger_impl {

typedef std::map<mpz_class, int> factored_type;

factored_type factor_integer(mpz_class n);

} //namespace factorinteger_impl

}} //namespace r0::builtin

#endif //BUILTIN_FACTORINTEGER_HPP_