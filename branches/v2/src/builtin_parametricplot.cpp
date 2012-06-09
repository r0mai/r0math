
#include "builtin_parametricplot.hpp"

#include <boost/assign/list_of.hpp>

#include "function_options.hpp"
#include "enviroment.hpp"

namespace r0 { namespace builtin {
#if 0
expression_tree ParametricPlot(const expression_tree::operands_t& ops_to_copy, enviroment& env) {

    function_options funcops;

    //default values
    funcops.set_option("Axes", expression_tree::make_symbol("True"));
    funcops.set_option("Mesh", expression_tree::make_symbol("None"));
    funcops.set_option("PlotPoints", expression_tree::make_exact_number(300));

    //end default values
    expression_tree::operands_t ops = funcops.get_options_from_argument_list(ops_to_copy);

    const bool mesh_all = funcops.get_option("Mesh").get_type() == expression_tree::SYMBOL && funcops.get_option("Mesh").get_symbol() == "All";

    
    unsigned long plot_resulotion = 300;
    const expression_tree& plot_points_expr = funcops.get_option( "PlotPoints" );
    if ( plot_points_expr.get_type() == expression_tree::EXACT_NUMBER && is_mpq_integer( plot_points_expr.get_exact_number() ) ) {
        if (!mpz_get_ui_checked(plot_resulotion, plot_points_expr.get_exact_number().get_num() ) ) {
            env.raise_error("ParametricPlot", "PlotPoints overflow");
            return expression_tree::make_operator("ParametricPlot", ops);
        }
    } else {
        env.raise_error("ParametricPlot", "option PlotPoints format error");
        return expression_tree::make_operator("ParametricPlot", ops);
    }

    if ( ops.size() != 2 ) {
        env.raise_error("ParametricPlot", "called with invalid arguments");
        return expression_tree::make_operator("ParametricPlot", ops);
    }

    ops[0].evaluate(env); //OVERRIDES HoldFirst ??!?
    ops[1].evaluate(env, true);

    plot_impl::iterator_spec_t iter;
    bool iter_valid_success = plot_impl::validate_iterator(ops[1], iter);

    if ( !iter_valid_success ) {
        env.raise_error("ParametricPlot", "invalid iterator form");
        return expression_tree::make_operator("ParametricPlot", ops);
    }

    std::vector< std::pair<expression_tree, expression_tree> > function_pairs;

    { //function extraction
        if ( !ops[0].is_operator("List") ) {
            env.raise_error("ParametricPlot", "invalid function form");
            return expression_tree::make_operator("ParametricPlot", ops);
        }        

        const expression_tree::operands_t& function_ops = ops[0].get_operands();

        const bool is_all_list = std::find_if( function_ops.begin(), function_ops.end(), 
            [](const expression_tree& expr){
                return !expr.is_operator("List");
            }
        ) != function_ops.end();

        if ( is_all_list ) {

        }

    }

    std::vector<parametricplot_impl::plotted_points_t> plotted_functions;

    if ( ops[0].get_type() == expression_tree::OPERATOR && ops[0].get_operator_name() == "List" ) {
        for (unsigned i = 0; i < ops[0].get_operands().size(); ++i) {
            plotted_functions.push_back( parametricplot_impl::plot_points(ops[0].get_operands()[i], iter, env, plot_resulotion) );
        }
    } else {
        plotted_functions.push_back( parametricplot_impl::plot_points(ops[0], iter, env, plot_resulotion) );
    }


    //{Line[f1], Line[f2], ...}
    expression_tree::operands_t graph_arg_list;

    double hue = 0.67;
    double hue_difference = 0.236068;

    for ( unsigned i = 0; i < plotted_functions.size(); ++i ) {

        expression_tree::operands_t line_list;
        expression_tree::operands_t mesh_point_list;

        plot_impl::plotted_points_t current_plotted_points = plotted_functions[i];        

        plot_impl::plotted_points_t::iterator begin = current_plotted_points.begin();
        plot_impl::plotted_points_t::iterator end = current_plotted_points.end();
        --end;

        for ( plot_impl::plotted_points_t::iterator it = current_plotted_points.begin(); it != end; /**/) {

            const plot_impl::plotted_points_t::value_type left_point = *it;
            const plot_impl::plotted_points_t::value_type right_point = *(++it); //implicit increment

            if ( left_point.second.first && right_point.second.first ) { //if we got a valid line
                line_list.push_back( expression_tree::make_operator(
                    "List",
                    expression_tree::make_approximate_number(left_point.first),
                    expression_tree::make_approximate_number(left_point.second.second) 
                ) );
            }

            if ( mesh_all ) { //draw mesh
                if ( left_point.second.first ) {
                    mesh_point_list.push_back( expression_tree::make_operator( 
                        "List",
                        expression_tree::make_approximate_number(left_point.first),
                        expression_tree::make_approximate_number(left_point.second.second) ) );
                }
            }
        }   
        
        //Graphics[{Line[{{x1,y1},{x2,y2}...}]}]
        
        

        expression_tree color_def = expression_tree::make_operator( "Hue", boost::assign::list_of(
            expression_tree::make_approximate_number( hue ))(
            expression_tree::make_approximate_number( mpf_class(0.6) ))(
            expression_tree::make_approximate_number( mpf_class(0.6) ))
        );

        hue += hue_difference;
        if ( hue > 1.0 ) {
            hue -= 1.0;
        }

        graph_arg_list.push_back( color_def/*.evaluate(env)*/ );
        graph_arg_list.push_back( expression_tree::make_operator( "Line", expression_tree::make_operator("List", line_list) ) );
        if (mesh_all) {
            graph_arg_list.push_back( expression_tree::make_symbol( "Black" ).evaluate(env) );
            graph_arg_list.push_back( expression_tree::make_operator( "Point", expression_tree::make_operator("List", mesh_point_list) ) );
        }

    }

    //if no explicit PlotRange given, then adjusting it to the plotted points
    if ( !funcops.has_option("PlotRange") ) {
        mpf_class x_min = iter.start;
        mpf_class x_max = iter.end;
        
        mpf_class y_min = iter.start;
        mpf_class y_max = iter.end;

        funcops.set_option("PlotRange", //{{x_min,x_max},{y_min,y_max}}
        expression_tree::make_operator("List",
            expression_tree::make_operator("List",
                expression_tree::make_approximate_number(x_min),
                expression_tree::make_approximate_number(x_max)),
            expression_tree::make_operator("List",
                expression_tree::make_approximate_number(y_min),
                expression_tree::make_approximate_number(y_max))
            )
        );
    }

    expression_tree::operands_t resultops = expression_tree::operands_t(1,
        expression_tree::make_operator("List",  graph_arg_list)
    );
    

    //appends to the back of the graphics object
    const expression_tree::operands_t options = funcops.to_operands_t();

    resultops.insert(resultops.end(), options.begin(), options.end()); 

    return expression_tree::make_operator("Graphics", resultops);

}

namespace parametricplot_impl {

plotted_points_t plot_points( 
    const expression_tree& funcx,
    const expression_tree& funcy,
    const plot_impl::iterator_spec_t& iter,
    enviroment& env,    
    unsigned graph_resolution) 
{

    plotted_points_t plotted_points;

    //const unsigned graph_resolution = 100;
    const mpf_class step = (iter.end - iter.start) / graph_resolution;

    enviroment scoped_env(env);
    scoped_env.set_print_error_messages(false);
    for( unsigned i = 0; i <= graph_resolution; ++i ) {

        const mpf_class current_t_point = iter.start + i*step;

        expression_tree main_expr_x = funcx;
        expression_tree main_expr_y = funcy;

        //workaround in case of constant function :(
        /*if (main_expr.get_type() == expression_tree::EXACT_NUMBER ||
            main_expr.get_type() == expression_tree::APPROXIMATE_NUMBER) {

            const mpf_class current_y_point = main_expr.get_number_as_mpf();
            plotted_points[current_x_point] = std::make_pair(true, current_y_point);
        } else */{
        
            scoped_env.get_symbols().set_variable(iter.variable, expression_tree::make_approximate_number(current_t_point));
            //what if main_expr_x/y changes t?
            main_expr_x.evaluate(scoped_env); //we need numeric evaulation in case f is an exact_number (workaround above)
            main_expr_y.evaluate(scoped_env); //we need numeric evaulation in case f is an exact_number (workaround above)
            

            //good debug place, this shouldn't happen (if func is EXACT_NUMBER , then this returns an exact number -> workaround above)
            //assert(main_expr.get_type() != expression_tree::EXACT_NUMBER);

            if ( main_expr_x.get_type() == expression_tree::APPROXIMATE_NUMBER &&
                main_expr_y.get_type() == expression_tree::APPROXIMATE_NUMBER)
            {
                const mpf_class current_x_point = main_expr_x.get_approximate_number();
                const mpf_class current_y_point = main_expr_y.get_approximate_number();
                plotted_points[current_t_point] = boost::make_tuple(true, current_x_point, current_y_point);
            } else {
                plotted_points[current_t_point] = boost::make_tuple(false, mpf_class(0), mpf_class(0));
            }
        }
    }

    return plotted_points;
}

} //parametricplot_impl
#endif
}} //namespace r0::builtin