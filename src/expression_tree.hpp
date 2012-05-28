
#ifndef EXPRESSION_TREE_HPP_
#define EXPRESSION_TREE_HPP_

#include <vector>
#include <string>
#include <iosfwd>

#include "enviroment_fwd.hpp"
#include "mpirxx_include.hpp"

#include "expression_tree_fwd.hpp"

namespace r0 {

class expression_tree {
public:

    enum type_t { //changing the order will change how the comparsion operators work.
        EXACT_NUMBER = 0,
        APPROXIMATE_NUMBER = 1,
        SYMBOL = 2,
        OPERATOR = 3,
        INVALID = 4,
        //STRING TODO support strings
    };

    typedef std::vector<expression_tree> operands_t;

    expression_tree(); //makes INVALID type
    expression_tree(const expression_tree& other);

    expression_tree& operator=(const expression_tree& other);

    static expression_tree make_invalid();
    static expression_tree make_symbol(const std::string& variable_name);
    static expression_tree make_exact_number(const mpq_class& n);
    static expression_tree make_approximate_number(const mpf_class& n);    
    static expression_tree make_operator(const std::string& op_name); //creates an operator, with empty argument list
    static expression_tree make_operator(const std::string& op_name, const expression_tree& single_op); //for a single operand
    static expression_tree make_operator(const std::string& op_name, const expression_tree& rhs, const expression_tree& lhs); //for two operands
    static expression_tree make_operator(const std::string& op_name, const operands_t& operands);
    template<class C> //any container
    static expression_tree make_operator(const std::string& op_name, const C& operands);

    type_t get_type() const;
    const mpq_class& get_exact_number() const;
    mpq_class& get_exact_number();
    const mpf_class& get_approximate_number() const;
    mpf_class& get_approximate_number();
    const std::string& get_symbol() const;
    const std::string& get_operator_name() const;
    const operands_t& get_operands() const;
    operands_t& get_operands();

    //returns type == EXACT_NUMBER || type == APPROXIMATE_NUMBER
    bool is_number_type() const;
    //returns type == EXACT_NUMBER && is_mpq_integer(exact_number)
    bool is_integer() const;
    //returns type == OPERATOR && operator_name == name
    bool is_operator(const std::string& name) const;

    //returns mpf class either from exact_number, or approximate_number
    mpf_class get_number_as_mpf() const;

    //Tries evaluating the tree
    //return *this
    expression_tree& evaluate(enviroment& env, bool is_numeric = false);
    //expression_tree& numeric_evaluate(enviroment& env);

    std::string output(enviroment& env) const;

    std::string to_fullform(enviroment& env) const;
    std::string to_inputform(enviroment& env) const;
    std::string to_codeform(enviroment& env) const;

    std::size_t get_memory_size() const;

private:

    type_t type;    

    mpq_class exact_number;
    mpf_class approximate_number;
    std::string symbol_name; //for OPERATOR and SYMBOL
    operands_t operands;
    
    void copy_to_this(const expression_tree& other);
    void flatten();

};

//Does the comparsion using the expression_tree's layout, not numerical, or symbolical value
//TODO maybe no operator overloading for these?
bool operator<(const expression_tree& lhs, const expression_tree& rhs);
bool operator==(const expression_tree& lhs, const expression_tree& rhs);
//not using boost::operator, because of long compile times
bool operator!=(const expression_tree& lhs, const expression_tree& rhs);
bool operator>(const expression_tree& lhs, const expression_tree& rhs);
bool operator<=(const expression_tree& lhs, const expression_tree& rhs);
bool operator>=(const expression_tree& lhs, const expression_tree& rhs);

//template IMPL :

template<class C>
expression_tree expression_tree::make_operator(const std::string& op_name, const C& operands) {
    expression_tree res;
    res.type = OPERATOR;
    res.symbol_name = op_name;
    res.operands = operands_t(operands.begin(), operands.end());
    return res;
}

} //namespace r0

#endif //EXPRESSION_TREE_HPP_