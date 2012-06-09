
#include "builtin_plus.hpp"

#include <cassert>
#include <map>
#include <iterator>
#include <algorithm>
#include <iostream>

#include "util.hpp"
#include "builtin_numericq.hpp"
#include "expression_tree.hpp"
#include "expression_tree_util.hpp"

namespace r0 { namespace builtin {

expression_tree Plus(const expression_tree::operands_t& ops, enviroment& env) {   
    return plus_impl::Plus(ops, env); 
}

expression_tree numeric_Plus(const expression_tree::operands_t& ops, enviroment& env) {
    return plus_impl::numeric_Plus(ops, env);
}

namespace plus_impl {

expression_tree Plus(expression_tree::operands_t ops, enviroment& env) {
#if 0

    mpq_class sum(0);
    std::map<expression_tree, expression_tree::operands_t> unknown_sums_map; 

    for ( unsigned i = 0; i < ops.size(); ++i ) {
        const expression_tree& current = ops[i];

        switch( current.get_type() ) {
        default :
            assert(false && "default case invoked");
            break;     
        case expression_tree::EXACT_NUMBER : 
            sum += current.get_exact_number();
            break;
        case expression_tree::SYMBOL : 
            unknown_sums_map[ current ].push_back( expression_tree::make_exact_number( 1 ) );
            break;
        case expression_tree::OPERATOR :
            {
                const std::string& op_name = current.get_operator_name();
                expression_tree::operands_t curr_ops = current.get_operands();

                if ( op_name == "Plus" ) {
                    ops.insert( ops.end(), curr_ops.begin(), curr_ops.end() );
                } else if ( op_name == "Times" ) {
                    expression_tree::operands_t::iterator numeric_end = 
                        std::stable_partition( curr_ops.begin(), curr_ops.end(),
                            [&env](const expression_tree& x) {
                                return numericq_impl::recursive_numericq( x, env );   
                            }
                    );
                    const expression_tree::operands_t nums( curr_ops.begin(), numeric_end );
                    const expression_tree::operands_t vars( numeric_end, curr_ops.end() );
                        
                    unknown_sums_map[ expression_tree::make_operator( "Times", vars ) ].push_back( expression_tree::make_operator( "Times", nums ) );

                } else {
                    unknown_sums_map[ current ].push_back( expression_tree::make_exact_number( 1 ) );
                }
            }
            break;
        }
    }

    expression_tree::operands_t result_sum_ops;

    for ( auto it = unknown_sums_map.begin(); it != unknown_sums_map.end(); ++it ) {
        assert( !it->second.empty() );
        

        result_sum_ops.push_back( expression_tree::make_operator( "Times", 
            it->first,
            non_numeric_only_numericq_plus(it->second, env)
        ) );
    }

    result_sum_ops.push_back( expression_tree::make_exact_number(sum) );

    return expression_tree::make_operator( "Plus", result_sum_ops );

#elif 0
    mpq_class sum(0);

    std::map<expression_tree, expression_tree::operands_t> sums_map;

    for ( unsigned i = 0; i < ops.size(); ++i ) {
        const expression_tree& current = ops[i];

        switch( current.get_type() ) {
        case expression_tree::EXACT_NUMBER : 
            sum += current.get_exact_number();
            break;
        case expression_tree::SYMBOL :

            break;
        default :
            assert(false && "default case invoked");
            break;        
        }
    }

#elif 0
    mpq_class sum(0);

    //for things like 3x+4x -> 7x
    std::map<expression_tree, mpq_class> sums_map;

    for ( unsigned i = 0; i < ops.size(); ++i ) {
        const expression_tree& current = ops[i];

        switch( current.get_type() ) {
        case expression_tree::EXACT_NUMBER :
            sum += current.get_exact_number();
            break;
        case expression_tree::SYMBOL :
            sums_map[current] += 1;
            break;
        case expression_tree::OPERATOR :
            if ( current.get_operator_name() == "Plus" ) {
                ops.insert( ops.end(), current.get_operands().begin(), current.get_operands().end() );
            } else if ( current.get_operator_name() == "Times" ) {                


                const mpq_class timer = extract_exact_numeric_values( current.get_operands(), 0 );
                const expression_tree::operands_t timed_value = remove_exact_numeric_values( current.get_operands() );
                
                
                if (timed_value.size() == 1) {
                    sums_map[ timed_value[0] ] += timer;
                } else {
                    sums_map[ expression_tree::make_operator( "Times", timed_value )] += timer;
                }

                

            } else {
                sums_map[current] += 1;
                
            }
            break;
        default :
            assert(false && "default case invoked");
            break;
        }
    }
    /*
    if ( sum != 0 ) {
        result_operands.push_back( expression_tree::make_exact_number(sum) );
    }
    */


    expression_tree::operands_t variables_res;

    std::for_each( sums_map.begin(), sums_map.end(), 
        [&env, &variables_res](const std::map<expression_tree, mpq_class>::value_type& val) {
            std::cout << "val : " << val.first.to_fullform(env) << " -> " << val.second << std::endl;
            std::cout << expression_tree::make_operator( 
                "Times", expression_tree::make_exact_number(val.second), val.first ).evaluate(env).to_fullform(env) << std::endl;
            if ( val.second != 0 ) {
                variables_res.push_back( expression_tree::make_operator( 
                    "Times", expression_tree::make_exact_number(val.second), val.first ).evaluate(env) ); //just to short it?
            }
        } 
    );

    if ( sum != 0 ) {
        variables_res.push_back( expression_tree::make_exact_number( sum ) );
    }

    if ( variables_res.size() == 1 ) {
        return variables_res[0];
    }

    return expression_tree::make_operator("Plus", variables_res);
#elif 0 //stable_partition
   mpq_class sum(0);
   std::map<expression_tree, mpq_class> sums_map;

    for ( unsigned i = 0; i < ops.size(); ++i ) {
        const expression_tree& current = ops[i];

        switch( current.get_type() ) {
        case expression_tree::EXACT_NUMBER :
            sum += current.get_exact_number();
            break;
        case expression_tree::SYMBOL :
            sums_map[ current ] += 1;
            break;
        case expression_tree::OPERATOR :
            assert( current.get_operator_name() != "Plus" );//Flat

            if ( current.get_operator_name() == "Times" ) {

                

                //TODO stable_partition is not the best here, if it is guarnteed to have only one EXACT_NUMBR

                expression_tree::operands_t curr_ops = current.get_operands();

                assert( curr_ops.size() > 1 );

                expression_tree::operands_t::iterator num_end = std::stable_partition( curr_ops.begin(), curr_ops.end(), 
                    [](const expression_tree& expr) {
                        return expr.get_type() == expression_tree::EXACT_NUMBER;
                    }
                );

                assert ( num_end - curr_ops.begin() <= 1 || "Warning : multiple numeric values in PLUS" );

                mpq_class multiplicator = 1;
                if ( num_end != curr_ops.begin() ) {
                    multiplicator = curr_ops.begin()->get_exact_number();
                }

                
                expression_tree::operands_t to_multiply_times_arglist( num_end, curr_ops.end() );
                expression_tree to_multiply;

                assert ( to_multiply_times_arglist.size() != 0 );

                if ( to_multiply_times_arglist.size() == 1 ) {
                    to_multiply = to_multiply_times_arglist[0];
                } else {
                    //doesn't need sort, since we used stable_partition
                    to_multiply = expression_tree::make_operator( "Times", to_multiply_times_arglist );
                }
                sums_map[ to_multiply ] += multiplicator;

            } else {
                sums_map[ current ] += 1;
            }
            break;


        default :
            assert(false && "default case invoked");
            break;
        }
    }

    std::cout << "sum " << sum << std::endl;
    for ( auto it = sums_map.begin(); it != sums_map.end(); ++it ) {
        std::cout << it->first.to_fullform(env) << " -> " << it->second << std::endl;
    }

    return expression_tree::make_operator( "Plus", ops );
#elif 1 //find_if
   mpq_class sum(0);
   std::map<expression_tree, mpq_class> sums_map;

    for ( unsigned i = 0; i < ops.size(); ++i ) {
        const expression_tree& current = ops[i];

        switch( current.get_type() ) {
        case expression_tree::EXACT_NUMBER :
            sum += current.get_exact_number();
            break;
        case expression_tree::SYMBOL :
            sums_map[ current ] += 1;
            break;
        case expression_tree::OPERATOR :
            assert( current.get_operator_name() != "Plus" );//Flat

            if ( current.get_operator_name() == "Times" ) {

                const expression_tree::operands_t& curr_ops = current.get_operands();

                assert( curr_ops.size() > 1 );

                expression_tree::operands_t::const_iterator number_it = std::find_if( curr_ops.begin(), curr_ops.end(), 
                    [](const expression_tree& expr) {
                        return expr.get_type() == expression_tree::EXACT_NUMBER;
                    }
                );

                mpq_class multiplicator = 1;
                if ( number_it != curr_ops.end() ) {
                    multiplicator = curr_ops.begin()->get_exact_number();
                }
                
                expression_tree::operands_t to_multiply_times_arglist;

                std::copy_if( curr_ops.begin(), curr_ops.end(), std::back_inserter(to_multiply_times_arglist), 
                    [](const expression_tree& expr) {
                        return expr.get_type() != expression_tree::EXACT_NUMBER;
                    }
                );
                
                expression_tree to_multiply;

                assert ( to_multiply_times_arglist.size() != 0 );

                if ( to_multiply_times_arglist.size() == 1 ) {
                    to_multiply = to_multiply_times_arglist[0];
                } else {
                    //doesn't need sort, since we didn't mixed up the order
                    to_multiply = expression_tree::make_operator( "Times", to_multiply_times_arglist );
                }
                sums_map[ to_multiply ] += multiplicator;

            } else {
                sums_map[ current ] += 1;
            }
            break;


        default :
            assert(false && "default case invoked");
            break;
        }
    }


    expression_tree::operands_t result_ops;


    for ( auto it = sums_map.begin(); it != sums_map.end(); ++it ) {

        if ( it->second == 0 ) {
            ;
        } else if ( it->second == 1 ) {
            result_ops.push_back(it->first);
        } else {
            
            expression_tree::operands_t part_ops(1, it->first);
            part_ops.push_back( expression_tree::make_exact_number( it->second ) );

            //std::sort( part_ops.begin(), part_ops.end() );

            //evaluate might cause problems. we call it to sort and possibly flatten the args
            result_ops.push_back(expression_tree::make_operator("Times", part_ops).evaluate(env));
        }

    }

    if ( sum != 0 ) {
        result_ops.push_back( expression_tree::make_exact_number(sum) );
    }

    if ( result_ops.size() == 0 ) {
        return expression_tree::make_exact_number( 0 );
    } else if ( result_ops.size() == 1 ) {
        return result_ops[0];
    } else {
        return expression_tree::make_operator( "Plus", result_ops );
    }

#else
    
    mpq_class sum(0);
    expression_tree::operands_t result_operands; //for variables etc.

    //for things like 3x+4x -> 7x
    //std::map<expression_tree, mpq_class> sums_map;

    for ( unsigned i = 0; i < ops.size(); ++i ) {
        const expression_tree& current = ops[i];

        switch( current.get_type() ) {
        case expression_tree::EXACT_NUMBER :
            sum += current.get_exact_number();
            break;
        case expression_tree::SYMBOL :
            result_operands.push_back(current);
            break;
        case expression_tree::OPERATOR :

            assert( current.get_operator_name() != "Plus" );//Flat

            if ( current.get_operator_name() == "Times" ) {
                result_operands.push_back(current); //?? TODO ??
            } else {
                result_operands.push_back(current);
            }
            break;
        default :
            assert(false && "default case invoked");
            break;
        }
    }
    
    if ( sum != 0 ) {
        result_operands.push_back( expression_tree::make_exact_number(sum) );
    }

    if (result_operands.size() == 0) {
        return expression_tree::make_exact_number( mpq_class(0) );
    } else if (result_operands.size() == 1) {
        return result_operands[0];
    } else {
        return expression_tree::make_operator("Plus", result_operands);
    }
#endif
}

expression_tree numeric_Plus(expression_tree::operands_t ops, enviroment& env) {
    mpf_class sum(0);
    expression_tree::operands_t result_operands; //for variables etc.

   

    for ( unsigned i = 0; i < ops.size(); ++i ) {
        const expression_tree& current = ops[i];

        switch( current.get_type() ) {
        case expression_tree::APPROXIMATE_NUMBER:
            sum += current.get_approximate_number();
            break;
        case expression_tree::EXACT_NUMBER :
            sum += current.get_exact_number();
            break;
        case expression_tree::SYMBOL :
            result_operands.push_back(current);
            break;
        case expression_tree::OPERATOR :
            if ( current.get_operator_name() == "Plus" ) {
                ops.insert( ops.end(), current.get_operands().begin(), current.get_operands().end() );
            } else if ( current.get_operator_name() == "Times" ) {
                result_operands.push_back(current); //?? TODO ??
            } else {
                result_operands.push_back(current);
            }
            break;
        default :
            assert(false && "default case invoked");
            break;
        }
    }
    
    if ( sum != 0 ) {
        result_operands.push_back( expression_tree::make_approximate_number(sum) );
    }

    if (result_operands.size() == 0) {
        return expression_tree::make_approximate_number( mpf_class(0.) );
    } else if (result_operands.size() == 1) {
        return result_operands[0];
    } else {
        return expression_tree::make_operator("Plus", result_operands);
    }
}

expression_tree non_numeric_only_numericq_plus(const expression_tree::operands_t& ops, enviroment& env) {

    assert(!ops.empty());

//    if ( ops.size() == 1 ) {
//        return ops[0];
//   }

    mpq_class sum(0);
    expression_tree::operands_t var_sum;

    for ( unsigned i = 0; i < ops.size(); ++i ) {

        const expression_tree& current = ops[i];
        switch( ops[i].get_type() ) {

        default :
            assert(false && "default case invoked");
            break;

        case expression_tree::EXACT_NUMBER :
            sum += ops[i].get_exact_number();
            break;

        case expression_tree::OPERATOR :
        case expression_tree::SYMBOL :
            var_sum.push_back( current );
            break;
        }
    }

    var_sum.push_back( expression_tree::make_exact_number( sum ) );
    if ( var_sum.size() == 1 ) {
        return var_sum[0];
    } else {
        return expression_tree::make_operator( "Plus", var_sum );
    }

}

} //namespace plus_impl

}} //namespace r0::builtin