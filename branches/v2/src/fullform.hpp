
#ifndef FULLFORM_HPP_
#define FULLFORM_HPP_

#include <string>
#include "expression_tree_fwd.hpp"
#include "enviroment_fwd.hpp"

namespace r0 {

std::string expression_tree_to_fullform(const expression_tree& expr, enviroment& env);

} //r0

#endif //FULLFORM_HPP_