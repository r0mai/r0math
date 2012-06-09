
#ifndef BUILTIN_EXPAND_HPP_
#define BUILTIN_EXPAND_HPP_

#include "expression_tree.hpp"

namespace r0 { namespace builtin {

expression_tree Expand(const expression_tree::operands_t& ops, enviroment& env);

namespace expand_impl {

typedef std::vector<expression_tree::operands_t> term_matrix_t;

expression_tree do_expand( const expression_tree& expr, enviroment& env);

void add_times_to_terms( const expression_tree::operands_t& ops, term_matrix_t& term_matrix );
void add_power_to_terms( const expression_tree::operands_t& ops, term_matrix_t& term_matrix );

} //namespace expand_impl

}} //namespace r0::builtin

#endif //BUILTIN_EXPAND_HPP_