
#ifndef MPIRXX_INCLUDE_HPP_
#define MPIRXX_INCLUDE_HPP_

#ifdef _WIN32

#pragma warning(push)
#pragma warning(disable : 4800)

#include <mpirxx.h>

#pragma warning(pop)

#else

#include <gmpxx.h>

#endif

#include <vector>

namespace r0 {
   
//TODO simplify (or delete) these
mpz_class mpz_pow_ui_class(mpz_class base, unsigned long exponent);
mpq_class mpq_pow_ui_class(mpq_class base, unsigned long exponent);
mpf_class mpf_pow_ui_class(mpf_class base, unsigned long exponent);

//returns true on success, false on truncation
bool mpz_root_class(mpz_class& result, mpz_class base, unsigned long inverse_exponent); //base^(1/inverse_exponent)

bool is_mpq_integer( const mpq_class& n );

mpz_class integer_gcd( const mpz_class& a, const mpz_class& b );
mpz_class integer_gcd( std::vector<mpz_class> nums );

mpz_class integer_lcm( const mpz_class& a, const mpz_class& b );
mpz_class integer_lcm( std::vector<mpz_class> nums );

bool mpz_get_ui_checked( unsigned long& result, const mpz_class& n );
bool mpz_get_si_checked( long& result, const mpz_class& n );

mpf_class mpf_natural_log_class( mpf_class z );
mpf_class mpf_exponentation_class( mpf_class z ); //E^z


} //namespace r0

#endif //MPIRXX_INCLUDE_HPP_
