
#ifndef BUILTIN_PLOT_HPP_
#define BUILTIN_PLOT_HPP_

#include <map>
#include <utility>

#include "expression_tree.hpp"

namespace r0 { namespace builtin {

expression_tree Plot(const expression_tree::operands_t& ops, enviroment& env);

namespace plot_impl {

struct iterator_spec_t {    
    std::string variable;
    mpf_class start;
    mpf_class end;    
};

bool validate_iterator(const expression_tree& it, iterator_spec_t& result);

//map : <x, <is_plottable, f(x)>>
typedef std::map<mpf_class, std::pair<bool, mpf_class>> plotted_points_t;

plotted_points_t plot_points( const expression_tree& func, const iterator_spec_t& iter, enviroment& env, unsigned graph_resolution);

} //namespace plot_impl

}} //namespace r0::builtin

#endif //BUILTIN_PLOT_HPP_