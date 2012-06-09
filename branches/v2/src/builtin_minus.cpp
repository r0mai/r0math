
#include "builtin_minus.hpp"

#include "enviroment.hpp"
#include "expression_tree.hpp"

namespace r0 { namespace builtin {
    
expression_tree Minus(const expression_tree::operands_t& ops, enviroment& env) {
    if ( ops.size() != 1 ) {
        env.raise_error( "Minus", "called with invalid arguments" );
        return expression_tree::make_operator("Minus", ops);
    }

    expression_tree::operands_t res_ops;

    res_ops.push_back( expression_tree::make_exact_number(mpq_class(-1)) );
    res_ops.push_back( ops[0] );

    return expression_tree::make_operator("Times", res_ops).evaluate(env);
}

}} //namespace r0::builtin