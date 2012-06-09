
#include "constant_e.hpp"
#include "enviroment.hpp"

#include <boost/math/constants/constants.hpp>

namespace r0 { namespace constants {

mpf_class E(unsigned precision, enviroment& env) {
    return boost::math::constants::e<double>();
}

}} //namespace r0::constant