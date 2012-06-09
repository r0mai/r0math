
#include "inputform.hpp"

#include <sstream>
#include <cassert>

#include "util.hpp"
#include "expression_tree.hpp"

namespace r0 {

std::string expression_tree_to_inputform(const expression_tree& expr, enviroment& env) {
    return inputform_impl::inputform_recurse(expr, -1);
}

namespace inputform_impl {

std::string inputform_recurse(const expression_tree& expr, int prev_precedence) {
    std::stringstream ss;
    ss.precision(6);
    ss << std::fixed;

    switch(expr.get_type()) {
    default:
        assert(false && "default branch invoked");
        break;
    case expression_tree::INVALID :
        ss << "invalid";
        break;
    case expression_tree::EXACT_NUMBER : 
        {
            const mpq_class& ex_num = expr.get_exact_number();

            if ( is_mpq_integer(ex_num) || prev_precedence < 1 ) { //TODO probably good... recheck
                ss << ex_num;
            } else {            
                ss << '(' << ex_num << ')';
            }
        }
        break;
    case expression_tree::APPROXIMATE_NUMBER:
        ss << expr.get_approximate_number();
        break;
    case expression_tree::OPERATOR :
        {
            const std::string& symbol_name = expr.get_operator_name();
            const expression_tree::operands_t& operands = expr.get_operands();

            if ( symbol_name == "Plus" ) {
                if ( prev_precedence < 0 ) { 
                    ss << container_to_string(operands, [](const expression_tree& ex){ return inputform_recurse(ex, 0); }, "+");
                } else {
                    ss << '(' << container_to_string(operands, [](const expression_tree& ex){ return inputform_recurse(ex, 0); }, "+") << ')';
                }
            } else if ( symbol_name == "Times" ) {
                 if ( prev_precedence < 1 ) { 
                    ss << print_pretty_times(operands);
                } else {
                    ss << '(' << print_pretty_times(operands) << ')';
                }
            } else if ( symbol_name == "Power" ) {
                 if ( prev_precedence < 2 ) { 
                    ss << container_to_string(operands, [](const expression_tree& ex){ return inputform_recurse(ex, 2); }, "^");
                } else {
                    ss << '(' << container_to_string(operands, [](const expression_tree& ex){ return inputform_recurse(ex, 2); }, "^") << ')';
                }
            } else if ( symbol_name == "List" ) {
                ss << "{" << container_to_string(operands, [](const expression_tree& ex){ return inputform_recurse(ex, -1); }, ", ") << "}";
            } else {
                ss << symbol_name << "[" <<
                container_to_string(operands, [](const expression_tree& ex){ return inputform_recurse(ex, -1); }, ", ") << "]";
            }
        }
        break;
    case expression_tree::SYMBOL :
        ss << expr.get_symbol();
        break;
    }

    return ss.str();
}

std::string print_pretty_times(const expression_tree::operands_t& operands) {
    
    bool negative = false;

    //TODO implement
    return container_to_string(operands, 
        [](const expression_tree& ex) -> std::string
        { 
            //requries that operands is sorted, has more then one element, and EXACT_NUMBER is the smallest in its enum
            /*if ( ex.get_type() == expression_tree::EXACT_NUMBER && ex.get_exact_number() == -1 ) {
                return "-";
            } else {*/
                return inputform_recurse(ex, 1);
            //}
        }, "*");
}

} //inputform_impl

} //namespace r0