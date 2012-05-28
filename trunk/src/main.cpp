
#include "enviroment.hpp"
#include <iostream>

#include "util.hpp"

#include "builtin_det.hpp"
#include <algorithm>

int main() {

    //std::vector<int>

    //r0::builtin::det_impl::inversion_number();
    /*
    const int n = 5;
    
    std::vector<int> m(n);

    for ( unsigned i = 0; i < n; ++i ) {
        m[i] = i;
    }

    while ( std::next_permutation( m.begin(), m.end() ) ) {
        std::cout << "{" << m[0];
        for ( unsigned i = 1; i < n; ++i ) {
            std::cout << ", " << m[i];
        }
        std::cout << "}, " << r0::builtin::det_impl::inversion_number(m) << "\n";
    }
    */
    r0::enviroment e;
    return e.run();
}