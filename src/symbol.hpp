
#ifndef SYMBOL_HPP_
#define SYMBOL_HPP_

#include <string>
#include <functional>
#include <vector>

#include <boost/unordered_set.hpp>

#include "enviroment_fwd.hpp"
#include "expression_tree.hpp"

namespace r0  {

class symbol {
public:

    typedef boost::unordered_set<std::string> attributes_t;
    typedef expression_tree (*function_fptr_t)(const expression_tree::operands_t&, enviroment& env);
    typedef expression_tree (*numeric_function_fptr_t)(const expression_tree::operands_t&, enviroment& env);

    typedef mpf_class (*numeric_constant_fptr_t)(unsigned precision, enviroment& env);

    enum type_t {
        INVALID,
        FUNCTION, 
        CONSTANT, //Pi, E etc
        SYMBOL //user defined variables
        //maybe more?
    };

    symbol(); //creates INVALID

    static symbol make_invalid();
    
    static symbol make_function(const std::string& name, function_fptr_t ffptr, numeric_function_fptr_t numeric_ffptr);
    template<class Container>
    static symbol make_function(const std::string& name, function_fptr_t ffptr, numeric_function_fptr_t numeric_ffptr, const Container& attrs);

    static symbol make_variable(const std::string& name, const expression_tree& value); //attributes doesn't required

    static symbol make_constant(const std::string& name, numeric_constant_fptr_t ncfptr); //attributes doesn't required

    type_t get_type() const;
    const std::string& get_name() const;
    const attributes_t& get_attributes() const;

    bool has_attribute(const std::string& attr_name) const;

    expression_tree call_function(expression_tree::operands_t ops, enviroment& env) const;
    expression_tree call_numeric_function(expression_tree::operands_t ops, enviroment& env) const;

    const expression_tree& get_variable_value() const;

    mpf_class get_numeric_constant(unsigned precision, enviroment& env);

private:
    type_t type;

    std::string name;
    attributes_t attributes;

    function_fptr_t function_fptr;
    numeric_function_fptr_t numeric_function_fptr;
    numeric_constant_fptr_t numeric_constant_fptr; 
    expression_tree variable_value;
    
};
/*
//symbol functors for only symbol.get_name() comparsion and hashing for unordered_set
struct symbol_name_equal : std::binary_function<symbol, symbol, bool> {
    bool operator()(const symbol& x, const symbol& y) const;
};

struct symbol_hash : std::unary_function<symbol, std::size_t> {
    std::size_t operator()(const symbol& x) const;
};
*/
template<class Container>
symbol symbol::make_function(const std::string& n, function_fptr_t ffptr, numeric_function_fptr_t numeric_ffptr, const Container& attrs) {
    symbol res;
    res.type = FUNCTION;
    res.name = n;
    res.function_fptr = ffptr;
    res.numeric_function_fptr = numeric_ffptr;
    res.attributes = attributes_t(attrs.begin(), attrs.end());
    return res;
}


} //namespace r0

#endif //SYMBOL_HPP_