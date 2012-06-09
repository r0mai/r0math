
#include "builtin_set.hpp"
#include "enviroment.hpp"
#include "expression_tree.hpp"

namespace r0 { namespace builtin {

expression_tree Set(const expression_tree::operands_t& ops, enviroment& env) {
    if ( ops.size() != 2 ) {
        env.raise_error("Set", "called with invalid arguments");
        return expression_tree::make_operator("Set", ops);
    }
    if ( ops[0].get_type() != expression_tree::SYMBOL ) {
        env.raise_error("Set", "symbol expected at position 1");
        return expression_tree::make_operator("Set", ops);       
    }
    env.get_symbols().set_variable( ops[0].get_symbol(), ops[1] );
    return ops[1];
}

}} //namespace r0::builtin