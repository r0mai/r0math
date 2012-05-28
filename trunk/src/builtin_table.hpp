
#ifndef BUILTIN_TABLE_HPP_
#define BUILTIN_TABLE_HPP_

#include "expression_tree.hpp"

namespace r0 { namespace builtin {

expression_tree Table(const expression_tree::operands_t& ops, enviroment& env);

namespace table_impl {

struct iterator_spec_t {    
    std::string variable; //If there is no variable, then variable == ""
    mpq_class start;
    mpq_class end;
    mpq_class step;
};

bool validate_iterator(const expression_tree& it, iterator_spec_t& result);

} //namespace table_impl

}} //namespace r0::builtin

#endif //BUILTIN_TABLE_HPP_