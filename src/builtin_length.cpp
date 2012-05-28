
#include "builtin_length.hpp"
#include "enviroment.hpp"
#include "expression_tree.hpp"

namespace r0 { namespace builtin {

expression_tree Length(const expression_tree::operands_t& ops, enviroment& env) {
    if ( ops.size() != 1 ) {
        env.raise_error("Length", "called with invalid arguments");
        return expression_tree::make_operator("Length", ops);
    }
    if ( ops[0].get_type() == expression_tree::OPERATOR ) {
        return expression_tree::make_exact_number( ops[0].get_operands().size() );
    } else {
        return expression_tree::make_exact_number( 0 );
    }
}

}} //namespace r0::builtin