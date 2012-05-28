
#include "builtin_log.hpp"

#include "enviroment.hpp"

#include <cassert>
#include <cmath>
#include <boost/assign.hpp>

namespace r0 { namespace builtin {

expression_tree Log(const expression_tree::operands_t& ops, enviroment& env) {
    if ( ops.size() == 0 || ops.size() > 2 ) {
        env.raise_error( "Log", "called with invalid arguments" );
        return expression_tree::make_operator("Log", ops);
    }
    if ( ops.size() == 1 ) {
        assert( ops[0].get_type() != expression_tree::APPROXIMATE_NUMBER );
        //Log[1] == 0
        switch( ops[0].get_type() ) {
        case expression_tree::EXACT_NUMBER :
            if ( ops[0].get_exact_number() == 1 ) {             
                return expression_tree::make_exact_number( mpq_class(0) ); 
            } else if ( ops[0].get_exact_number() == 0 ) {
                return expression_tree::make_operator( "Times", expression_tree::make_symbol("Infinity"), expression_tree::make_exact_number(mpq_class(-1)) ).evaluate(env); 
            } else if ( ops[0].get_exact_number() < 0 ) {
                env.raise_error( "Log", "Indeterminate expression encountered : Log[a] with a < 0" );
                return expression_tree::make_symbol("Indeterminate"); //Indeterminate?!
            }
            break;
        case expression_tree::SYMBOL :
            {
                symbol sym;
                if ( env.get_symbols().get_symbol( ops[0].get_symbol(), sym ) ) {
                    if ( sym.get_type() == symbol::CONSTANT && sym.get_name() == "E" ) {
                        return expression_tree::make_exact_number( 1 );
                    }
                }
            }
            break;
        case expression_tree::OPERATOR :
            if ( ops[0].get_operator_name() == "Power" ) {

                const expression_tree::operands_t& power_ops = ops[0].get_operands();

                assert(power_ops.size() == 2);

                const expression_tree& base = power_ops[0];
                const expression_tree& exp = power_ops[1];

                if ( base.get_type() == expression_tree::SYMBOL ) {
                    symbol sym;
                    if ( env.get_symbols().get_symbol( base.get_symbol(), sym ) ) {
                        if ( sym.get_type() == symbol::CONSTANT && sym.get_name() == "E" ) {
                            return exp;
                        }
                    }
                }

            } 
            break;            
            
        default:
            assert( false && "default case invoked" );
            break;
        }

        //default case
        return expression_tree::make_operator("Log", ops);
        
    } else { //ops.size() == 2
        assert(ops.size() == 2);
        assert( ops[0].get_type() != expression_tree::APPROXIMATE_NUMBER && ops[1].get_type() != expression_tree::APPROXIMATE_NUMBER );
        //Log[b,x] == Log[x]/Log[b]
        //Log[b,x] -> Divide[Log[x],Log[b]] OR Times[Log[x], Power[Log[b],-1]]

        const expression_tree& base = ops[0];
        const expression_tree& num = ops[1];


#if 0 //exact number gues
        if ( base.get_type() == expression_tree::EXACT_NUMBER &&
            num.get_type() == expression_tree::EXACT_NUMBER ) 
        {
            const mpq_class& base_num = base.get_exact_number(); 
            const mpq_class& num_num = num.get_exact_number();

            if ( base_num > 0 && num_num > 0 && base_num != 1 && base_num != num_num ) {

                if ( true ) {

                    if ( (base_num > 1 && base_num < num_num) ||
                        (base_num < 1 && base_num > num_num) ) {

                        std::cout << "1st" << std::endl;
                        std::cout << "base_num " << base_num << std::endl;
                        //O(n) algo, but meh...
                        mpq_class base_counter = base_num;

                        int c = 1;
                        do {
                            base_counter *= base_num;
                            ++c;
                        } while ( base_counter < num_num );

                        if ( base_counter == num_num ) {
                            return expression_tree::make_exact_number( c );
                        }
                    } else {

                        std::cout << "2nd" << std::endl;

                        //O(n) algo, but meh...
                        mpq_class base_counter = base_num;

                        int c = 1;
                        do {
                            base_counter /= base_num;
                            --c;
                        } while ( base_counter > num_num );

                        if ( base_counter == num_num ) {
                            return expression_tree::make_exact_number( c );
                        }
                    }

                } else { //base_num < 1



                }

            }
        }
#endif
        using boost::assign::list_of;

        return //Times[Log[x], Power[Log[b],-1]]
            expression_tree::make_operator( 
                "Times", list_of
                    ( expression_tree::make_operator( "Log", num ))
                    ( expression_tree::make_operator( "Power", list_of
                        (expression_tree::make_operator( "Log", base ))
                        (expression_tree::make_exact_number( -1 )
                    ))) 
            ).evaluate(env);

    }
}

expression_tree numeric_Log(const expression_tree::operands_t& ops, enviroment& env) {
    if ( ops.size() == 0 || ops.size() > 2 ) {
        env.raise_error( "numeric Log", "called with invalid arguments" );
        return expression_tree::make_operator("Log", ops);
    }
    if ( ops.size() == 1 ) {
        mpf_class val;
        bool do_log = false;
        if ( ops[0].get_type() == expression_tree::EXACT_NUMBER ) {
            val = ops[0].get_exact_number();
            do_log = true;
        } else if ( ops[0].get_type() == expression_tree::APPROXIMATE_NUMBER ) {
            val = ops[0].get_approximate_number();
            do_log = true;
        }
        if ( do_log ) {
            if ( val < 0 ) {
                env.raise_error( "numeric Log", "Indeterminate expression encountered : Log[a] with a < 0" );
                return expression_tree::make_symbol("Indeterminate"); //Indeterminate?!
            } else if ( val == 0 ) {
                return expression_tree::make_symbol("Infinity");
            }
            return expression_tree::make_approximate_number( mpf_class( std::log( val.get_d() ) ) );
        } else {
            return expression_tree::make_operator("Log", ops);
        }
    } else { //ops.size() == 2
        assert(ops.size() == 2);

        using boost::assign::list_of;

        return //Times[Log[x], Power[Log[b],-1]]
            expression_tree::make_operator( 
                "Times", 
                    expression_tree::make_operator( "Log", ops[1] ), 
                    expression_tree::make_operator( "Power", 
                        expression_tree::make_operator( "Log", ops[0] ), 
                        expression_tree::make_exact_number(mpq_class(-1)
                    ))
        ).evaluate(env, true);

    }
}

}} //namespace r0::builtin