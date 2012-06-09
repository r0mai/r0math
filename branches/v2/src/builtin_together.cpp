
#include "builtin_together.hpp"
#include "enviroment.hpp"

#include <utility>

namespace r0 { namespace builtin {

expression_tree Together(const expression_tree::operands_t& ops, enviroment& env) {
    if ( ops.size() != 1 ) {
        env.raise_error("Together", "called with invalid arguments");
        return expression_tree::make_operator("Together", ops);
    }

    const expression_tree& expr = ops[0];

    if ( !expr.is_operator("Plus") ) {
        return expr;
    }

    //std::vector< PLUS[std::pair<NUMERATOR, DENOMINATOR>] >

    std::vector< std::pair<expression_tree, expression_tree> > fraction_matrix;

    const expression_tree::operands_t& plus_ops = expr.get_operands();

    for ( unsigned i = 0; i < plus_ops.size(); ++i ) {

        const expression_tree& current_op = plus_ops[i];


        if ( current_op.get_type() == expression_tree::EXACT_NUMBER ) {

            fraction_matrix.push_back( std::make_pair(
                expression_tree::make_exact_number( current_op.get_exact_number().get_num() ),
                expression_tree::make_exact_number( current_op.get_exact_number().get_den() )
            ) );

        } else if ( current_op.is_operator("Times") ) {

            const expression_tree::operands_t& times_op = current_op.get_operands();

            expression_tree::operands_t numer;
            expression_tree::operands_t denom;

            for ( unsigned j = 0; j < times_op.size(); ++j ) {

                expression_tree times_result;

                if ( times_op[j].get_type() == expression_tree::EXACT_NUMBER ) {
                    numer.push_back( expression_tree::make_exact_number( times_op[j].get_exact_number().get_num() ) );
                    denom.push_back( expression_tree::make_exact_number( times_op[j].get_exact_number().get_den() ) );
                } else {
                    if ( together_impl::to_negative_exponent( times_op[j], times_result ) ) {
                        denom.push_back(times_result);
                    } else {
                        numer.push_back(times_op[j]);
                    }
                }
            }

            fraction_matrix.push_back( std::make_pair( 
                expression_tree::make_operator("Times", numer).evaluate(env), 
                expression_tree::make_operator("Times", denom).evaluate(env)
            ) );
        } else if ( current_op.is_operator("Power") ) {

            expression_tree exp_result;

            if ( together_impl::to_negative_exponent( current_op, exp_result ) ) {
                fraction_matrix.push_back( std::make_pair(                    
                    expression_tree::make_exact_number(1),
                    exp_result
                ) );
            } else {
                fraction_matrix.push_back( std::make_pair(
                    current_op,
                    expression_tree::make_exact_number(1)                                       
                ) );
            }

        } else { //!current_op.is_operator("Times" || "Power")
            // current_op/1
            fraction_matrix.push_back( std::make_pair( 
                current_op, 
                expression_tree::make_exact_number(1)
            ) );
        }
    }
    /*
    for ( unsigned i = 0; i < fraction_matrix.size(); ++i ) {
        std::cout << '(' << fraction_matrix[i].first.to_inputform(env) << ")/(" << fraction_matrix[i].second.to_inputform(env) << ")\n";
    }
    */
    expression_tree::operands_t numerator_plus;
    expression_tree::operands_t denominator_times;

    for ( unsigned i = 0; i < fraction_matrix.size(); ++i ) {

        expression_tree::operands_t numerator_times_element;

        numerator_times_element.push_back( fraction_matrix[i].first );

        for ( unsigned j = 0; j < fraction_matrix.size(); ++j ) {
            if ( i != j ) {
                numerator_times_element.push_back( fraction_matrix[j].second );
            }
        }

        numerator_plus.push_back( expression_tree::make_operator("Times", numerator_times_element) );
        denominator_times.push_back( fraction_matrix[i].second );
    }

    
    return expression_tree::make_operator( "Times",
        expression_tree::make_operator( "Plus", numerator_plus ),
        expression_tree::make_operator( "Power", 
            expression_tree::make_operator( "Times", denominator_times ),
            expression_tree::make_exact_number( -1 )
        )
    ).evaluate(env);

}

namespace together_impl {

//returns true if negative, and sets result to the negated
bool to_negative_exponent(const expression_tree& expr, expression_tree& result) {
    if ( expr.is_operator("Power") ) {
        assert( expr.get_operands().size() == 2 );

        const expression_tree& base = expr.get_operands()[0];
        const expression_tree& exp = expr.get_operands()[1];

        if ( exp.get_type() == expression_tree::EXACT_NUMBER ) {
            if ( exp.get_exact_number() < 0 ) {
                result = expression_tree::make_operator("Power", base, expression_tree::make_exact_number(-exp.get_exact_number()));
                return true;
            }
        } else if ( exp.get_type() == expression_tree::APPROXIMATE_NUMBER ) {
            if ( exp.get_approximate_number() < 0 ) {
                result = expression_tree::make_operator("Power", base, expression_tree::make_approximate_number(-exp.get_approximate_number()));
                return true;
            }
        }
    }

    return false;
}

} //namespace together_impl

}} //namespace r0::builtin