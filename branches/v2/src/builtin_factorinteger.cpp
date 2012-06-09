
#include "builtin_factorinteger.hpp"

#include "enviroment.hpp"
#include "builtin_primeq.hpp"

namespace r0 { namespace builtin {

expression_tree FactorInteger(const expression_tree::operands_t& ops, enviroment& env) {
    if ( ops.size() != 1 ) {
        env.raise_error("FactorInteger", "called with invalid arguments");
        return expression_tree::make_operator("FactorInteger", ops);
    }

    if ( !ops[0].get_type() == expression_tree::EXACT_NUMBER ) {
        env.raise_error("FactorInteger", "exact integer number expected at position 1");
        return expression_tree::make_operator("FactorInteger", ops);
    }

    //<prime, exponent>
    factorinteger_impl::factored_type factors = factorinteger_impl::factor_integer(ops[0].get_exact_number().get_num());
    
    //if rational then, insert the factored denominator, with negative exponents
    if ( !is_mpq_integer(ops[0].get_exact_number()) ) {

        const factorinteger_impl::factored_type factored_denom = factorinteger_impl::factor_integer(ops[0].get_exact_number().get_den());

        for ( auto it = factored_denom.begin(); it != factored_denom.end(); ++it ) {
            //no substracting as gmp keeps the numerator and denominator without common factors
            factors[it->first] = -it->second;
        }
    }

    expression_tree::operands_t factorlist;

    for ( auto it = factors.begin(); it != factors.end(); ++it ) {
        factorlist.push_back( 
            expression_tree::make_operator("List",
                expression_tree::make_exact_number(it->first),
                expression_tree::make_exact_number(it->second)
            )    
        );
    }

    return expression_tree::make_operator("List", factorlist);

}

namespace factorinteger_impl {

factored_type factor_integer(mpz_class number) {

    //<prime, exponent>
    factored_type factors; 

    if ( number == -1 || number == 0 || number == 1 ) {
        factors[number] = 1;
        return factors;
    }

    if ( number < 0 ) {
        factors[-1] = 1;
        number = -number;
    }

    mpz_class last_factor = 2;

    while ( number != 1 ) {
        //std::cout << "number = " << number << std::endl;
        
        last_factor = primeq_impl::trial_division(number, last_factor);

        factors[last_factor]++;

        number /= last_factor;
    }

    return factors;
}

} //namespace factorinteger_impl

}} //namespace r0::builtin