
#include "builtin_numericq.hpp"

#include "enviroment.hpp"

#include <cassert>
#include <algorithm>

namespace r0 { namespace builtin {

expression_tree NumericQ(const expression_tree::operands_t& ops, enviroment& env) {
    if ( ops.size() != 1 ) {
        env.raise_error( "NumericQ", "called with invalid arguments" );
        return expression_tree::make_operator( "NumericQ", ops );
    }

    const bool result = numericq_impl::recursive_numericq( ops[0], env );

    if ( result ) {
        return expression_tree::make_symbol( "True" );
    } else {
        return expression_tree::make_symbol( "False" );
    }
}

namespace numericq_impl {

bool recursive_numericq(const expression_tree& expr, enviroment& env) {
    switch (expr.get_type()) {
    default:
        assert( false && "default case invoked" );
        return false; //silent warning
    case expression_tree::EXACT_NUMBER :
    case expression_tree::APPROXIMATE_NUMBER :
        return true;
    case expression_tree::SYMBOL :
        {
            symbol sym;
            if ( !env.get_symbols().get_symbol( expr.get_symbol(), sym ) ) {
                return false;
            }

            switch( sym.get_type() ) {
            default:
                assert( false && "default case invoked" );
                return false; //silent warning
            case symbol::CONSTANT :
                return true;
            case symbol::FUNCTION :
                return false;
            case symbol::SYMBOL :
                return recursive_numericq( sym.get_variable_value(), env );
            }
        }
    case expression_tree::OPERATOR :
        {
            symbol sym;
            if ( !env.get_symbols().get_symbol( expr.get_operator_name(), sym ) ) {
                return false;
            }
            if ( sym.get_type() != symbol::FUNCTION ) {
                return false;
            }

            const bool numeric_func = sym.has_attribute("NumericFunction");
            if ( !numeric_func ) {
                return false;
            } else {
                const bool numeric_arguments = std::find_if( expr.get_operands().begin(), expr.get_operands().end(), 
                    [&env](const expression_tree& x) {
                        return !recursive_numericq(x, env);
                    }
                ) == expr.get_operands().end();

                return numeric_arguments;
            }

        }
    }
}

} //namespace numericq_impl

}} //namespace r0::builtin