
#include "function_options.hpp"

#include <sstream>
#include "enviroment.hpp"

namespace r0 {

function_options::function_options() {}

void function_options::set_option( const std::string key, const expression_tree& value ) {
    options[key] = value;
}

expression_tree function_options::get_option( const std::string key ) {
    assert( options.find(key) != options.end() );
    return options[key];
}

expression_tree::operands_t function_options::get_options_from_argument_list( expression_tree::operands_t ops ) {

    
    int i = ops.size()-1;
    for( ; i >= 0 && ops[i].get_type() == expression_tree::OPERATOR && ops[i].get_operator_name() == "Rule";
        --i )
    {     


        //if size test fails, then there was probably an error somewhere before, just ignore it
        if ( ops[i].get_operands().size() == 2 && ops[i].get_operands()[0].get_type() == expression_tree::SYMBOL ) {
            options[ops[i].get_operands()[0].get_symbol()] = ops[i].get_operands()[1];
        }
    }
    ops.resize( i+1 );
    return ops;
}

bool function_options::has_option( const std::string& option_name ) const {
    return options.find(option_name) != options.end();
}

expression_tree::operands_t function_options::to_operands_t() const {

    expression_tree::operands_t res;

    std::for_each(options.begin(), options.end(), [&res](const options_t::value_type& o)
    {
        res.push_back( 
            expression_tree::make_operator("Rule", expression_tree::make_symbol(o.first), o.second)
            );
    } );

    return res;
}

std::string function_options::to_string() const {
    std::stringstream ss;
    enviroment env; //temporary

    std::for_each( options.begin(), options.end(), [&env, &ss](const options_t::value_type& val)
    {
        ss << val.first << " -> " << val.second.to_fullform(env) << '\n';

    } );
    return ss.str();
}

} //namespace r0