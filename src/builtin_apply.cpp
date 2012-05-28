
#include "builtin_apply.hpp"
#include "enviroment.hpp"

namespace r0 { namespace builtin {

expression_tree Apply(const expression_tree::operands_t& ops, enviroment& env) {
    if (ops.size() != 2) {
        env.raise_error("Apply", "called with invalid arguments");
        return expression_tree::make_operator("Apply", ops);
    }
    if ( ops[0].get_type() != expression_tree::SYMBOL ||
        ops[1].get_type() != expression_tree::OPERATOR ) {
            //no error messages
        return expression_tree::make_operator("Apply", ops);
    }
    return expression_tree::make_operator( ops[0].get_symbol(), ops[1].get_operands() ).evaluate(env);
}

}} //namespace r0::builtin
