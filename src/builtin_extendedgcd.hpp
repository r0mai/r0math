
#ifndef BUILTIN_EXTENDEDGCD_HPP_
#define BUILTIN_EXTENDEDGCD_HPP_

#include "expression_tree.hpp"

#include <utility>
#include <boost/tuple/tuple.hpp>

namespace r0 { namespace builtin {

expression_tree ExtendedGCD(const expression_tree::operands_t& ops, enviroment& env);

namespace extendedgcd_impl {

typedef std::pair<mpz_class, mpz_class> int_pair;
typedef boost::tuple<mpz_class, mpz_class, mpz_class> int_triplet;

//(x,y,gcd(a,b)) where ax+by=gcd(a,b)
int_triplet extended_gcd2(const mpz_class& a, const mpz_class& b);

} //namespace extendedgcd_impl

}} //namespace r0::builtin

#endif //BUILTIN_EXTENDEDGCD_HPP_