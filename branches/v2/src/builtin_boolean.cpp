
#include "builtin_boolean.hpp"

#include <algorithm>
#include <iterator>

#include "enviroment.hpp"

namespace r0 { namespace builtin {

expression_tree Not(const expression_tree::operands_t& ops, enviroment& env) {
    if ( ops.size() != 1 ) {
        env.raise_error("Not", "called with invalid arguments");
        return expression_tree::make_operator("Not", ops);
    }

    const expression_tree& expr = ops[0];

    switch( expr.get_type() ) {
    default:
        assert( false && "default case invoked" );
        return expression_tree::make_operator("Not", ops);

    case expression_tree::APPROXIMATE_NUMBER :
    case expression_tree::EXACT_NUMBER :
        return expression_tree::make_operator("Not", ops);

    case expression_tree::SYMBOL :
        {
            const std::string& symbol_name = expr.get_symbol();
            if ( symbol_name == "True" ) {
                return expression_tree::make_symbol("False");
            } else if ( symbol_name == "False" ) {
                return expression_tree::make_symbol("True");
            } else {
                return expression_tree::make_operator("Not", ops);
            }
        }
    case expression_tree::OPERATOR :
        {
            const std::string& operator_name = expr.get_operator_name();
            const expression_tree::operands_t& expr_ops = expr.get_operands();

            if ( operator_name == "Less" ) {
                return expression_tree::make_operator("GreaterEqual", expr_ops);
            } else if ( operator_name == "Greater" ) {
                return expression_tree::make_operator("LessEqual", expr_ops);
            } else if ( operator_name == "GreaterEqual" ) {
                return expression_tree::make_operator("Less", expr_ops);
            } else if ( operator_name == "LessEqual" ) {
                return expression_tree::make_operator("Greater", expr_ops);
            } else if ( operator_name == "Not" ) {
                if( expr_ops.size() == 1 ) {
                    return expr_ops[0];
                }
                return expression_tree::make_operator("Not", ops);
            } else {
                return expression_tree::make_operator("Not", ops);
            }
        }
    }
}

expression_tree And(const expression_tree::operands_t& ops_to_copy, enviroment& env) {

    expression_tree::operands_t ops(ops_to_copy);

    //lazy evaluation
    for ( unsigned i = 0; i < ops.size(); ++i ) {

        expression_tree& current = ops[i].evaluate(env);

        if ( current.get_type() == expression_tree::SYMBOL ) {
            if ( current.get_symbol() == "False" ) {
                return expression_tree::make_symbol("False");
            }
        }

    }

    expression_tree::operands_t result_ops;

    std::remove_copy_if(ops.begin(), ops.end(), std::back_inserter(result_ops), 
        [](const expression_tree& expr) {
            return expr.get_type() == expression_tree::SYMBOL && expr.get_symbol() == "True";
        }
    );

    if (result_ops.size() == 1) {
        return result_ops[0];
    } else {
        return expression_tree::make_operator("And", result_ops);
    }
}

expression_tree Or(const expression_tree::operands_t& ops_to_copy, enviroment& env) {

    expression_tree::operands_t ops(ops_to_copy);

    //lazy evaluation
    for ( unsigned i = 0; i < ops.size(); ++i ) {

        expression_tree& current = ops[i].evaluate(env);

        if ( current.get_type() == expression_tree::SYMBOL ) {
            if ( current.get_symbol() == "True" ) {
                return expression_tree::make_symbol("True");
            }
        }

    }

    expression_tree::operands_t result_ops;

    std::remove_copy_if(ops.begin(), ops.end(), std::back_inserter(result_ops), 
        [](const expression_tree& expr) {
            return expr.get_type() == expression_tree::SYMBOL && expr.get_symbol() == "False";
        }
    );

    if (result_ops.size() == 1) {
        return result_ops[0];
    } else {
        return expression_tree::make_operator("Or", result_ops);
    }
}

}} //namespace r0::builtin