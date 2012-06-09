
#ifndef BUILTIN_PRIMEQ_HPP_
#define BUILTIN_PRIMEQ_HPP_

#include "expression_tree.hpp"

namespace r0 { namespace builtin {

expression_tree Prime(const expression_tree::operands_t& ops, enviroment& env);
expression_tree PrimeQ(const expression_tree::operands_t& ops, enviroment& env);
expression_tree PrimePi(const expression_tree::operands_t& ops, enviroment& env);

namespace primeq_impl {








bool is_prime(const mpz_class& n);

mpz_class nth_prime(const mpz_class& n);

mpz_class primepi(const mpz_class& n);

//returns a factor, if the returned value is 1, then n is 1, if the returned value is n, then n is a prime !!!
mpz_class trial_division(const mpz_class& n, const mpz_class& starting_divisor = 2);

} //namespace primeq_impl

}} //namespace r0::builtin

#endif //BUILTIN_PRIMEQ_HPP_