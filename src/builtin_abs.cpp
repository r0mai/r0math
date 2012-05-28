
#include "builtin_abs.hpp"
#include "enviroment.hpp"

namespace r0 { namespace builtin {

expression_tree Abs(const expression_tree::operands_t& ops, enviroment& env) {
    if ( ops.size() != 1 ) {
        env.raise_error("Abs", "called with invalid arguments");
        return expression_tree::make_operator("Abs", ops);
    }

    assert( ops[0].get_type() != expression_tree::APPROXIMATE_NUMBER );

    if ( ops[0].get_type() == expression_tree::EXACT_NUMBER ) {
        return expression_tree::make_exact_number( abs(ops[0].get_exact_number()) );
    } else {
        return expression_tree::make_operator("Abs", ops);
    }
} 


expression_tree numeric_Abs(const expression_tree::operands_t& ops, enviroment& env) {
    if ( ops.size() != 1 ) {
        env.raise_error("Abs", "called with invalid arguments");
        return expression_tree::make_operator("Abs", ops);
    }
    if ( ops[0].get_type() == expression_tree::EXACT_NUMBER ) {
        return expression_tree::make_exact_number( abs(ops[0].get_exact_number()) );
    } else if ( ops[0].get_type() == expression_tree::APPROXIMATE_NUMBER ) {
        return expression_tree::make_approximate_number( abs(ops[0].get_approximate_number()) );
    } else {
        return expression_tree::make_operator("Abs", ops);
    }
}

}} //namespace r0::builtin