
#ifndef BUILTIN_NESTLIST_HPP_
#define BUILTIN_NESTLIST_HPP_

#include "expression_tree.hpp"

namespace r0 { namespace builtin {

expression_tree NestList(const expression_tree::operands_t& ops, enviroment& env);

}} //namespace r0::builtin

#endif //BUILTIN_NESTLIST_HPP_