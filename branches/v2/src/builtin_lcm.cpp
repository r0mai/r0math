
#include "builtin_lcm.hpp"

#include <algorithm>
#include <vector>

#include "mpirxx_include.hpp"
#include "util.hpp"
#include "expression_tree.hpp"
#include "enviroment.hpp"

namespace r0 { namespace builtin {

expression_tree LCM(const expression_tree::operands_t& ops, enviroment& env) {
    if ( ops.size() == 0 ) {
        env.raise_error("LCM", "called without arguments");
        return expression_tree::make_operator("LCM", ops);
    } /*else if ( ops.size() == 1 ) {
        return ops[0];
    }*/ else {
        bool all_integer = std::find_if( ops.begin(), ops.end(),
            [](const expression_tree& expr) {
                return expr.get_type() != expression_tree::EXACT_NUMBER || !is_mpq_integer(expr.get_exact_number());
            }
        ) == ops.end();
        if ( all_integer ) {
            std::vector<mpz_class> integers(ops.size());
            for ( unsigned i = 0; i < ops.size(); ++i ) {
                integers[i] = ops[i].get_exact_number().get_num();
            }
            return expression_tree::make_exact_number( mpq_class( integer_lcm( integers) ) );
        } else {
            return expression_tree::make_operator("LCM", ops);
        }
    }
}

}} //namespace r0::builtin