
#ifndef BUILTIN_BYTECOUNT_HPP_
#define BUILTIN_BYTECOUNT_HPP_

#include "expression_tree.hpp"

namespace r0 { namespace builtin {

expression_tree ByteCount(const expression_tree::operands_t& ops, enviroment& env);

}} //namespace r0::builtin

#endif //BUILTIN_BYTECOUNT_HPP_