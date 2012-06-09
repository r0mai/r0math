
#include "builtin_factor.hpp"

namespace r0 { namespace builtin {

expression_tree Factor(const expression_tree::operands_t& ops, enviroment& env) {
	return expression_tree::make_operator("Factor", ops);
}

}} //namespace r0::builtin
