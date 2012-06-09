
#include "fullform.hpp"

#include <sstream>

#include "enviroment.hpp"
#include "expression_tree.hpp"
#include "util.hpp"

namespace r0 {

std::string expression_tree_to_fullform(const expression_tree& expr, enviroment& env) {
    std::stringstream ss;

    switch(expr.get_type()) {
    default:
        assert(false && "default branch invoked");
        break;
    case expression_tree::INVALID :
        ss << "invalid";
        break;
    case expression_tree::EXACT_NUMBER :
        ss << expr.get_exact_number();
        break;
    case expression_tree::APPROXIMATE_NUMBER:
        ss << expr.get_approximate_number();
        break;
    case expression_tree::OPERATOR :
        ss << expr.get_operator_name() << "[" << 
            container_to_string(expr.get_operands(), [&env](const expression_tree& expr)
        { 
            return expression_tree_to_fullform(expr, env);
        }, ", ")        
        << "]";
        break;
    case expression_tree::SYMBOL :
        ss << expr.get_symbol();
        break;
    }

    return ss.str();
}

} //namespace r0