
#include "lexer.hpp"

#include <iostream>
#include <string>
#include <vector>

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/qi_real.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
 
namespace spirit = boost::spirit;
namespace qi = boost::spirit::qi;

namespace r0 {

//TODO rewrite this from scratch
template<class Iterator>
struct main_grammar : qi::grammar<Iterator, parser_expression_tree(), spirit::ascii::blank_type> {    

    main_grammar() : main_grammar::base_type(start) {
        
        using qi::char_;
        using qi::digit;
        using qi::ascii::alpha;
        using qi::ascii::alnum;
        using qi::eps;
        using qi::_1;
        using qi::_val;
        using qi::lit;

        start %= expr.alias();
        expr %= assing_expression.alias();

        text %= alpha >> *alnum;
        variable %= text;         
        function_call_pre %= text >> '[' >> -(expr % ',') >> ']';
        function_call %= function_call_pre;   

        assing_expression = rule_expression[_val = _1] >> *( char_('=') >> rule_expression[_val == _1] ); //right to left associavity handled at function call level (see workaround below)

        rule_expression = additive_expression[_val = _1] >> *(lit("->") >> additive_expression[_val >>= _1]); //right to left associavity handled at function call level (see workaround below)

        additive_expression = multiplicative_expression[_val = _1] >> *( char_('+') >> multiplicative_expression[_val += _1] | char_('-') >> multiplicative_expression[_val -= _1] );        

        multiplicative_expression = power_expression[_val = _1] >> *( char_('*') >> power_expression[_val *= _1] | char_('/') >> power_expression[_val /= _1] | ( !(char_('+') | char_('-')) >> power_expression[_val *= _1] ) );

        power_expression = primary[_val = _1] >> *( char_('^') >> primary[_val ^= _1] ); //right to left associavity handled at function call level (see workaround below)

        //unrary_op = ('-' >> primary)[_val = -_1] | ('+' >> primary)[_val = _1];
        
        //both seems equivalent
        //unrary_op = ('-' >> multiplicative_expression)[_val = -_1] | ('+' >> multiplicative_expression)[_val = _1];
        unrary_op = ('-' >> power_expression)[_val = -_1] | ('+' >> power_expression)[_val = _1];

        list_pre %= '{' >> -(expr % ',') >> '}';
        list %= list_pre;

        exact_number %= mpz_exact_number_parser;

        approximate_number %= mpf_approximate_number_parser;

        parenthesized_expression = ('(' >> expr[_val = (_1 *= parser_expression_tree( mpq_class(1) ))] >> ')'); //*=1 workaround for right to left assoc bug

        primary %= unrary_op | parenthesized_expression | list | function_call | variable | approximate_number | exact_number;
    }

    qi::uint_parser< mpq_class > mpz_exact_number_parser;
    qi::real_parser< double, qi::strict_ureal_policies<double> > mpf_approximate_number_parser; //4E parses as mpf insted of Times[4,E]
    //qi::real_parser<mpf_class, qi::strict_ureal_policies<mpf_class> > mpz_approximate_number_parser; //mpir(gmp) can't do pow(mpf,mpf) for some reason. so no parsing :(

    qi::rule<Iterator, parser_expression_tree(), spirit::ascii::blank_type> start;
    qi::rule<Iterator, parser_expression_tree(), spirit::ascii::blank_type> expr;
    qi::rule<Iterator, parser_expression_tree(), spirit::ascii::blank_type> assing_expression; //Set[]
    qi::rule<Iterator, parser_expression_tree(), spirit::ascii::blank_type> rule_expression; //Rule[]
    qi::rule<Iterator, parser_expression_tree(), spirit::ascii::blank_type> additive_expression;
    qi::rule<Iterator, parser_expression_tree(), spirit::ascii::blank_type> multiplicative_expression;
    qi::rule<Iterator, parser_expression_tree(), spirit::ascii::blank_type> power_expression;
    qi::rule<Iterator, parser_expression_tree(), spirit::ascii::blank_type> primary;
    qi::rule<Iterator, boost::fusion::vector< std::string, std::vector<parser_expression_tree> >(), spirit::ascii::blank_type> function_call_pre;
    qi::rule<Iterator, parser_expression_tree(), spirit::ascii::blank_type> function_call;
    qi::rule<Iterator, parser_expression_tree()> exact_number;
    qi::rule<Iterator, parser_expression_tree()> approximate_number; 
    qi::rule<Iterator, std::vector<parser_expression_tree>(), spirit::ascii::blank_type> list_pre;
    qi::rule<Iterator, parser_expression_tree(), spirit::ascii::blank_type> list; //{a,b,c}
    qi::rule<Iterator, parser_expression_tree(), spirit::ascii::blank_type> unrary_op;
    qi::rule<Iterator, parser_expression_tree(), spirit::ascii::blank_type> variable;
    qi::rule<Iterator, parser_expression_tree(), spirit::ascii::blank_type> parenthesized_expression;
    qi::rule<Iterator, std::string()> text; //for variable and funtion names

};

lexer::lexer() {}

bool lexer::do_lex(const std::string& input, parser_expression_tree& result) {

    std::string::const_iterator begin = input.begin();
    std::string::const_iterator end = input.end();

    main_grammar<std::string::const_iterator> mg;
    
    bool success = qi::phrase_parse(
        begin, end,
        mg,
        spirit::ascii::blank,
        result);

    return success && begin == end;

}

} //namespace r0 
