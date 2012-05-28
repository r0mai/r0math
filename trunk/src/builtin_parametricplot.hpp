
#ifndef BUILTIN_PARAMETRICPLOT_HPP_
#define BUILTIN_PARAMETRICPLOT_HPP_

#include "expression_tree.hpp"

#include "builtin_plot.hpp"//using iterator validation
#include <boost/tuple/tuple.hpp>

namespace r0 { namespace builtin {

expression_tree ParametricPlot(const expression_tree::operands_t& ops, enviroment& env);

namespace parametricplot_impl {

typedef std::map<mpf_class, boost::tuple<bool, mpf_class, mpf_class>> plotted_points_t;

plotted_points_t plot_points( 
    const expression_tree& funcx,
    const expression_tree& funcy,
    const plot_impl::iterator_spec_t& iter,
    enviroment& env,    
    unsigned graph_resolution);

} //parametricplot_impl

}} //namespace r0::builtin

#endif //BUILTIN_PARAMETRICPLOT_HPP_