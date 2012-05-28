
#ifndef BUILTIN_FINDROOT_HPP_
#define BUILTIN_FINDROOT_HPP_

#include "expression_tree.hpp"

namespace r0 { namespace builtin {

expression_tree FindRoot(const expression_tree::operands_t& ops, enviroment& env);

namespace findroot_impl {

bool execute_function(expression_tree expr, enviroment& scoped_env, const std::string& var, const mpf_class& x, mpf_class& result);

bool secant_method(
    const expression_tree& func, 
    enviroment& env,
    const std::string& variable, 
    mpf_class x1, 
    mpf_class x2, 
    mpf_class& result);

bool newton_method(
    const expression_tree& func, 
    enviroment& env,
    const std::string& variable, 
    mpf_class x,
    mpf_class& result);

} //findroot_impl

}} //namespace r0::builtin

#endif //BUILTIN_FINDROOT_HPP_