
#include "builtin_integerdigits.hpp"

#include "enviroment.hpp"
#include "util.hpp"
#include "expression_tree.hpp"

namespace r0 { namespace builtin {

expression_tree IntegerDigits(const expression_tree::operands_t& ops, enviroment& env) {
    
    if ( ops.size() == 0 || ops.size() > 2 ) {
        env.raise_error("IntegerDigits", "called with invliad arguments");
        return expression_tree::make_operator("IntegerDigits", ops);
    }
    //TODO REWRITE !!!
    expression_tree number_expr( ops[0] );
    expression_tree base_expr = expression_tree::make_exact_number( mpq_class(10) );

    if ( ops.size() == 2 ) {
        base_expr = ops[1];
    }

    if ( number_expr.get_type() == expression_tree::EXACT_NUMBER && base_expr.get_type() == expression_tree::EXACT_NUMBER ) {
        if ( is_mpq_integer(number_expr.get_exact_number()) && is_mpq_integer(base_expr.get_exact_number()) ) {

            mpz_class number = abs(number_expr.get_exact_number().get_num());
            mpz_class base = base_expr.get_exact_number().get_num();

            if ( base <= 1 ) { 
                env.raise_error( "IntegerDigits", "integer greater than 1 expected as base" );
                return expression_tree::make_operator("IntegerDigits", ops);
            }


            if ( number == 0 ) {
                expression_tree::operands_t list_ops;
                list_ops.push_back( expression_tree::make_exact_number( mpq_class(0) ) );
                return expression_tree::make_operator("List", list_ops);
            }

            expression_tree::operands_t rev_digits_list;
            //TODO optimizeable doing rem and div in one operation
            while ( number != 0 ) {
                rev_digits_list.push_back( expression_tree::make_exact_number( mpq_class(number % base) ) );
                number /= base;
            }
            return expression_tree::make_operator("List", expression_tree::operands_t( rev_digits_list.rbegin(), rev_digits_list.rend() ) );
        } else {
            env.raise_error( "IntegerDigits", "integers expected both as base and number" );
            return expression_tree::make_operator("IntegerDigits", ops);
        }
    } else {
        return expression_tree::make_operator("IntegerDigits", ops);
    }

}

}} //namespace r0::builtin