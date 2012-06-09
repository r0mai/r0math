
#include "mpirxx_include.hpp"

#include <cassert>

namespace r0 {

mpz_class mpz_pow_ui_class(mpz_class base_class, unsigned long exponent) {

    mpz_class result_class;

    mpz_pow_ui( result_class.get_mpz_t(), base_class.get_mpz_t(), exponent );

    return result_class;
}

mpq_class mpq_pow_ui_class(mpq_class base, unsigned long exponent) {
    mpz_class result_numerator = mpz_pow_ui_class(base.get_num(), exponent);
    mpz_class result_denominator = mpz_pow_ui_class(base.get_den(), exponent);

    mpq_class result( result_numerator, result_denominator ); //don't have to call canonicalize, common factors cannot be produced
    return result;
}

mpf_class mpf_pow_ui_class(mpf_class base, unsigned long exponent) {
    mpf_class res;
    mpf_pow_ui(res.get_mpf_t(), base.get_mpf_t(), exponent);
    return res;
}

bool mpz_root_class(mpz_class& result_class, mpz_class base_class, unsigned long inverse_exponent) {
    mpz_t base;
    mpz_t result;

    mpz_init_set( base, base_class.get_mpz_t() );
    mpz_init( result );

    bool success = mpz_root( result, base, inverse_exponent ) != 0; //!= 0 -> silent warning C4800
    result_class = mpz_class( result );

    mpz_clear(base);
    mpz_clear(result);

    return success;
}

bool is_mpq_integer(  const mpq_class& n ) {
    return n.get_den() == 1;
}

mpz_class integer_gcd( const mpz_class& a, const mpz_class& b ) {
    mpz_class res;
    mpz_gcd( res.get_mpz_t(), a.get_mpz_t(), b.get_mpz_t() );
    return res;
}

mpz_class integer_gcd( std::vector<mpz_class> nums ) {

    assert(!nums.empty());

    if ( nums.size() == 1 ) {
        return abs(nums[0]);
    }

    while ( nums.size() > 1 ) {
        mpz_class a = nums.back();
        nums.pop_back();
        mpz_class b = nums.back();
        nums.pop_back();
        nums.push_back( integer_gcd(a, b) );
    }

    return nums[0];

}

mpz_class integer_lcm( const mpz_class& a, const mpz_class& b ) {
    mpz_class res;
    mpz_lcm( res.get_mpz_t(), a.get_mpz_t(), b.get_mpz_t() );
    return res;
}

mpz_class integer_lcm( std::vector<mpz_class> nums ) {

    assert(!nums.empty());

    if ( nums.size() == 1 ) {
        return abs(nums[0]);
    }

    while ( nums.size() > 1 ) {
        mpz_class a = nums.back();
        nums.pop_back();
        mpz_class b = nums.back();
        nums.pop_back();
        nums.push_back( integer_lcm(a, b) );
    }

    return nums[0];
}

bool mpz_get_ui_checked( unsigned long& result, const mpz_class& n ) {
    if ( n.fits_ulong_p() ) {
        result = n.get_ui();
        return true;
    }
    return false;
}

bool mpz_get_si_checked( long& result, const mpz_class& n ) {
    if ( n.fits_slong_p() ) {
        result = n.get_si();
        return true;
    }
    return false;
}

mpf_class mpf_natural_log_class( mpf_class z ) {
    // z > 0         
    // Log[z] ~= 2( Sum[( 1/(2n+1) )*((z-1)/(z+1))^(2n+1), {n, 0, inf}] )
    //
    const unsigned max_n = 1000;
    assert( z > 0 );

    mpf_class res(0);
    mpf_class one(1);

    for( unsigned n = 0; n <= max_n; ++n ) {
        
        //x = ((z-1)/(z+1))^(2n+1)
        mpf_class x( (z-1)/(z+1) );
        
        const unsigned long twonplus1 = 2*n+1;
        mpf_pow_ui(x.get_mpf_t(), x.get_mpf_t(), twonplus1);

        res += (one / twonplus1)*x;
    }
    return 2*res;

}

mpf_class mpf_exponentation_class( mpf_class x ) {
    //Exp[z] ~= Sum[x^n/n!,{n,0,inf}]

    const unsigned max_n = 1000;

    mpf_class res = 1 + x; //n=0,1
    mpz_class factorial = 1;    

    for( unsigned n = 2; n <= max_n; ++n ) {
        factorial *= n;
        res += mpf_pow_ui_class(x, n) / factorial;
    }

    return res;
}

} //namespace r0
