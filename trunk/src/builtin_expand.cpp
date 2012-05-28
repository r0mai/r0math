
#include "builtin_expand.hpp"
#include "enviroment.hpp"

#include <boost/assign.hpp>

namespace r0 { namespace builtin {

expression_tree Expand(const expression_tree::operands_t& ops, enviroment& env) {
    if ( ops.size() != 1 ) {
        env.raise_error("Expand", "called with invalid arguments");
        return expression_tree::make_operator("Expand", ops);
    }

    return expand_impl::do_expand(ops[0], env);
    
}

namespace expand_impl {

expression_tree do_expand( const expression_tree& expr, enviroment& env) {


    switch (expr.get_type()) {
    default:
        assert( false && "default case invoked" );
        return expression_tree::make_operator("Expand", expression_tree::operands_t(1, expr));

    case expression_tree::EXACT_NUMBER :
    case expression_tree::APPROXIMATE_NUMBER :
    case expression_tree::SYMBOL :
        return expr;

    case expression_tree::OPERATOR :
        {
            //terms[0] * terms[1] * ... * terms[n]
            //terms[i] = terms[i][0] + terms[i][1] + ... + terms[i][k]
            //std::vector< expression_tree::operands_t > terms;

            expand_impl::term_matrix_t term_matrix;

            const std::string& op_name = expr.get_operator_name();
            const expression_tree::operands_t& exprops = expr.get_operands();

            if ( op_name == "Power" ) {

                expand_impl::add_power_to_terms( exprops, term_matrix );

            } else if ( op_name == "Times" ) {

                expand_impl::add_times_to_terms( exprops, term_matrix );

            } else if ( op_name == "Plus" ) {

                expression_tree::operands_t sum;

                for ( unsigned i = 0; i < exprops.size(); ++i ) {
                    sum.push_back( do_expand( exprops[i], env ) );
                }

                return expression_tree::make_operator("Plus", sum).evaluate(env);

            } else {
                return expr;
            }

            //multiplying terms

            assert( !term_matrix.empty() );

            expression_tree::operands_t result_plus_ops(term_matrix[0]);
            for ( unsigned i = 1; i < term_matrix.size(); ++i ) {

                expression_tree::operands_t new_result_plus_ops;

                for ( unsigned x = 0; x < result_plus_ops.size(); ++x ) {
                    for ( unsigned y = 0; y < term_matrix[i].size(); ++y ) {
                        new_result_plus_ops.push_back( 
                            expression_tree::make_operator( "Times", 
                                result_plus_ops[x], 
                                term_matrix[i][y]
                            ).evaluate(env) 
                        );
                    }
                }

                result_plus_ops = new_result_plus_ops;

            }

            return expression_tree::make_operator( "Plus", result_plus_ops ).evaluate(env);

        }
    }
}

void add_times_to_terms( const expression_tree::operands_t& ops, term_matrix_t& term_matrix ) {
    for ( unsigned i = 0; i < ops.size(); ++i ) {

        if ( ops[i].get_type() == expression_tree::OPERATOR ) {
            const std::string& op_name = ops[i].get_operator_name();
            const expression_tree::operands_t& exprops = ops[i].get_operands();

            if ( op_name == "Plus" ) {
                term_matrix.push_back( ops[i].get_operands() );
            } else if ( op_name == "Power" ) {
                add_power_to_terms(exprops, term_matrix);
            } else {
                term_matrix.push_back( expression_tree::operands_t(1, ops[i]) );
            }
        } else {
            term_matrix.push_back( expression_tree::operands_t(1, ops[i]) );
        }
    }
}

void add_power_to_terms( const expression_tree::operands_t& ops, term_matrix_t& term_matrix ) {
    assert ( ops.size() == 2 );

    const expression_tree& base = ops[0];
    const expression_tree& exp = ops[1];

    if ( exp.get_type() == expression_tree::EXACT_NUMBER &&
        is_mpq_integer(exp.get_exact_number()) &&
        exp.get_exact_number() >= 2 && 
        base.get_type() == expression_tree::OPERATOR &&
        base.get_operator_name() == "Plus")
    {

        const expression_tree::operands_t& base_ops = base.get_operands();
        const mpz_class& exp_mpz = exp.get_exact_number().get_num();

        unsigned long exp_ui = 0;
        assert( mpz_get_ui_checked(exp_ui, exp_mpz) );

        if ( base_ops.size() == 2 ) {

            //n = exp_ui;
            //Binomial[n, k] = n!/(k!(n-k)!) = Product[n-a+1,{a,1,k}] / Product[a,{a,1,k}]

            //Binomial formula
            //(a+b)^n = Sum[Binomial[n, k] a^(n-k) b^k, {k, 0, n}]
            const expression_tree& a = base_ops[0];
            const expression_tree& b = base_ops[1];

            //Binomial[n, k] == Binomial[n, n-k]

            expression_tree::operands_t result_sum;

            mpz_class binomial_numerator = 1;
            mpz_class binomial_denominator = 1;

            for ( unsigned long k = 0; k <= exp_ui / 2; ++k ) {
                
                if ( k != 0 ) {
                    binomial_numerator *= exp_ui - k + 1;
                    if ( k != 1 ) { 
                        binomial_denominator *= k;
                    }
                }

                result_sum.push_back( 
                    expression_tree::make_operator("Times", boost::assign::list_of
                        ( expression_tree::make_exact_number(binomial_numerator/binomial_denominator) )
                        ( expression_tree::make_operator( "Power", a, expression_tree::make_exact_number( exp_ui - k ) ) )
                        ( expression_tree::make_operator( "Power", b, expression_tree::make_exact_number( k ) ) )
                    )
                );

                //don't count possible middle element twice
                if ( k != exp_ui - k ) {
                    result_sum.push_back( 
                        expression_tree::make_operator("Times", boost::assign::list_of
                            ( expression_tree::make_exact_number(binomial_numerator/binomial_denominator) )
                            ( expression_tree::make_operator( "Power", a, expression_tree::make_exact_number( k ) ) )
                            ( expression_tree::make_operator( "Power", b, expression_tree::make_exact_number( exp_ui - k ) ) )
                        )
                    );
                }
                //std::cout << "bin : " << k << " : " << binomial_numerator/binomial_denominator << std::endl;

            }

            term_matrix.push_back( result_sum );

        } else {
            for ( unsigned long i = 0; i < exp_ui; ++i ) {
                term_matrix.push_back( base_ops ); //base is Plus
            }
        }
    } else {
        term_matrix.push_back( term_matrix_t::value_type( 1, expression_tree::make_operator("Power", ops) ) );
    }
}

} //namespace expand_impl

}} //namespace r0::builtin