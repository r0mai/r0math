
#include "builtin_nestlist.hpp"
#include "enviroment.hpp"

namespace r0 { namespace builtin {

expression_tree NestList(const expression_tree::operands_t& ops, enviroment& env) {
    if ( ops.size() != 3 ) {
        env.raise_error("NestList", "called with invalid arguments");
        return expression_tree::make_operator("NestList", ops);
    }

    const expression_tree& func_as_expr = ops[0];
    const expression_tree& expr = ops[1];
    const expression_tree& n_as_expr = ops[2];

    if ( func_as_expr.get_type() != expression_tree::SYMBOL ||
        n_as_expr.get_type() != expression_tree::EXACT_NUMBER ||
        !is_mpq_integer(n_as_expr.get_exact_number()) ||
        n_as_expr.get_exact_number() < 0)
    {
        env.raise_error("NestList", "invalid argument format");
        return expression_tree::make_operator("NestList", ops);
    }

    const std::string& func_name = func_as_expr.get_symbol();

    unsigned long n = 0;
    if ( !mpz_get_ui_checked(n, n_as_expr.get_exact_number().get_num()) ) {
        env.raise_error("NestList", "overflow");
        return expression_tree::make_operator("NestList", ops);
    }

    expression_tree::operands_t result_list(n+1);
    result_list[0] = expr;
    for ( unsigned long i = 1; i <= n; ++i ) {
        result_list[i] = expression_tree::make_operator(func_name, result_list[i-1]).evaluate(env);
    }

    return expression_tree::make_operator("List", result_list); //doesn't need eval

}

}} //namespace r0::builtin