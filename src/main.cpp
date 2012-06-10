
#include "enviroment.hpp"
#include <iostream>

#include "util.hpp"

#include "polynomial.hpp"
#include <algorithm>
#include <boost/array.hpp>

int main() {

	boost::array<int, 3> coeff = {{1,2,3}};

	r0::single_variable_polynomial<int> p1( coeff );

	std::cout << p1 << std::endl;

    r0::enviroment e;
    return e.run();
}
