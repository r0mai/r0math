
#include "builtin_accumulate.hpp"

#include "builtin_plus.hpp"
#include "enviroment.hpp"

namespace r0 { namespace builtin {

expression_tree Accumulate(const expression_tree::operands_t& ops, enviroment& env) {
    if ( ops.size() != 1 ) {
        env.raise_error("Accumulate", "called with invalid arguments");
        return expression_tree::make_operator("Accumulate", ops);
    }

    if ( ops[0].get_type() != expression_tree::OPERATOR ) {
        env.raise_error("Accumulate", "operator expected at position 1");
        return expression_tree::make_operator("Accumulate", ops);
    }

    expression_tree::operands_t summands(ops[0].get_operands());

    for ( unsigned i = 1; i < summands.size(); ++i ) {
        summands[i] = expression_tree::make_operator( "Plus", summands[i-1], summands[i] ).evaluate(env);
    }

    return expression_tree::make_operator(ops[0].get_operator_name(), summands);
}


}} //namespace r0::builtin