
#ifndef INPUTFORM_HPP_
#define INPUTFORM_HPP_

#include <string>

#include "expression_tree.hpp"
#include "enviroment_fwd.hpp"

namespace r0 {

std::string expression_tree_to_inputform(const expression_tree& expr, enviroment& env);

namespace inputform_impl {

std::string inputform_recurse(const expression_tree& expr, int prev_precedence);
std::string print_pretty_times(const expression_tree::operands_t& ops);

} //namespace inputform_impl

} //namespace r0

#endif //INPUTFORM_HPP_