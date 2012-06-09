
#include "builtin_holdform.hpp"

#include "enviroment.hpp"
#include "expression_tree.hpp"

namespace r0 { namespace builtin {

expression_tree HoldForm(const expression_tree::operands_t& ops, enviroment& env) {
    if ( ops.size() > 1 ) {
        env.raise_error("HoldForm", "called with invalid arguments");
        return expression_tree::make_operator("HoldForm", ops);
    }
    //no evaluation
    return expression_tree::make_operator( "HoldForm", ops );
}

}} //namespace r0::builtin