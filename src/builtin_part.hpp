
#ifndef BUILTIN_PART_HPP_
#define BUILTIN_PART_HPP_

#include "expression_tree.hpp"

namespace r0 { namespace builtin {

expression_tree Part(const expression_tree::operands_t& ops, enviroment& env);

namespace part_impl {

bool extract_part( const expression_tree& expr, long& res, enviroment& env);

} //namespace part_impl

}} //namespace r0::builtin

#endif //BUILTIN_PART_HPP_