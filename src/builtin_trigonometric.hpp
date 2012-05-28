
#ifndef BUILTIN_TRIGONOMETRIC_HPP_
#define BUILTIN_TRIGONOMETRIC_HPP_

#include "expression_tree.hpp"

namespace r0 { namespace builtin {

expression_tree Sin(const expression_tree::operands_t& ops, enviroment& env);
expression_tree numeric_Sin(const expression_tree::operands_t& ops, enviroment& env);

expression_tree Cos(const expression_tree::operands_t& ops, enviroment& env);
expression_tree numeric_Cos(const expression_tree::operands_t& ops, enviroment& env);

expression_tree Tan(const expression_tree::operands_t& ops, enviroment& env);
expression_tree numeric_Tan(const expression_tree::operands_t& ops, enviroment& env);

expression_tree Cot(const expression_tree::operands_t& ops, enviroment& env);
expression_tree numeric_Cot(const expression_tree::operands_t& ops, enviroment& env);

expression_tree Sec(const expression_tree::operands_t& ops, enviroment& env);
expression_tree numeric_Sec(const expression_tree::operands_t& ops, enviroment& env);

expression_tree Csc(const expression_tree::operands_t& ops, enviroment& env);
expression_tree numeric_Csc(const expression_tree::operands_t& ops, enviroment& env);

expression_tree ArcSin(const expression_tree::operands_t& ops, enviroment& env);
expression_tree numeric_ArcSin(const expression_tree::operands_t& ops, enviroment& env);

expression_tree ArcCos(const expression_tree::operands_t& ops, enviroment& env);
expression_tree numeric_ArcCos(const expression_tree::operands_t& ops, enviroment& env);

expression_tree ArcTan(const expression_tree::operands_t& ops, enviroment& env);
expression_tree numeric_ArcTan(const expression_tree::operands_t& ops, enviroment& env);

expression_tree Sinh(const expression_tree::operands_t& ops, enviroment& env);
expression_tree numeric_Sinh(const expression_tree::operands_t& ops, enviroment& env);

expression_tree Cosh(const expression_tree::operands_t& ops, enviroment& env);
expression_tree numeric_Cosh(const expression_tree::operands_t& ops, enviroment& env);

expression_tree Tanh(const expression_tree::operands_t& ops, enviroment& env);
expression_tree numeric_Tanh(const expression_tree::operands_t& ops, enviroment& env);

expression_tree Coth(const expression_tree::operands_t& ops, enviroment& env);
expression_tree numeric_Coth(const expression_tree::operands_t& ops, enviroment& env);

expression_tree Sech(const expression_tree::operands_t& ops, enviroment& env);
expression_tree numeric_Sech(const expression_tree::operands_t& ops, enviroment& env);

expression_tree Csch(const expression_tree::operands_t& ops, enviroment& env);
expression_tree numeric_Csch(const expression_tree::operands_t& ops, enviroment& env);

}} //namespace r0::builtin

#endif //BUILTIN_TRIGONOMETRIC_HPP_