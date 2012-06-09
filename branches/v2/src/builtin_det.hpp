
#ifndef BUILTIN_DET_HPP_
#define BUILTIN_DET_HPP_

#include <utility>

#include "expression_tree.hpp"

namespace r0 { namespace builtin {

expression_tree Det(const expression_tree::operands_t& ops, enviroment& env);

namespace det_impl {

template<class Iterator>
unsigned inversion_number(Iterator begin, Iterator end) {

    unsigned result = 0;
    
    for ( Iterator it = begin; it != end; ++it ) {
        for ( Iterator jt = begin; jt != it; ++jt ) {
            if ( *jt > *it ) {
                ++result;
            }
        }
    }
    return result;
}

} //namespace det_impl

}} //namespace r0::builtin

#endif //BUILTIN_DET_HPP_