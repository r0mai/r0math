
#ifndef LEXER_HPP_
#define LEXER_HPP_

#include <string>

#include "expression_tree.hpp"
#include "parser_expression_tree.hpp"

namespace r0 {
    
class lexer {
public:
    lexer();
        
    bool do_lex(const std::string& input, parser_expression_tree& result);
private:

};

} //namespace r0

#endif //LEXER_HPP_