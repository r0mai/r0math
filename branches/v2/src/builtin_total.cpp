
#include "builtin_total.hpp"

#include "enviroment.hpp"
#include "expression_tree.hpp"

#include <cassert>

namespace r0 { namespace builtin {

expression_tree Total(const expression_tree::operands_t& ops, enviroment& env) {

    if ( ops.size() == 1 || ops.size() == 2 ) {
        if ( ops[0].get_type() != expression_tree::OPERATOR ) {
            env.raise_error("Total", "nonatomic expression expected at position 1");
            return expression_tree::make_operator( "Total", ops );
        }
    } else {
        env.raise_error("Total", "called with invalid number of arguments");
        return expression_tree::make_operator( "Total", ops );
    }

    if ( ops.size() == 1 ) {
        return total_impl::Total(ops[0].get_operands(), 1, 1, env);
    } else { //ops.size() == 2
        assert(ops.size() == 2);
        return total_impl::Total(ops[0].get_operands(), 1, 1, env); //TODO change this
    }
}

namespace total_impl {

//Total([head[1,2,3..],<{x}]) calls this with impl::Total([1,2,3], x.min, x.max)
expression_tree Total(const expression_tree::operands_t& ops, unsigned min_level, unsigned max_level, enviroment& env) {
    return expression_tree::make_operator( "Plus", ops ).evaluate(env);
}

}}} //namespace r0::builtin::total_impl