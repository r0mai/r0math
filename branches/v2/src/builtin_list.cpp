
#include "builtin_list.hpp"
#include "expression_tree.hpp"

namespace r0 { namespace builtin {

expression_tree List(const expression_tree::operands_t& ops, enviroment& env) {
    return expression_tree::make_operator( "List", ops );
}

}} //namespace r0::builtin