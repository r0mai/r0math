
#include "parser_expression_tree.hpp"

#include <cassert>
#include <iostream>
#include <algorithm>

namespace r0 {

parser_expression_tree::parser_expression_tree() 
    : type(expression_tree::INVALID) {}

parser_expression_tree::parser_expression_tree(const std::string& var_name) 
    : type(expression_tree::SYMBOL), symbol_name(var_name) {}

parser_expression_tree::parser_expression_tree(const mpq_class& n) 
    : type(expression_tree::EXACT_NUMBER), exact_number(n) {}

parser_expression_tree::parser_expression_tree(const mpf_class& n) 
    : type(expression_tree::APPROXIMATE_NUMBER), approximate_number(n) {}

parser_expression_tree::parser_expression_tree(double n) 
    : type(expression_tree::APPROXIMATE_NUMBER), approximate_number(n) {}

parser_expression_tree::parser_expression_tree(const operands_t& list) 
    : type(expression_tree::OPERATOR), symbol_name("List"), operands(list) {}

parser_expression_tree::parser_expression_tree(const std::string& op_name, const operands_t& ops)
    : type(expression_tree::OPERATOR), symbol_name(op_name), operands(ops) {}

parser_expression_tree::parser_expression_tree(const boost::fusion::vector< std::string, operands_t >& fc)
   : type(expression_tree::OPERATOR), symbol_name(boost::fusion::at_c<0>( fc )), operands(boost::fusion::at_c<1>( fc )) {}

parser_expression_tree& parser_expression_tree::operator+=( const parser_expression_tree& rhs ) {
    do_binary_fill_op_list_with_this("Plus", rhs);
    return *this;
}

parser_expression_tree& parser_expression_tree::operator-=( const parser_expression_tree& rhs ) {
    do_binary_fill_op_list_with_this("Plus", -rhs);
    return *this;
}

parser_expression_tree& parser_expression_tree::operator*=( const parser_expression_tree& rhs ) {
    do_binary_fill_op_list_with_this("Times", rhs);
    return *this;
}

parser_expression_tree& parser_expression_tree::operator/=( const parser_expression_tree& rhs ) {
    assert( type != expression_tree::INVALID && rhs.type != expression_tree::INVALID );

    operands_t ops;
    ops.push_back( rhs );
    ops.push_back( mpq_class( -1 ) );

    parser_expression_tree newrhs("Power", ops);

    //do_binary_op_with_this("Times", newrhs);
    *this *= newrhs;
    return *this;
}

parser_expression_tree& parser_expression_tree::operator^=( const parser_expression_tree& rhs ) {
    do_binary_fill_op_list_with_this("Power", rhs);
    return *this;
}

parser_expression_tree& parser_expression_tree::operator==( const parser_expression_tree& rhs ) {
    do_binary_fill_op_list_with_this("Set", rhs);
    return *this;
}

parser_expression_tree& parser_expression_tree::operator>>=( const parser_expression_tree& rhs ) {
    do_binary_fill_op_list_with_this("Rule", rhs);
    return *this;
}

parser_expression_tree parser_expression_tree::operator-() const {
    assert( type != expression_tree::INVALID );
    parser_expression_tree copy = *this;
    return copy *= mpq_class(-1);
}

parser_expression_tree parser_expression_tree::operator+() const {
    assert( type != expression_tree::INVALID );
    return parser_expression_tree(*this);
}

void parser_expression_tree::do_binary_op_with_this( const std::string& opname, const parser_expression_tree& rhs ) {
    assert( type != expression_tree::INVALID && rhs.type != expression_tree::INVALID );
       
    operands_t ops;
    ops.push_back(*this);
    ops.push_back(rhs);

    type = expression_tree::OPERATOR;
    operands = ops;
    symbol_name = opname;   
}

void parser_expression_tree::do_binary_fill_op_list_with_this( const std::string& opname, const parser_expression_tree& rhs ) {
    assert( type != expression_tree::INVALID && rhs.type != expression_tree::INVALID );

    if ( type == expression_tree::OPERATOR && symbol_name == opname ) {
        operands.push_back( rhs );
    } else {
        do_binary_op_with_this( opname, rhs );
    }
}

expression_tree parser_expression_tree::to_expression_tree() const {

    switch(type) {
    case expression_tree::INVALID :
        return expression_tree::make_invalid();
        break;
    case expression_tree::APPROXIMATE_NUMBER :
        return expression_tree::make_approximate_number( approximate_number );
        break;
    case expression_tree::EXACT_NUMBER :
        return expression_tree::make_exact_number( exact_number );
        break;
    case expression_tree::SYMBOL :
        return expression_tree::make_symbol( symbol_name );
        break;
    case expression_tree::OPERATOR :
        {
            //TODO possible optimazilation
            expression_tree::operands_t resops;
            for(unsigned i = 0; i < operands.size(); ++i) {
                resops.push_back( operands[i].to_expression_tree() );
            }
            return expression_tree::make_operator( symbol_name, resops );
            break;
        }
    default:
        std::cout << "parser_expression_tree::to_expression_tree() default branch invoked" << std::endl;
        return expression_tree(); 
        break;
    }
}

} //namespace r0