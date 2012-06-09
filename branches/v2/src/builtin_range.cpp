
#include "builtin_range.hpp"

#include <algorithm>

#include "enviroment.hpp"

namespace r0 { namespace builtin {


#if 0
expression_tree Range(const expression_tree::operands_t& ops, enviroment& env) {

    if ( ops.size() > 3 || ops.empty() ) {
        env.raise_error("Range", "called with invalid arguments");
        return expression_tree::make_operator("Range", ops);
    }

    const bool is_all_numeric = std::find_if(ops.begin(), ops.end(),
        [](const expression_tree& expr) {
            return !expr.is_number_type();
        }
    ) == ops.end();

    if ( !is_all_numeric ) {
        env.raise_error("Range", "numeric arguments expected");
        return expression_tree::make_operator("Range", ops);
    }

    const bool is_exact_eval = std::find_if(ops.begin(), ops.end(), 
        [](const expression_tree& expr) {
            return expr.get_type() == expression_tree::APPROXIMATE_NUMBER;
        }
    ) == ops.end();

    if ( is_exact_eval ) {

    } else {

    }

    mpf_class num_min = 1;
    mpf_class num_max;
    mpf_class num_step = 1;

    mpq_class exact_min = 1;
    mpq_class exact_max;
    mpq_class exact_step = 1;

    switch( ops.size() ) {
    default :

    case 3 :
        switch( ops[2].get_type() ) {
        case expression_tree::EXACT_NUMBER :
            exact_step = ops[2]
        }
    case 2:
    case 1 :

    }
}
#endif
}} //namespace r0::builtin