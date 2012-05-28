
#ifndef LIST_STRUCTURE_PARSER_HPP_
#define LIST_STRUCTURE_PARSER_HPP_

#include <vector>
#include <string>

#include "mpirxx_include.hpp"
#include "expression_tree.hpp"

namespace r0 {

bool list_structure_parser(const std::string& pattern, const expression_tree& expr, std::vector<expression_tree>& result);

//calls list_structure_parser, then converts the args to mpf_class if possible 
bool list_structure_mpf_parser(const std::string& pattern, const expression_tree& expr, std::vector<mpf_class>& result);

//calls list_structure_parser, accepting only EXACT_NUMBER s
bool list_structure_mpq_parser(const std::string& pattern, const expression_tree& expr, std::vector<mpq_class>& result);

namespace list_parser_impl {

unsigned find_closing_brace(const std::string& pattern, int open_brace_position);

} //list_parser_impl

} //namespace r0

#endif //LIST_STRUCTURE_PARSER_HPP_