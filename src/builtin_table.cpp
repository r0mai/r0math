
#include "builtin_table.hpp"

#include "enviroment.hpp"
#include "expression_tree.hpp"

namespace r0 { namespace builtin {

expression_tree Table(const expression_tree::operands_t& ops, enviroment& env) {
    if ( ops.size() != 2 ) {
        env.raise_error("Table", "called with invalid arguments");
        return expression_tree::make_operator("Table", ops);
    }

    table_impl::iterator_spec_t iter;
    bool iter_valid_success = table_impl::validate_iterator(ops[1], iter);

    if ( !iter_valid_success ) {
        env.raise_error("Table", "invalid iterator form");
        return expression_tree::make_operator("Table", ops);
    }

    expression_tree::operands_t result_list;

    if ( iter.variable == "" ) {
        while( iter.start <= iter.end ) {

            expression_tree main_expr = ops[0];
            main_expr.evaluate(env); //buggy is_numeric?? seems not...
            result_list.push_back(main_expr);

            iter.start += iter.step;
        }
    } else { //have a running variable

        enviroment scoped_env(env);

        while( iter.start <= iter.end ) {

            expression_tree main_expr = ops[0];
            scoped_env.get_symbols().set_variable(iter.variable, expression_tree::make_exact_number(iter.start));
            main_expr.evaluate(scoped_env); //buggy is_numeric?? seems not...
            result_list.push_back(main_expr);

            iter.start += iter.step;
        }
    }

    return expression_tree::make_operator("List", result_list);

}

namespace table_impl {

bool validate_iterator(const expression_tree& it, iterator_spec_t& result) {
    if ( it.get_type() != expression_tree::OPERATOR || it.get_operator_name() != "List" ) {
        return false;
    }
    const unsigned opsize = it.get_operands().size();
    const expression_tree::operands_t& ops = it.get_operands();

    if ( opsize == 0 ) {
        return false;
    }

    if ( opsize == 1 ) {
        if ( ops[0].get_type() != expression_tree::EXACT_NUMBER ) {
            return false;
        }
        
        result.variable = "";
        result.start = 1;
        result.end = ops[0].get_exact_number();
        result.step = 1;

        return true;
    }
    if ( opsize == 2 ) {
        if ( ops[0].get_type() != expression_tree::SYMBOL ||
            ops[1].get_type() != expression_tree::EXACT_NUMBER ) {
            return false;
        }
        
        result.variable = ops[0].get_symbol();
        result.start = 1;
        result.end = ops[1].get_exact_number();
        result.step = 1;

        return true;
    }
    if ( opsize == 3 ) {
        if ( ops[0].get_type() != expression_tree::SYMBOL ||
            ops[1].get_type() != expression_tree::EXACT_NUMBER ||
            ops[2].get_type() != expression_tree::EXACT_NUMBER ) {
            return false;
        }
        
        result.variable = ops[0].get_symbol();
        result.start = ops[1].get_exact_number();
        result.end = ops[2].get_exact_number();
        result.step = 1;

        return true;
    }
    if ( opsize == 4 ) {
        if ( ops[0].get_type() != expression_tree::SYMBOL ||
            ops[1].get_type() != expression_tree::EXACT_NUMBER ||
            ops[2].get_type() != expression_tree::EXACT_NUMBER ||
            ops[3].get_type() != expression_tree::EXACT_NUMBER) {
            return false;
        }

        result.variable = ops[0].get_symbol();
        result.start = ops[1].get_exact_number();
        result.end = ops[2].get_exact_number();
        result.step = ops[3].get_exact_number();

        return true;
    }
    return false;
}

} //namespace table_impl

}} //namespace r0::builtin