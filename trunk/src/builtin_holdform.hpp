
#ifndef BUILTIN_HOLDFORM_HPP_
#define BUILTIN_HOLDFORM_HPP_

#include "expression_tree.hpp"

namespace r0 { namespace builtin {

expression_tree HoldForm(const expression_tree::operands_t& ops, enviroment& env);

}} //namespace r0::builtin

#endif //BUILTIN_HOLDFORM_HPP_