
#include "builtin_factorial.hpp"

#include "enviroment.hpp"
#include "expression_tree.hpp"
#include "util.hpp"

namespace r0 { namespace builtin {

expression_tree Factorial(const expression_tree::operands_t& ops, enviroment& env) {

    if ( ops.size() != 1 ) {
        env.raise_error("Factorial", "called with invalid arguments");
        return expression_tree::make_operator("Factorial", ops);
    }
    if ( ops[0].get_type() == expression_tree::EXACT_NUMBER ) {
        if ( ops[0].get_exact_number() < 0 || !is_mpq_integer(ops[0].get_exact_number()) ) {
            env.raise_error("Factorial", "negative and non-integer arguments are not supported");
            return expression_tree::make_operator("Factorial", ops);
        } else {

            unsigned long n = 0;
            bool n_conversion_success = mpz_get_ui_checked(n, ops[0].get_exact_number().get_num());
            
            if ( !n_conversion_success ) {
                env.raise_error("Factorial", "argument overflow");
                return expression_tree::make_operator("Factorial", ops);
            }

            mpz_class product;
            mpz_fac_ui(product.get_mpz_t(), n);

            return expression_tree::make_exact_number( mpq_class( product ) );
        }
    } else {
        return expression_tree::make_operator("Factorial", ops);
    }

}

}} //namespace r0::builtin
