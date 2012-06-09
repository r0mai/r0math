
#include "builtin_rule.hpp"
#include "enviroment.hpp"

namespace r0 { namespace builtin {

expression_tree Rule(const expression_tree::operands_t& ops, enviroment& env) {
    if ( ops.size() != 2 ) {
        env.raise_error("Rule", "called with invalid arguments");
        return expression_tree::make_operator("Rule", ops);
    }
    return expression_tree::make_operator("Rule", ops);
}

}} //namespace r0::builtin
