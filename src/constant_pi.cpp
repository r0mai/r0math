
#include "constant_pi.hpp"
#include "enviroment.hpp"

#include <boost/math/constants/constants.hpp>

namespace r0 { namespace constants {

mpf_class Pi(unsigned precision, enviroment& env) {
/*
    precision = 3000;

    //1/Pi==12Sum[((-1)^k (6k)!(13591409+545140134k))/((3k)! (k!)^3 640320^(3k+3/2)),{k,0,Infinity}]

    // k6_fact = (6k)!
    // a = 545140134k
    // k3_fact = (3k)!
    // k_fact = k!
    // b = 640320^(3k)
    // c = 640320^(3/2) = 5122560*10005^(1/2) //constant
    // f(n) = (k6_fact * (13591409 + a)) / (k3_fact * k_fact^3 * b * c)

    mpf_class sum(0, precision);

    //(-1)^k
    bool sign_negative = false;

    //(6k)!
    mpz_class k6_fact = 1;
    mpz_class k3_fact = 1;
    mpz_class k_fact = 1;
    mpz_class a = 0;
    mpz_class b = 1;

    mpf_class c(0, precision);
    mpf_sqrt_ui(c.get_mpf_t(), 10005);
    c *= 5122560;


    const unsigned max_iter = 10;
    for (unsigned k = 0; k < max_iter; ++k) {

        mpz_class six_k = 6*k;
        k6_fact *= ((six_k+1) * (six_k+2) * (six_k+3) * (six_k+4) * (six_k+5) * (six_k+6));

        mpz_class three_k = 3*k;
        k3_fact *= ((three_k+1) * (three_k+2) * (three_k+3));

        if ( k > 0 ) {
            k_fact *= k;
        }

        a += 545140134;

        mpz_pow_ui(b.get_mpz_t(), b.get_mpz_t(), 3*k);
        mpz_class k_fact_cube = k_fact * k_fact * k_fact;

        // f(n) = (k6_fact * (13591409 + a)) / (k3_fact * k_fact^3 * b * c)
        std::cout << (k6_fact * (13591409 + a)) / (k3_fact * k_fact_cube * b * c) << std::endl;
    }
*/
    return boost::math::constants::pi<double>();
}

}} //namespace r0::constant