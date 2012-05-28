
#ifndef CODEFORM_HPP_
#define CODEFORM_HPP_

#include "expression_tree.hpp"
#include "enviroment_fwd.hpp"

namespace r0 {

std::string expression_tree_to_codeform(const expression_tree& expr, enviroment& env);

namespace codeform_impl {

std::string codeform_recursive(const expression_tree& expr, enviroment& env, int tabbing);

} //namespace codeform_impl

} //namespace r0

#endif //CODEFORM_HPP_