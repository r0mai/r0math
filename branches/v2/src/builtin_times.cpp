
#include "builtin_times.hpp"

#include <cassert>
#include <map>
#include <iostream>
#include <algorithm>

#include "expression_tree.hpp"
#include "expression_tree_util.hpp"

namespace r0 { namespace builtin {

expression_tree Times(const expression_tree::operands_t& ops_to_copy, enviroment& env) {
#if 0 //ORIGINAL
    const expression_tree::operands_t& ops(ops_to_copy);

    mpq_class product(1);
    expression_tree::operands_t result_operands; //for variables etc.

    for ( unsigned i = 0; i < ops.size(); ++i ) {
        const expression_tree& current = ops[i];

        switch( current.get_type() ) {
        case expression_tree::EXACT_NUMBER :
            if ( current.get_exact_number() == 0 ) {
                return expression_tree::make_exact_number(0);
            }
            product *= current.get_exact_number();
            break;
        case expression_tree::SYMBOL :
            result_operands.push_back(current);
            break;
        case expression_tree::OPERATOR :
            assert ( current.get_operator_name() != "Times" );

            if ( current.get_operator_name() == "Plus" ) {
                result_operands.push_back(current); //TODO Expand
            } else {
                result_operands.push_back(current);
            }
            break;
        default :
            assert(false && "default case invoked");
            break;
        }
    }
    
    if ( product != 1 ) {
        result_operands.push_back( expression_tree::make_exact_number(product) );
    }

    if (result_operands.size() == 0) {
        return expression_tree::make_exact_number( mpq_class(1) );
    } else if (result_operands.size() == 1) {
        return result_operands[0];
    } else {
        return expression_tree::make_operator("Times", result_operands);
    }
#elif 1
    const expression_tree::operands_t& ops = ops_to_copy; //TODO rename later

    if ( ops.size() == 0 ) {
        return expression_tree::make_exact_number( 1 );
    } else if ( ops.size() == 1 ) {
        return ops[0];
    }

    //std::cout << "partly "  << expression_tree::make_operator( "Times", ops ).to_fullform(env) << std::endl;

    std::map<expression_tree, expression_tree::operands_t> product_map;

    for ( unsigned i = 0; i < ops.size(); ++i ) {
        const expression_tree& current = ops[i];

        switch( current.get_type() ) {
        case expression_tree::EXACT_NUMBER :
            if ( current.get_exact_number() == 0 ) {
                return expression_tree::make_exact_number( 0 );
            }
            product_map[ current ].push_back( expression_tree::make_exact_number( 1 ) );
            break;
        case expression_tree::SYMBOL :
            product_map[ current ].push_back( expression_tree::make_exact_number( 1 ) );
            break;
        case expression_tree::OPERATOR :
            {
                const std::string& op_name = current.get_operator_name();
                const expression_tree::operands_t& curr_ops = current.get_operands();

                assert(  op_name != "Times" ); //Flat

                if ( op_name == "Power" ) {
                    assert( curr_ops.size() == 2 );

                    const expression_tree& base = curr_ops[0];
                    const expression_tree& exp = curr_ops[1];

                    product_map[ base ].push_back(exp);
                } else {
                    product_map[ current ].push_back( expression_tree::make_exact_number( 1 ) );
                }

            }
            break;
        default :
            assert(false && "default case invoked");
            break;       
        }
    }

    expression_tree::operands_t result_product_ops;

    for ( auto it = product_map.begin(); it != product_map.end(); ++it ) {
        assert( !it->second.empty() );

        const expression_tree& base = it->first;
        const expression_tree exp = expression_tree::make_operator( "Plus", it->second )/*.evaluate(env)*/; //eval below

        result_product_ops.push_back( expression_tree::make_operator( "Power", base, exp ).evaluate(env) );

    }

    expression_tree::operands_t::iterator num_end = std::partition( result_product_ops.begin(), result_product_ops.end(), 
        [](const expression_tree& expr) {
            return expr.get_type() == expression_tree::EXACT_NUMBER;
        }
    );

    mpq_class numeric_product = 1;

    for ( auto it = result_product_ops.begin(); it != num_end; ++it ) {
        assert( it->get_exact_number() != 0 );
        numeric_product *= it->get_exact_number();
    }

    expression_tree::operands_t numeric_timed_result( num_end, result_product_ops.end() );

    if ( numeric_product != 1 ) {
        numeric_timed_result.push_back( expression_tree::make_exact_number( numeric_product ) );
    }

    if ( numeric_timed_result.size() == 0 ) {
        return expression_tree::make_exact_number( 1 ); //Times[1,1]
    } else if ( numeric_timed_result.size() == 1 ) {
        return numeric_timed_result[0];
    } else {
        return expression_tree::make_operator( "Times", numeric_timed_result );
    } 

/*
    //Times[] = 1
    if ( product_map.size() == 0 ) {
        return expression_tree::make_exact_number( 1 );
    }
*/

#elif 0
    expression_tree::operands_t ops(ops_to_copy);

    std::map<expression_tree, expression_tree::operands_t> unknown_product_map;

    for ( unsigned i = 0; i < ops.size(); ++i ) {
        const expression_tree& current = ops[i];

        switch( current.get_type() ) {
        case expression_tree::EXACT_NUMBER :
            if ( current.get_exact_number() == 0 ) {
                return expression_tree::make_exact_number( 0 );
            }
            unknown_product_map[ current ].push_back( expression_tree::make_exact_number( 1 ) );
            break;
        case expression_tree::SYMBOL :
            unknown_product_map[ current ].push_back( expression_tree::make_exact_number( 1 ) );
            break;
        case expression_tree::OPERATOR :
            {
                const std::string& op_name = current.get_operator_name();
                const expression_tree::operands_t& curr_ops = current.get_operands();

                if ( op_name == "Times" ) {
                    ops.insert( ops.end(), curr_ops.begin(), curr_ops.end() );
                } else if ( op_name == "Power" ) {
                    assert( curr_ops.size() == 2 );

                    const expression_tree& base = curr_ops[0];
                    const expression_tree& exp = curr_ops[1];

                    unknown_product_map[ base ].push_back(exp);

                } else {
                    unknown_product_map[ current ].push_back( expression_tree::make_exact_number( 1 ) );
                }
            }
            break;
        default :
            assert(false && "default case invoked");
            break;        
        }
    }

    expression_tree::operands_t result_product_ops;

  
    for ( auto it = unknown_product_map.begin(); it != unknown_product_map.end(); ++it ) {
        assert( !it->second.empty() );
        if ( it->second.size() == 1 ) {
            std::cout << "itt jartam : " << 
                expression_tree::make_operator( "Power", 
                    it->first,
                    it->second[0]
                ).evaluate(env).to_fullform(env) << std::endl;
            result_product_ops.push_back( 
                expression_tree::make_operator( "Power", 
                    it->first,
                    it->second[0]
                ).evaluate(env)
            );
        } else {
            std::cout << "itt NEM jartam" << std::endl;
            result_product_ops.push_back( 
                expression_tree::make_operator( "Power",
                    it->first,
                    expression_tree::make_operator( "Plus", it->second )
                ).evaluate(env)
            );
        }
    }

    

    assert( !result_product_ops.empty() );

    expression_tree::operands_t::iterator num_end = std::partition( result_product_ops.begin(), result_product_ops.end(),
        [](const expression_tree& expr) {
            return expr.get_type() == expression_tree::EXACT_NUMBER;
        }
    );

    mpq_class numeric_product = 1;
    for ( auto it = result_product_ops.begin(); it != num_end; ++it ) {
        assert(it->get_type() == expression_tree::EXACT_NUMBER);
        assert(it->get_exact_number() != 0); //check above in switch
        numeric_product *= it->get_exact_number();
    }

    expression_tree::operands_t numeric_timed_result( num_end, result_product_ops.end() );

    if ( numeric_product != 1 ) {
        numeric_timed_result.push_back( expression_tree::make_exact_number( numeric_product ) );
    }

    if ( numeric_timed_result.size() == 1 ) {
        return result_product_ops[0];
    } else {
        return expression_tree::make_operator( "Times", numeric_timed_result );
    }




#else
    expression_tree::operands_t ops(ops_to_copy);

    mpq_class product(1);
    //for things like x^3+x^4 -> x^7
    std::map<expression_tree, mpq_class> product_map;   

    for ( unsigned i = 0; i < ops.size(); ++i ) {
        const expression_tree& current = ops[i];

        switch( current.get_type() ) {
        case expression_tree::EXACT_NUMBER :
            if ( current.get_exact_number() == 0 ) {
                return expression_tree::make_exact_number(0);
            }
            product *= current.get_exact_number();
            break;
        case expression_tree::SYMBOL :
            product_map[current] += 1;
            break;
        case expression_tree::OPERATOR :
            assert( current.get_operator_name() != "Times" ); //Flat
            if ( current.get_operator_name() == "Power" ) {

                const mpq_class powerer = extract_exact_numeric_values( current.get_operands(), 1 );
                const expression_tree::operands_t powered_value = remove_exact_numeric_values( current.get_operands() );
                
                
                if (powered_value.size() == 1) {
                    product_map[ powered_value[0] ] += powerer;
                } else {
                    product_map[ expression_tree::make_operator( "Power", powered_value )] += powerer;
                }
            } else {
                product_map[current] += 1;
            }
            break;
        default :
            assert(false && "default case invoked");
            break;
        }
    }

    if (product_map.size() == 0) {
        return expression_tree::make_exact_number( product );
    } else {

        expression_tree::operands_t variables_res;

        std::for_each( product_map.begin(), product_map.end(), 
            [&env, &variables_res](const std::map<expression_tree, mpq_class>::value_type& val) {
                if ( val.second != 1 ) {
                    variables_res.push_back( expression_tree::make_operator( 
                        "Power", val.first, expression_tree::make_exact_number(val.second) ).evaluate(env) ); //just to short it?
                } else {
                    variables_res.push_back( val.first );
                }
            } 
        );

        if ( variables_res.empty() ) {
            return expression_tree::make_exact_number( product );
        }

        if ( product != 1 ) {
            variables_res.push_back( expression_tree::make_exact_number( product ) );
        }

        return expression_tree::make_operator("Times", variables_res);
    }
#endif

}

expression_tree numeric_Times(const expression_tree::operands_t& ops_to_copy, enviroment& env) {
    expression_tree::operands_t ops(ops_to_copy);

    mpf_class product(1);
    expression_tree::operands_t result_operands; //for variables etc.

    for ( unsigned i = 0; i < ops.size(); ++i ) {
        const expression_tree& current = ops[i];

        switch( current.get_type() ) {
        case expression_tree::EXACT_NUMBER :
            if ( current.get_exact_number() == 0 ) {
                return expression_tree::make_approximate_number(0);
            }
            product *= current.get_exact_number();
            break;
        case expression_tree::APPROXIMATE_NUMBER :
            if ( current.get_approximate_number() == 0 ) {
                return expression_tree::make_approximate_number(0);
            }
            product *= current.get_approximate_number();
            break;
        case expression_tree::SYMBOL :
            result_operands.push_back(current);
            break;
        case expression_tree::OPERATOR :
            if ( current.get_operator_name() == "Times" ) {
                ops.insert( ops.end(), current.get_operands().begin(), current.get_operands().end() );
            } else if ( current.get_operator_name() == "Plus" ) {
                result_operands.push_back(current); //TODO Expand
            } else {
                result_operands.push_back(current);
            }
            break;
        default :
            assert(false && "default case invoked");
            break;
        }
    }
    
    if ( product != 1 ) {
        result_operands.push_back( expression_tree::make_approximate_number(product) );
    }

    if (result_operands.size() == 0) {
        return expression_tree::make_approximate_number( mpf_class(1) );
    } else if (result_operands.size() == 1) {
        return result_operands[0];
    } else {
        return expression_tree::make_operator("Times", result_operands);
    }
}

}} //namespace r0::builtin