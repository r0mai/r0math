
#ifndef BUILTIN_D_HPP_
#define BUILTIN_D_HPP_

#include <boost/unordered_map.hpp>
#include <string>

#include "expression_tree.hpp"

namespace r0 { namespace builtin {

expression_tree D(const expression_tree::operands_t& ops, enviroment& env);

namespace d_impl {

typedef expression_tree (*derive_function_t)(const expression_tree::operands_t& ops, const std::string& var, enviroment& env);
typedef boost::unordered_map<std::string, derive_function_t> derive_functions_t;

derive_functions_t build_derive_functions();

expression_tree derive(const expression_tree& expr, const std::string& var, enviroment& env);

expression_tree derive_Plus(const expression_tree::operands_t& ops, const std::string& var, enviroment& env);
expression_tree derive_Times(const expression_tree::operands_t& ops, const std::string& var, enviroment& env);
expression_tree derive_Power(const expression_tree::operands_t& ops, const std::string& var, enviroment& env);
expression_tree derive_Log(const expression_tree::operands_t& ops, const std::string& var, enviroment& env);
expression_tree derive_Sin(const expression_tree::operands_t& ops, const std::string& var, enviroment& env);
expression_tree derive_Cos(const expression_tree::operands_t& ops, const std::string& var, enviroment& env);
expression_tree derive_Tan(const expression_tree::operands_t& ops, const std::string& var, enviroment& env);
expression_tree derive_Cot(const expression_tree::operands_t& ops, const std::string& var, enviroment& env);
expression_tree derive_Sec(const expression_tree::operands_t& ops, const std::string& var, enviroment& env);
expression_tree derive_Csc(const expression_tree::operands_t& ops, const std::string& var, enviroment& env);
expression_tree derive_ArcSin(const expression_tree::operands_t& ops, const std::string& var, enviroment& env);
expression_tree derive_ArcCos(const expression_tree::operands_t& ops, const std::string& var, enviroment& env);
expression_tree derive_ArcTan(const expression_tree::operands_t& ops, const std::string& var, enviroment& env);
expression_tree derive_Sinh(const expression_tree::operands_t& ops, const std::string& var, enviroment& env);
expression_tree derive_Cosh(const expression_tree::operands_t& ops, const std::string& var, enviroment& env);
expression_tree derive_Tanh(const expression_tree::operands_t& ops, const std::string& var, enviroment& env);
expression_tree derive_Coth(const expression_tree::operands_t& ops, const std::string& var, enviroment& env);
expression_tree derive_Sech(const expression_tree::operands_t& ops, const std::string& var, enviroment& env);
expression_tree derive_Csch(const expression_tree::operands_t& ops, const std::string& var, enviroment& env);

} //namespace d_impl

}} //namespace r0::builtin

#endif //BUILTIN_D_HPP_
