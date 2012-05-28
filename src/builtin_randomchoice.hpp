
#ifndef BUILTIN_RANDOMCHOICE_HPP_
#define BUILTIN_RANDOMCHOICE_HPP_

#include "expression_tree.hpp"

namespace r0 { namespace builtin {

expression_tree RandomChoice(const expression_tree::operands_t& ops, enviroment& env);

}} //namespace r0::builtin

#endif //BUILTIN_RANDOMCHOICE_HPP_