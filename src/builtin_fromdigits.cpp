
#include "builtin_fromdigits.hpp"
#include "enviroment.hpp"

#include <boost/assign/list_of.hpp>

namespace r0 { namespace builtin {

expression_tree FromDigits(const expression_tree::operands_t& ops, enviroment& env) {
    if ( ops.empty() || ops.size() > 2 ) {
        env.raise_error("FromDigits", "called with invalid arguments");
        return expression_tree::make_operator("FromDigits", ops);
    }

    expression_tree base = expression_tree::make_exact_number( 10 );

    if ( ops.size() == 2 ) {
        base = ops[1];
    }

    if ( !ops[0].is_operator("List") ) {
        env.raise_error("FromDigits", "a list expected at position 1");
        return expression_tree::make_operator("FromDigits", ops);
    }

    if ( ops[0].get_operands().size() == 2 && ops[0].get_operands()[0].is_operator("List") ) {
        //Q number

        const expression_tree::operands_t& digits = ops[0].get_operands()[0].get_operands();
        expression_tree::operands_t rep_digits;

        const expression_tree& exponent = ops[0].get_operands()[1];

        int digits_size = digits.size();

        if ( !digits.empty() && digits.back().is_operator("List") ) {
            rep_digits = digits.back().get_operands();
            --digits_size;
        }

        expression_tree::operands_t result_plus;

        for ( int i = 0; i < digits_size; ++i ) {
            result_plus.push_back(
                expression_tree::make_operator( "Times",
                    digits[i],
                    expression_tree::make_operator( "Power",
                        base,
                        expression_tree::make_operator( "Plus",
                            expression_tree::make_exact_number( mpq_class(-i-1, 1) ),
                            exponent
                        )
                    )
                ).evaluate(env)
            );
        }

        for ( int i = 0; i < static_cast<int>(rep_digits.size()); ++i ) {

            result_plus.push_back(
                expression_tree::make_operator( "Times", boost::assign::list_of(
                    rep_digits[i])(
                    expression_tree::make_operator( "Power",
                        base,
                        expression_tree::make_operator( "Plus",
                            expression_tree::make_exact_number( mpq_class(-i + static_cast<int>(rep_digits.size()) - digits_size - 1, 1) ),
                            exponent
                        )
                    ))(
                    expression_tree::make_operator( "Power",
                        expression_tree::make_operator( "Plus",
                            expression_tree::make_exact_number( mpq_class(-1, 1) ),
                            expression_tree::make_operator( "Power",
                                base,
                                expression_tree::make_exact_number( rep_digits.size() )
                            )
                        ),
                        expression_tree::make_exact_number( mpq_class(-1, 1) )
                    )
                )).evaluate(env)
            );
        }

        return expression_tree::make_operator("Plus", result_plus).evaluate(env);

    } else {
        //Z number
        const expression_tree::operands_t& digits = ops[0].get_operands();

        expression_tree::operands_t result_plus;

        for ( unsigned i = 0; i < digits.size(); ++i ) {
            result_plus.push_back(
                expression_tree::make_operator( "Times",
                    digits[ digits.size() - i - 1 ],
                    expression_tree::make_operator( "Power",
                        base,
                        expression_tree::make_exact_number( i )
                    )
                )    
            );
        }
        return expression_tree::make_operator("Plus", result_plus).evaluate(env);
    }

    

}

}} //namespace r0::builtin