
#include "builtin_rational.hpp"

#include "enviroment.hpp"

namespace r0 { namespace builtin {

expression_tree Rational(const expression_tree::operands_t& ops, enviroment& env) {
    if ( ops.size() != 2 ) {
        env.raise_error("Rational", "called with invalid arguments");
        return expression_tree::make_operator("Rational", ops);
    }
    //not worrying about exact, but non-integer argumnts..
    if ( ops[0].get_type() != expression_tree::EXACT_NUMBER || ops[1].get_type() != expression_tree::EXACT_NUMBER ) {
        env.raise_error("Rational", "called with non-exact arguments");
        return expression_tree::make_operator("Rational", ops);
    }
    return expression_tree::make_exact_number(mpq_class( ops[0].get_exact_number() / ops[1].get_exact_number() ) );
}

}} //namespace r0::builtin

