
#include "builtin_sqrt.hpp"
#include "enviroment.hpp"

namespace r0 { namespace builtin {

expression_tree Sqrt(const expression_tree::operands_t& ops, enviroment& env) {
    if ( ops.size() != 1 ) {
        env.raise_error("Sqrt", "called with invalid arguments");
        return expression_tree::make_operator("Sqrt", ops);
    }
    return expression_tree::make_operator("Power", ops[0], expression_tree::make_exact_number( mpq_class(1, 2) ) ).evaluate(env);
}

}} //namespace r0::builtin