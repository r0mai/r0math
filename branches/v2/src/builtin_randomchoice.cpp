
#include "builtin_randomchoice.hpp"

#include "enviroment.hpp"

#include <cstdlib>

namespace r0 { namespace builtin {

expression_tree RandomChoice(const expression_tree::operands_t& ops, enviroment& env) {
    if ( ops.size() != 1 ) {
        env.raise_error("RandomChoice", "called with invalid arguments");
        return expression_tree::make_operator("RandomChoice", ops);
    }

    if ( ops[0].get_type() != expression_tree::OPERATOR || ops[0].get_operator_name() != "List" || ops[0].get_operands().empty() ) {
        env.raise_error("RandomChoice", "non-empty list expected at position 1");
        return expression_tree::make_operator("RandomChoice", ops);
    }

    return ops[0].get_operands()[ std::rand() % ops[0].get_operands().size() ];
}

}} //namespace r0::builtin