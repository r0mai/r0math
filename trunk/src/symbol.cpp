
#include "symbol.hpp"

#include <cassert>

namespace r0  {

symbol::symbol() : type(INVALID) {}

symbol symbol::make_function(const std::string& n, function_fptr_t ffptr, numeric_function_fptr_t numeric_ffptr) {
    symbol res;
    res.type = FUNCTION;
    res.name = n;
    res.numeric_function_fptr = numeric_ffptr;
    res.function_fptr = ffptr;
    return res;
}

symbol symbol::make_variable(const std::string& n, const expression_tree& value) {
    symbol res;
    res.type = SYMBOL;
    res.name = n;
    res.variable_value = value;
    return res;
}

symbol symbol::make_constant(const std::string& n, numeric_constant_fptr_t ncfptr) {
    symbol res;
    res.type = CONSTANT;
    res.name = n;
    res.numeric_constant_fptr = ncfptr;
    return res;
}

symbol::type_t symbol::get_type() const {
    return type;
}

const std::string& symbol::get_name() const {
    return name;
}

const symbol::attributes_t& symbol::get_attributes() const {
    return attributes;
}

bool symbol::has_attribute(const std::string& attr_name) const {
    return attributes.find( attr_name ) != attributes.end();
}

expression_tree symbol::call_function(expression_tree::operands_t ops, enviroment& env) const {
    assert( type == FUNCTION && function_fptr != 0 );
    return function_fptr(ops, env);
}

expression_tree symbol::call_numeric_function(expression_tree::operands_t ops, enviroment& env) const {
    assert( type == FUNCTION );
    assert( has_attribute("NumericFunction") );
    assert( numeric_function_fptr != 0 );
    return numeric_function_fptr(ops, env);
}

const expression_tree& symbol::get_variable_value() const {
    assert( type == SYMBOL );
    return variable_value;
}

mpf_class symbol::get_numeric_constant(unsigned precision, enviroment& env) {
    assert( type == CONSTANT );
    assert( numeric_constant_fptr != 0 );
    return numeric_constant_fptr(precision, env);
}
/*
bool symbol_name_equal::operator()(const symbol& x, const symbol& y) const {
    return x.get_name() == y.get_name();
}

std::size_t symbol_hash::operator()(const symbol& x) const {
    return boost::hash_value( x.get_name() );
}

*/

} //namespace r0