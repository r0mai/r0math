
#include "builtin_power.hpp"

#include <cassert>
#include <cmath>

#include "enviroment.hpp"
#include "expression_tree.hpp"

namespace r0 { namespace builtin {

expression_tree Power(const expression_tree::operands_t& ops_to_copy, enviroment& env) {

    expression_tree::operands_t ops(ops_to_copy);

    if ( ops.size() == 0 ) {
        return expression_tree::make_exact_number( mpq_class(1) );   
    }

    if ( ops.size() == 1 ) {
        return expression_tree( ops[0] );   
    }
    
    assert( ops.size() == 2 );

    assert( ops[0].get_type() != expression_tree::APPROXIMATE_NUMBER );
    assert( ops[1].get_type() != expression_tree::APPROXIMATE_NUMBER );


    //if exponent is an integer, then there are possible simplifications
    if ( ops[1].is_integer() ) {

        //(a^b)^c == a^(bc) if c is an integer
        if ( ops[0].is_operator("Power") ) {

            const expression_tree::operands_t& power_ops = ops[0].get_operands();

            assert( power_ops.size() == 2 );

            ops[1] = expression_tree::make_operator("Times", power_ops[1], ops[1]).evaluate(env);
            ops[0] = power_ops[0];

        }

    }

    //x^0 == 0
    //x^1 == x
    //But 0^0 is Indeterminate, TODO!!?? DEBUG : Expand[Det[{{n,n,n},{n+1,n+1,n+2},{n+3,n+2,n+1}}]]
    if ( ops[1].get_type() == expression_tree::EXACT_NUMBER ) {
        if ( ops[1].get_exact_number() == 0 ) {
            if ( ops[0].get_type() == expression_tree::EXACT_NUMBER ) {
                env.raise_error( "Power", "Indeterminate expression 0^0 encountered." );
                return expression_tree::make_symbol( "Indeterminate" );
            } else {
                return expression_tree::make_exact_number( 1 );
            }
        } else if ( ops[1].get_exact_number() == 1 ) {
            return ops[0];
        }        
    }

    if ( ops[0].get_type() != expression_tree::EXACT_NUMBER || ops[1].get_type() != expression_tree::EXACT_NUMBER ) {
        return expression_tree::make_operator( "Power", ops ); //nonnumber nonpowering
    }

    
    //TODO check if fits
    //If the don't fit into these, then why bother calculating?

    long exponent = 0;
    unsigned long inverse_exponent = 0;

    bool exponent_conversion_success = mpz_get_si_checked(exponent, ops[1].get_exact_number().get_num());
    bool inverse_exponent_conversion_success = mpz_get_ui_checked(inverse_exponent, ops[1].get_exact_number().get_den());

    if ( !exponent_conversion_success || !inverse_exponent_conversion_success ) {
        env.raise_error( "Power", "exponent overflow" );
        return expression_tree::make_operator( "Power", ops );
    }

    if ( ops[0].get_exact_number() < 0 ) {
        if ( inverse_exponent != 1 ) {
            env.raise_error( "Power", "can't raise a negative base to a rational exponent" );
            return expression_tree::make_symbol( "Indeterminate" );
        }
    } else if ( ops[0].get_exact_number() == 0 ) {
        if ( exponent > 0 ) {
            return expression_tree( ops[0] ); // mpq_class(0) why reconstruct?
        } else if ( exponent == 0 ) {
            env.raise_error( "Power", "Indeterminate 0^0" );
            return expression_tree::make_symbol( "Indeterminate" );
        } else { //exponent < 0
            env.raise_error( "Power", "Infinite expression 0^-exp" );
            return expression_tree::make_symbol( "Infinity" );
        }
    } else { //ops[0].get_exact_number() > 0
        //everything is mathOK here
    }
    //shortcut for Power[a, -1]
    if ( exponent == -1 && inverse_exponent == 1 ) {
        mpq_class result = mpq_class(1) / ops[0].get_exact_number();
        return expression_tree::make_exact_number( result );
    }

    bool negative_exponent = false;
    if ( exponent < 0 ) {
        negative_exponent = true;
        exponent = -exponent;
    }        

    mpq_class resultq = mpq_pow_ui_class( ops[0].get_exact_number(), exponent );

    if (negative_exponent) {
        resultq = mpq_class(1) / resultq;
    }
    if ( inverse_exponent == 1 ) {
        return expression_tree::make_exact_number( resultq );
    } else {
        //ops[1] is now equals 1/d. We did the numerator's job above.
        //ops[1] = mpq_class( mpz_class(1), ops[1].get_exact_number().get_den() ); //implicit constructor

        //TODO check if fits
        //unsigned long inverse_exponent = ops[1].get_exact_number().get_den().get_ui();

        mpz_class numerator_result;
        mpz_class denominator_result;

        bool numerator_success = mpz_root_class( numerator_result, resultq.get_num(), inverse_exponent);
        bool denominator_success = mpz_root_class( denominator_result, resultq.get_den(), inverse_exponent);

        if ( numerator_success && denominator_success ) {
            mpq_class result( numerator_result, denominator_result );
            return expression_tree::make_exact_number( result );
        } else if ( numerator_success ) { //&& !denominator_success
            /*
                Creating :
                Times[numerator_result, Power[resultq.get_den(), -1/inverse_exponent]]
            */                
            expression_tree::operands_t ops_power;
            expression_tree::operands_t ops_times;

            mpq_class negated_exponent( mpq_class(-1)*mpq_class(1, inverse_exponent) );

            ops_power.push_back( expression_tree::make_exact_number( mpq_class(resultq.get_den()) ) );
            ops_power.push_back( expression_tree::make_exact_number( negated_exponent ) ); //could use 1/inverse_exponent also
            expression_tree power_expr = expression_tree::make_operator( "Power", ops_power ); //don't have to evaulate, can't do anything anyway

            if ( numerator_result == 1 ) {
                return power_expr;
            } else {
                ops_times.push_back( expression_tree::make_exact_number( mpq_class(numerator_result) ) );
                ops_times.push_back( power_expr );
                return expression_tree::make_operator("Times", ops_times)/*.evaluate()*/;
            }
        } else if ( denominator_success ) { //&& !numerator_success
            /*
                Creating :
                Times[Power[resultq.get_num(), 1/inverse_exponent], 1/denominator_result]
            */
            expression_tree::operands_t ops_powerlhs;
            expression_tree::operands_t ops_times;

            ops_powerlhs.push_back( expression_tree::make_exact_number( mpq_class(resultq.get_num()) ) );
            ops_powerlhs.push_back( expression_tree::make_exact_number( mpq_class(1, inverse_exponent) ) );
            expression_tree powerlhs_tree = expression_tree::make_operator("Power", ops_powerlhs);

            if ( denominator_result == 1 ) {
                return powerlhs_tree;
            } else {
                ops_times.push_back( powerlhs_tree );
                ops_times.push_back( expression_tree::make_exact_number(mpq_class( mpz_class(1), denominator_result )) );
                return expression_tree::make_operator("Times", ops_times)/*.evaluate()*/; //FIXME might evaulate this? no...
            }

        } else { //!denominator_success && !numerator_success
            /*
                Creating :
                Power[resultq, exponent/inverse_exponent]
            */
            expression_tree::operands_t ops_power;
            ops_power.push_back( expression_tree::make_exact_number( resultq ) );
            ops_power.push_back( expression_tree::make_exact_number( mpq_class(1, inverse_exponent) ) );
            return expression_tree::make_operator("Power", ops_power);
        }

    }
    
}

expression_tree numeric_Power(const expression_tree::operands_t& ops, enviroment& env) {
    if ( ops.size() != 2 ) {
        env.raise_error("numeric Power", "called with invalid arguments");
        return expression_tree::make_operator( "Power", ops );       
    }

    //double powering
    mpf_class base;

    
    switch ( ops[0].get_type() ) {
    case expression_tree::EXACT_NUMBER :
        base = ops[0].get_exact_number();
        break;
    case expression_tree::APPROXIMATE_NUMBER :
        base = ops[0].get_approximate_number();
        break;
    default:
        return expression_tree::make_operator("Power", ops);
    }

    //if exp is an integral value, then use pow(double, int) version

    const bool approx_num = ops[1].get_type() == expression_tree::APPROXIMATE_NUMBER;
    const bool rational_num = ops[1].get_type() == expression_tree::EXACT_NUMBER && !is_mpq_integer(ops[1].get_exact_number());

    if (approx_num || rational_num) {
        const mpf_class exp = ops[1].get_number_as_mpf();
        if ( base < 0 || (base == 0 && exp < 0) ) {
            env.raise_error( "numeric Power", "Indeterminate expression encountered" );
            return expression_tree::make_symbol("Indeterminate");
        } else if ( base == 0 && exp < 0 ) {
            return expression_tree::make_symbol("Infinity");
        }
        return expression_tree::make_approximate_number( mpf_class(std::pow(base.get_d(), exp.get_d())) );

    } else if ( ops[1].get_type() == expression_tree::EXACT_NUMBER ) {

        long exp = 0;

        if ( !mpz_get_si_checked( exp, ops[1].get_exact_number() ) ) {
            env.raise_error("numeric Power", "exponent overflow");
            return expression_tree::make_operator("Power", ops);
        }
        if ( base == 0 && exp < 0 ) {
            return expression_tree::make_symbol("Infinity");
        }

        return expression_tree::make_approximate_number( mpf_class(std::pow(base.get_d(), exp)) );

    } else {
        return expression_tree::make_operator("Power", ops);
    }
    
}

}} //namespace r0::builtin