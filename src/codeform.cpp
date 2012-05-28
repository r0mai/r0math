
#include "codeform.hpp"

#include <sstream>

#include "enviroment.hpp"

namespace r0 {

std::string expression_tree_to_codeform(const expression_tree& expr, enviroment& env) {
    return codeform_impl::codeform_recursive(expr, env, 0);
}

namespace codeform_impl {

std::string codeform_recursive(const expression_tree& expr, enviroment& env, int tabbing) {
    std::stringstream ss;

    const unsigned tab_size = 4;

    ss << std::string(tab_size*tabbing, ' ');

    switch(expr.get_type()) {
    default:
        assert(false && "default branch invoked");
        break;
    case expression_tree::INVALID :
        ss << "expression_tree::make_invalid()";
        break;
    case expression_tree::EXACT_NUMBER :
        ss << "expression_tree::make_exact_number( mpq_class(" << expr.get_exact_number().get_num() << ", " << expr.get_exact_number().get_den() << ") )";
        break;
    case expression_tree::APPROXIMATE_NUMBER:
        ss << "expression_tree::make_approximate_number( mpf_class(" << expr.get_approximate_number() << ") )";
        break;
    case expression_tree::SYMBOL :
        ss << "expression_tree::make_symbol( \"" << expr.get_symbol() << "\" )";
        break;
    case expression_tree::OPERATOR :
        {
            ss << "expression_tree::make_operator( \"" << expr.get_operator_name() << "\", ";

            expression_tree::operands_t ops = expr.get_operands();
            switch(ops.size()) {
            case 0:
                break;
            case 1:
                ss << "\n" << codeform_recursive(ops[0], env, tabbing+1);
                break;
            case 2:
                ss << "\n" << codeform_recursive(ops[0], env, tabbing+1) << ",\n";
                ss << codeform_recursive(ops[1], env, tabbing+1);
                break;
            default : // >2
                ss << "boost::assign::list_of";
                for (unsigned i = 0; i < ops.size(); ++i) {
                    ss << "(\n" << codeform_recursive(ops[i], env, tabbing+1) << ")";
                }
                break;
            }

            ss << "\n" << std::string(tab_size*tabbing, ' ') << ")";
        }
        break;
    }

    //ss << '\n';

    return ss.str();

}


} //namespace codeform_impl


} //namespace r0
