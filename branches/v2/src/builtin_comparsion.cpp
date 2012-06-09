
#include "builtin_comparsion.hpp"

#include "builtin_numericq.hpp"
#include "enviroment.hpp"

namespace r0 { namespace builtin {


#define R0_BUILTIN_COMPARSION_FUNCTION(name, op) \
expression_tree name(const expression_tree::operands_t& ops, enviroment& env) { \
    if ( ops.size() != 2 ) { \
        env.raise_error(#name, "called with invalid arguments"); \
        return expression_tree::make_operator(#name, ops); \
    } \
    \
    if ( !numericq_impl::recursive_numericq(ops[0], env) || \
        !numericq_impl::recursive_numericq(ops[1], env) ) \
    { \
        return expression_tree::make_operator(#name, ops); \
    } \
    \
    expression_tree lhs = ops[0]; \
    expression_tree rhs = ops[1]; \
    \
    lhs.evaluate(env, true);  \
    rhs.evaluate(env, true); \
     \
    if ( !lhs.is_number_type() || !rhs.is_number_type() ) { \
        return expression_tree::make_operator(#name, ops); \
    }\
     \
    if ( lhs.get_number_as_mpf() op rhs.get_number_as_mpf() ) { \
        return expression_tree::make_symbol("True"); \
    } else { \
        return expression_tree::make_symbol("False"); \
    } \
}

R0_BUILTIN_COMPARSION_FUNCTION(Less, <)

R0_BUILTIN_COMPARSION_FUNCTION(Greater, >)

R0_BUILTIN_COMPARSION_FUNCTION(LessEqual, <=)

R0_BUILTIN_COMPARSION_FUNCTION(GreaterEqual, >=)


#undef R0_BUILTIN_COMPARSION_FUNCTION


}} //namespace r0::builtin