
#include "builtin_outputforms.hpp"
#include "enviroment.hpp"

namespace r0 { namespace builtin {

expression_tree InputForm(const expression_tree::operands_t& ops, enviroment& env) {
    if ( ops.size() != 1 ) {
        env.raise_error("InputForm", "called with invalid arguments");
        return expression_tree::make_operator("InputForm", ops);
    }
    return expression_tree::make_operator("InputForm", ops);
}

expression_tree FullForm(const expression_tree::operands_t& ops, enviroment& env) {
    if ( ops.size() != 1 ) {
        env.raise_error("FullForm", "called with invalid arguments");
        return expression_tree::make_operator("FullForm", ops);
    }
    return expression_tree::make_operator("FullForm", ops);
}

expression_tree CodeForm(const expression_tree::operands_t& ops, enviroment& env) {
    if ( ops.size() != 1 ) {
        env.raise_error("CodeForm", "called with invalid arguments");
        return expression_tree::make_operator("CodeForm", ops);
    }
    return expression_tree::make_operator("CodeForm", ops);
}

}} //namespace r0::builtin