
#include "builtin_n.hpp"
#include "enviroment.hpp"

#include <cassert>

namespace r0 { namespace builtin {

expression_tree N(const expression_tree::operands_t& ops, enviroment& env) {

    if ( ops.size() != 1 ) {
        env.raise_error("N", "called with invalid arguments");
        return expression_tree::make_operator("N", ops);
    }

    //ugly workaround for cases like N[2/3]
    if (ops[0].get_type() == expression_tree::EXACT_NUMBER) {
        return expression_tree::make_approximate_number(ops[0].get_exact_number());
    }

    expression_tree res = ops[0];
    res.evaluate(env, true); //numeric eval
    return res;

}

}} //namespace r0::builtin