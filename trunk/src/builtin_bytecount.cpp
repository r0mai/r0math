
#include "builtin_bytecount.hpp"
#include "enviroment.hpp"
#include "expression_tree.hpp"

namespace r0 { namespace builtin {

expression_tree ByteCount(const expression_tree::operands_t& ops, enviroment& env) {
    if ( ops.size() != 1 ) {
        env.raise_error("ByteCount", "called with invalid arguments");
        return expression_tree::make_operator("ByteCount", ops);
    }
    return expression_tree::make_exact_number( mpq_class(ops[0].get_memory_size()) );
}

}} //namespace r0::builtin