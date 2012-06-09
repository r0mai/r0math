
#ifndef BUILTIN_GRAPHICS_HPP_
#define BUILTIN_GRAPHICS_HPP_

#include "expression_tree.hpp"

namespace r0 { namespace builtin {

//Does nothing, returns itself
expression_tree Graphics(const expression_tree::operands_t& ops, enviroment& env);

//Graphics primitives
expression_tree Line(const expression_tree::operands_t& ops, enviroment& env);
expression_tree Circle(const expression_tree::operands_t& ops, enviroment& env);
expression_tree Point(const expression_tree::operands_t& ops, enviroment& env);
expression_tree Disk(const expression_tree::operands_t& ops, enviroment& env);
expression_tree Rectangle(const expression_tree::operands_t& ops, enviroment& env);

//Graphics directives
expression_tree RGBColor(const expression_tree::operands_t& ops, enviroment& env);
expression_tree Hue(const expression_tree::operands_t& ops, enviroment& env);

}} //namespace r0::builtin

#endif //BUILTIN_GRAPHICS_HPP_