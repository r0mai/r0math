
#include "builtin_map.hpp"
#include "enviroment.hpp"

namespace r0 { namespace builtin {

expression_tree Map(const expression_tree::operands_t& ops, enviroment& env) {
    if ( ops.size() != 2 ) {
        env.raise_error("Map", "called with invalid arguments");
        return expression_tree::make_operator("Map", ops);
    }
    
    
    if ( ops[0].get_type() != expression_tree::SYMBOL ) {
        env.raise_error("Map", "symbol expected at position 1");
        return expression_tree::make_operator("Map", ops);
    }
    
    std::string function = ops[0].get_symbol();

    if ( ops[1].get_type() != expression_tree::OPERATOR ) {
        return ops[1];
    }

    expression_tree::operands_t args_to_map = ops[1].get_operands();
    for ( unsigned i = 0; i < args_to_map.size(); ++i ) {
        args_to_map[i] = expression_tree::make_operator( function, args_to_map[i] ).evaluate(env); 
    }

    return expression_tree::make_operator(ops[1].get_operator_name(), args_to_map);
}

}} //namespace r0::builtin