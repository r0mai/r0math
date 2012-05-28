
#include "builtin_divide.hpp"

#include "enviroment.hpp"
#include "expression_tree.hpp"

namespace r0 { namespace builtin {

expression_tree Divide(const expression_tree::operands_t& ops, enviroment& env) {
    if ( ops.size() != 2 ) {
        env.raise_error( "Divide", "called with invalid arguments" );
        return expression_tree::make_operator("Divide", ops);
    }

    return 
    expression_tree::make_operator("Times",
        ops[0],
        expression_tree::make_operator("Power", ops[1], expression_tree::make_exact_number(mpq_class(-1)))
    ).evaluate(env);
    /*
    expression_tree::operands_t ops2;
    ops2.push_back( ops[1] );
    ops2.push_back( expression_tree::make_exact_number(mpq_class(-1)) );

    expression_tree pow = expression_tree::make_operator("Power", ops2);

    expression_tree::operands_t ops3;
    ops3.push_back( ops[0] );
    ops3.push_back( pow );

    return expression_tree::make_operator("Times", ops3).evaluate(env);
    */
}

}} //namespace r0::builtin