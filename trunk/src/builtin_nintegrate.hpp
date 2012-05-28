
#ifndef BUILTIN_NINTEGRATE_HPP_
#define BUILTIN_NINTEGRATE_HPP_

#include "expression_tree.hpp"

namespace r0 { namespace builtin {

expression_tree NIntegrate(const expression_tree::operands_t& ops, enviroment& env);

namespace nintegrate_impl {

struct iterator_spec_t {    
    std::string variable; //If there is no variable, then variable == ""
    mpq_class start;
    mpq_class end;
};

bool validate_iterator(const expression_tree& it, iterator_spec_t& result);

} //namespace nintegrate_impl

}} //namespace r0::builtin

#endif //BUILTIN_NINTEGRATE_HPP_