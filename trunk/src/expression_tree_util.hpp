
#ifndef EXPRESSION_TREE_UTIL_HPP_
#define EXPRESSION_TREE_UTIL_HPP_

#include "expression_tree.hpp"
#include "mpirxx_include.hpp"

namespace r0 {

//assumes there could be more then one exact_number in ops ( is it necesseary? )

//extract the EXACT_NUMBERS together
//will fail with assert() if more than one EXACT_NUMBERs found
mpq_class extract_exact_numeric_values( const expression_tree::operands_t& ops, const mpq_class& default_value );
//returns ops with exact_numeric_values_removed
expression_tree::operands_t remove_exact_numeric_values( const expression_tree::operands_t& ops );

} //namespace r0

#endif //EXPRESSION_TREE_UTIL_HPP_