
#include "builtin_part.hpp"

#include <cassert>

#include "enviroment.hpp"
#include "expression_tree.hpp"
#include "util.hpp"

namespace r0 { namespace builtin {

expression_tree Part(const expression_tree::operands_t& ops, enviroment& env) {

    if ( ops.size() == 0 || ops.size() > 2 ) {
        env.raise_error("Part", "called with invalid number of arguments, 1 or 2 expected");
        return expression_tree::make_operator( "Part" );
    }
    if ( ops.size() == 1 ) {
        return ops[0];
    } else { //ops.size() == 2

        long part = 0;
        bool part_spec_success = part_impl::extract_part(ops[1], part, env);

        if ( !part_spec_success ) {
            //part_impl::extract_part already wrote out an error message
            return expression_tree::make_operator( "Part", ops ); 
        }

        if ( part == 0 ) {
            switch ( ops[0].get_type() ) {
            default:
                assert( false && "default case invoked" );
                return expression_tree::make_invalid();
            case expression_tree::OPERATOR :
                return expression_tree::make_symbol( ops[0].get_operator_name() );
            case expression_tree::SYMBOL :
                return expression_tree::make_symbol( "Symbol" );
            case expression_tree::EXACT_NUMBER :
                return expression_tree::make_symbol( "Rational" );
            case expression_tree::APPROXIMATE_NUMBER :
                return expression_tree::make_symbol( "Real" );
            }
        } else if ( part > 0 ) {
            if ( ops[0].get_type() == expression_tree::OPERATOR && ops[0].get_operands().size() >= static_cast<unsigned>(part) ) {
                return ops[0].get_operands().at(part-1);
            } else {
                env.raise_error("Part", "index out of range (+)");
                return expression_tree::make_operator( "Part" );
            }
        } else { //part < 0
            if ( ops[0].get_type() == expression_tree::OPERATOR && ops[0].get_operands().size() >= static_cast<unsigned>(-part) ) {
                return ops[0].get_operands()[ops[0].get_operands().size() + part];
            } else {
                env.raise_error("Part", "index out of range (-)");
                return expression_tree::make_operator( "Part" );
            }
        }
    }
}

namespace part_impl {

bool extract_part( const expression_tree& expr, long& res, enviroment& env ) {

    if ( expr.get_type() != expression_tree::EXACT_NUMBER || !is_mpq_integer(expr.get_exact_number()) ) {
        env.raise_error("Part", "specification error");
        return false;
    }

    bool success = mpz_get_si_checked(res, expr.get_exact_number().get_num());

    if (!success) {
        env.raise_error("Part", "specification overflow");
        return false;
    }
    return true;
}

} //namespace part_impl

}} //namespace r0::builtin