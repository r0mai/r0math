
#include "builtin_det.hpp"
#include "enviroment.hpp"
#include "matrix.hpp"
#include "util.hpp"

#include <algorithm>

namespace r0 { namespace builtin {

expression_tree Det(const expression_tree::operands_t& ops, enviroment& env) {

    if ( ops.size() != 1 || !ops[0].is_operator("List") ) {
        env.raise_error("Det", "1 list parameter expected");
        return expression_tree::make_operator("Det", ops);
    }

    unsigned n = ops[0].get_operands().size();
    if ( n == 0 ) {
        env.raise_error("Det", "n != 0");
        return expression_tree::make_operator("Det", ops);
    }

    matrix<expression_tree> m(n,n);

    for ( unsigned i = 0; i < n; ++i ) {
        if ( !ops[0].get_operands()[i].is_operator("List") || ops[0].get_operands()[i].get_operands().size() != n ) {
            env.raise_error("Det", "Sqaure matrix expected");
            return expression_tree::make_operator("Det", ops);
        } else {
            for ( unsigned j = 0; j < n; ++j ) {
                m(i,j) = ops[0].get_operands()[i].get_operands()[j];
            }
        }
    }

    std::vector<unsigned> perm = range_in<unsigned>(0, n);

    expression_tree::operands_t sum;

    do {
        const bool do_inversion = det_impl::inversion_number(perm.begin(), perm.end()) % 2 == 1;

        expression_tree::operands_t product(n);

        for ( unsigned i = 0; i < n; ++i ) {
            product[i] = m(i, perm[i]);
        }
        if ( do_inversion ) {
            product.push_back( expression_tree::make_exact_number(-1) );
        }

        sum.push_back( expression_tree::make_operator("Times", product) );
         
    } while ( std::next_permutation( perm.begin(), perm.end() ) );

    return expression_tree::make_operator("Plus", sum).evaluate(env);
}

}} //namespace r0::builtin
