
#include "expression_tree_util.hpp"

#include <algorithm>
#include <cassert>
#include <iterator>

namespace r0 {

mpq_class extract_exact_numeric_values( const expression_tree::operands_t& ops, const mpq_class& default_value ) {
    mpq_class result = default_value;

    for(unsigned i = 0; i < ops.size(); ++i) {
        if ( ops[i].get_type() == expression_tree::EXACT_NUMBER ) {
            result = ops[i].get_exact_number();

            //continue to assert
            for(i += 1; i < ops.size(); ++i) {
                assert ( ops[i].get_type() != expression_tree::EXACT_NUMBER );
            }
            break;
        }
    }
    return result;
}

expression_tree::operands_t remove_exact_numeric_values( const expression_tree::operands_t& ops ) {
    expression_tree::operands_t result;
    std::remove_copy_if( ops.begin(), ops.end(), std::back_insert_iterator<expression_tree::operands_t>(result), 
        [](const expression_tree& expr) {
            return expr.get_type() == expression_tree::EXACT_NUMBER;
        }
    );
    return result;
}

} //namespace r0