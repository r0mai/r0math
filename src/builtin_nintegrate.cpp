
#include "builtin_nintegrate.hpp"

#include "enviroment.hpp"
#include "list_structure_parser.hpp"

namespace r0 { namespace builtin {

expression_tree NIntegrate(const expression_tree::operands_t& ops, enviroment& env) {

    if ( ops.size() != 2 ) {
        env.raise_error("NIntegrate", "called with invalid arguments");
        return expression_tree::make_operator("NIntegrate", ops);
    }

    nintegrate_impl::iterator_spec_t iter;
    const bool iter_valid_success = nintegrate_impl::validate_iterator(ops[1], iter);

    if ( !iter_valid_success ) {
        env.raise_error("NIntegrate", "invalid iterator form");
        return expression_tree::make_operator("NIntegrate", ops);
    }

    const unsigned integration_resolution = 300;
    const mpf_class step = (iter.end - iter.start) / integration_resolution;

    std::vector<mpf_class> plotted_points;

    enviroment scoped_env(env);
    scoped_env.set_print_error_messages(false);

    for( unsigned i = 0; i <= integration_resolution; ++i ) {

        expression_tree main_expr = ops[0];

        const mpf_class current_x_point = iter.start + i*step;

        scoped_env.get_symbols().set_variable(iter.variable, expression_tree::make_approximate_number(current_x_point));

        main_expr.evaluate(scoped_env, true); //true false?, Plot has the same problems...

        if ( main_expr.get_type() != expression_tree::APPROXIMATE_NUMBER ) {
            env.raise_error("NIntegrate", "singularity detected");
            return expression_tree::make_operator("NIntegrate", ops);            
        }

        plotted_points.push_back( main_expr.get_approximate_number() );
    }

    mpf_class integration_res = 0;

    for( unsigned i = 1; i < plotted_points.size(); ++i ) {
        //(1/2)*(x2-x1)*(f[x1]+f[x2])
        const mpf_class x1 = (i-1)*step;
        const mpf_class x2 = i*step;

        const mpf_class& y1 = plotted_points[i-1];
        const mpf_class& y2 = plotted_points[i];

        integration_res += mpf_class(0.5) * (x2-x1) * (y1+y2);

        //std::cout << i*step << " -> " << plotted_points[i] << std::endl;
    }

    return expression_tree::make_approximate_number(integration_res);
}

namespace nintegrate_impl {

bool validate_iterator(const expression_tree& it, iterator_spec_t& result) {

    std::vector< expression_tree > parse_result(3);
    if ( !list_structure_parser("0,1,2", it, parse_result) ) {
        return false;
    }

    if ( parse_result[0].get_type() != expression_tree::SYMBOL ||
         parse_result[1].get_type() != expression_tree::EXACT_NUMBER ||
         parse_result[2].get_type() != expression_tree::EXACT_NUMBER) {
        return false;
    }

    result.variable = parse_result[0].get_symbol();
    result.start = parse_result[1].get_exact_number();
    result.end = parse_result[2].get_exact_number();

    if ( result.start > result.end ) {
        std::swap(result.start, result.end);
    }

    return true;
}

} //namespace nintegrate_impl

}} //namespace r0::builtin