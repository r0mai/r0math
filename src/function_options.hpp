
#ifndef FUNCTION_OPTIONS_HPP_
#define FUNCTION_OPTIONS_HPP_

#include <string>

#include <unordered_map>

#include "expression_tree.hpp"

namespace r0 {

class function_options {
public:
    function_options();

    void set_option( const std::string key, const expression_tree& value );

    //will fail if key is not in the container
    expression_tree get_option( const std::string key );

    //removes the Rule[]s from the end of the argument list,
    //puts them into the container, then returns with the truncated argument list
    //Those roles, whihc doesn't have a SYMBOL on the right side, will be excluded, but still truncated
    expression_tree::operands_t get_options_from_argument_list( expression_tree::operands_t ops );

    bool has_option( const std::string& option_name ) const;

    //can be appended after the default ops
    expression_tree::operands_t to_operands_t() const;

    std::string to_string() const;
private:

    typedef std::unordered_map<std::string, expression_tree> options_t;
    options_t options;
};

} //namespace r0

#endif //FUNCTION_OPTIONS_HPP_