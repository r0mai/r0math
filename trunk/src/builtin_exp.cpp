
#include "builtin_exp.hpp"
#include "enviroment.hpp"

namespace r0 { namespace builtin {

expression_tree Exp(const expression_tree::operands_t& ops, enviroment& env) {
    if (ops.size() != 1) {
        env.raise_error("Exp", "called with invalid arguments");
        return expression_tree::make_operator("Exp", ops);
    }

    return expression_tree::make_operator("Power", expression_tree::make_symbol("E"), ops[0]).evaluate(env);
}

}} //namespace r0::builtin