
#include "list_structure_parser.hpp"


#include <cctype>
#include <cassert>


namespace r0 {

bool list_structure_parser(const std::string& pattern, const expression_tree& expr, std::vector<expression_tree>& result) {


    if ( expr.get_type() != expression_tree::OPERATOR || 
        expr.get_operator_name() != "List" ) {
        return false;
    }

    unsigned i = 0;
    unsigned list_index = 0;

    while ( i < pattern.size() ) {

        const char ch = pattern[i];

        if ( std::isdigit(ch) ) {
            unsigned result_index = ch - '0';
            assert( result_index < result.size() );

            

            if ( list_index >= expr.get_operands().size() ) {
                return false;
            }

            result[result_index] = expr.get_operands()[list_index];
            ++i;
        } else {
            switch (ch) {
            case '{' : 
                {
                unsigned closing_brace = list_parser_impl::find_closing_brace(pattern, i);

                const std::string substring = pattern.substr(i+1, closing_brace-i-1);

                if ( !list_structure_parser(substring, expr.get_operands()[list_index], result) ) {
                    return false;
                }
                
                
                i += substring.size()+2;
                break;
                }
            case ',' :
                ++list_index;
                ++i;
                break;
            default :
                assert( false && "default case invoked in list_structrue_parser" );
                break;
            }
        }
    }
    return true;
}

bool list_structure_mpf_parser(const std::string& pattern, const expression_tree& expr, std::vector<mpf_class>& result) {
    std::vector<expression_tree> expr_res(result.size());
    if ( !list_structure_parser(pattern, expr, expr_res) ) {
        return false;
    }

    for( unsigned i = 0; i < expr_res.size(); ++i ) {
        if ( expr_res[i].get_type() != expression_tree::APPROXIMATE_NUMBER && 
            expr_res[i].get_type() != expression_tree::EXACT_NUMBER ) {
                return false;
        }
        result[i] = expr_res[i].get_number_as_mpf();
    }
    return true;
}

bool list_structure_mpq_parser(const std::string& pattern, const expression_tree& expr, std::vector<mpq_class>& result) {
    std::vector<expression_tree> expr_res(result.size());
    if ( !list_structure_parser(pattern, expr, expr_res) ) {
        return false;
    }

    for( unsigned i = 0; i < expr_res.size(); ++i ) {
        if ( expr_res[i].get_type() != expression_tree::EXACT_NUMBER ) {
                return false;
        }
        result[i] = expr_res[i].get_exact_number();
    }
    return true;
}

namespace list_parser_impl {

unsigned find_closing_brace(const std::string& pattern, int open_brace_position) {

    assert( pattern[open_brace_position] == '{' );

    int nest_level = 1;
    for ( int i = open_brace_position+1; i < static_cast<int>(pattern.size()); ++i ) {
        const char ch = pattern[i];
        switch ( ch ) {
        case '{':
            ++nest_level;
            break;
        case '}' :
            --nest_level;
            if ( nest_level == 0 ) {
                return i;
            }
            break;
        }

    }

    assert( false && "brace mismatch" );

    return 0;

}

} //list_parser_impl

} //namespace r0
