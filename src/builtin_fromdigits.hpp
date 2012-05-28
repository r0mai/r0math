
#ifndef BUILTIN_FROMDIGITS_HPP_
#define BUILTIN_FROMDIGITS_HPP_

#include "expression_tree.hpp"

namespace r0 { namespace builtin {

expression_tree FromDigits(const expression_tree::operands_t& ops, enviroment& env);

}} //namespace r0::builtin

#endif //BUILTIN_FROMDIGITS_HPP_