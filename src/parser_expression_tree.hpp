
#ifndef PARSER_EXPRESSION_TREE_HPP_
#define PARSER_EXPRESSION_TREE_HPP_

#include <boost/fusion/include/vector.hpp>
#include <boost/fusion/include/at_c.hpp>

#include "expression_tree.hpp"

namespace r0 {

//this class is used by the, parser. it will be copied into similiarly structured (but cleaner) expression_tree
class parser_expression_tree {
public:
    typedef std::vector<parser_expression_tree> operands_t;

    //http://www.boost.org/doc/libs/1_47_0/libs/spirit/doc/html/spirit/advanced/customize/transform.html
    //might need this, to get rid of these constructors
    parser_expression_tree();
    parser_expression_tree(const std::string& var_name);
    parser_expression_tree(const mpq_class& n);
    parser_expression_tree(const mpf_class& n);
    parser_expression_tree(double n);
    parser_expression_tree(const operands_t& n); //FOR LISTS !!!
    parser_expression_tree(const std::string& op_name, const operands_t& operands);
    parser_expression_tree(const boost::fusion::vector< std::string, operands_t >& function_call); //for spirit::qi attributes

    parser_expression_tree& operator+=( const parser_expression_tree& rhs );
    parser_expression_tree& operator-=( const parser_expression_tree& rhs );
    parser_expression_tree& operator*=( const parser_expression_tree& rhs );
    parser_expression_tree& operator/=( const parser_expression_tree& rhs );
    parser_expression_tree& operator^=( const parser_expression_tree& rhs ); //!! Power not bitwise xor
    parser_expression_tree& operator==( const parser_expression_tree& rhs ); //!! Set (=) not equality
    parser_expression_tree& operator>>=( const parser_expression_tree& rhs ); //!! Rule (->) not right shift assingment
    parser_expression_tree operator-() const;
    parser_expression_tree operator+() const;  

    expression_tree to_expression_tree() const;
private:

    void do_binary_op_with_this( const std::string& opname, const parser_expression_tree& rhs );
    void do_binary_fill_op_list_with_this( const std::string& opname, const parser_expression_tree& rhs );

    expression_tree::type_t type;          

    mpq_class exact_number;
    mpf_class approximate_number;
    std::string symbol_name; //for OPERATOR and SYMBOL
    operands_t operands;

};



} //namespace r0

#endif //PARSER_EXPRESSION_TREE_HPP_
