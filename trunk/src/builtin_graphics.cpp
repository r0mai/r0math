
#include "builtin_graphics.hpp"

namespace r0 { namespace builtin {

expression_tree Graphics(const expression_tree::operands_t& ops_to_copy, enviroment& env) {
    expression_tree::operands_t ops = ops_to_copy;
    for(unsigned i = 0; i < ops.size(); ++i) {
        ops[i].evaluate(env, true);
    }
    return expression_tree::make_operator("Graphics", ops);
}

//Graphics primitives
expression_tree Line(const expression_tree::operands_t& ops, enviroment& env) {
    return expression_tree::make_operator("Line", ops);
}

expression_tree Circle(const expression_tree::operands_t& ops, enviroment& env) {
    return expression_tree::make_operator("Circle", ops);
}

expression_tree Point(const expression_tree::operands_t& ops, enviroment& env) {
    return expression_tree::make_operator("Point", ops);
}

expression_tree Disk(const expression_tree::operands_t& ops, enviroment& env) {
    return expression_tree::make_operator("Disk", ops);
}

expression_tree Rectangle(const expression_tree::operands_t& ops, enviroment& env) {
    return expression_tree::make_operator("Rectangle", ops);
}

expression_tree RGBColor(const expression_tree::operands_t& ops, enviroment& env) {
    return expression_tree::make_operator("RGBColor", ops);
}

expression_tree Hue(const expression_tree::operands_t& ops, enviroment& env) {
    return expression_tree::make_operator("Hue", ops);
}

}} //namespace r0::builtin