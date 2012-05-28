
#include "builtin_extendedgcd.hpp"
#include "enviroment.hpp"

namespace r0 { namespace builtin {

expression_tree ExtendedGCD(const expression_tree::operands_t& ops, enviroment& env) {
    if ( ops.size() == 0 ) {
        env.raise_error("ExtendedGCD", "called with invalid arguments");
        return expression_tree::make_operator("ExtendedGCD", ops);
    }

    if ( ops.size() == 1 ) {
        return ops[0];
    }

    for ( unsigned i = 0; i < ops.size(); ++i ) {
        if ( !ops[i].is_integer() ) {
            env.raise_error("ExtendedGCD", "integer arguments expected");
            return expression_tree::make_operator("ExtendedGCD", ops);
        }
    }

    std::vector<mpz_class> nums(ops.size());
    for ( unsigned i = 0; i < nums.size(); ++i ) {
        nums[i] = ops[i].get_exact_number();
    }

    std::vector<mpz_class> result(ops.size(), 0);
/*
    while( nums.size() > 1 ) {
        extendedgcd_impl::int_triplet xygcd = extendedgcd_impl::extended_gcd2(
            nums[nums.size() - 1],
            nums[nums.size() - 2]
        );
        result[i] = xygcd.get<0>();
        result[i+1] = xygcd.get<1>();
        //current_gcd = xygcd.get<2>();
    }
*/
    std::vector<expression_tree> result_expr(result.size());
    
    for ( unsigned i = 0; i < result_expr.size(); ++i ) {
        result_expr[i] = expression_tree::make_exact_number(result[i]);
    }
    return expression_tree::make_operator("List", result_expr);   
}

namespace extendedgcd_impl {

int_triplet extended_gcd2(const mpz_class& a, const mpz_class& b) { 

    int_pair x_lastx = int_pair(0, 1);
    int_pair y_lasty = int_pair(1, 0);
    int_pair ab = int_pair(a, b);

    while ( ab.second != 0 ) {
        mpz_class q = ab.first/ab.second;

        ab = int_pair( ab.second, ab.first % ab.second );

        x_lastx = int_pair( x_lastx.second - q*x_lastx.first, x_lastx.first );
        y_lasty = int_pair( y_lasty.second - q*y_lasty.first, y_lasty.first );

    }

    std::cout << ab.first << std::endl;

    return int_triplet(x_lastx.second, y_lasty.second, ab.first);

}

} //namespace extendedgcd_impl

}} //namespace r0::builtin