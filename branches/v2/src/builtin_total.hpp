
#ifndef BUILTIN_TOTAL_HPP_
#define BUILTIN_TOTAL_HPP_

#include "expression_tree.hpp"

namespace r0 { namespace builtin {

expression_tree Total(const expression_tree::operands_t& ops, enviroment& env);

namespace total_impl {

expression_tree Total(const expression_tree::operands_t& ops, unsigned min_level, unsigned max_level, enviroment& env);

}}} //namespace r0::builtin::total_impl

#endif //BUILTIN_TOTAL_HPP_