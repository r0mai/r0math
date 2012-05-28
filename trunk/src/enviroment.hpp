
#ifndef ENVIROMENT_HPP_
#define ENVIROMENT_HPP_

#include <string>

#include "symbol_map.hpp"
#include "lexer.hpp"
#include "enviroment_fwd.hpp"

namespace r0 {

class enviroment {
public:
    enviroment();
    
    int run();

    void set_print_error_messages(bool b);

    //Builtin functions use it, to print error messages
    void raise_error(const std::string& function_name, const std::string& error_text);

    symbol_map& get_symbols();
    const symbol_map& get_symbols() const;
private:
    void work_on_input(std::string input);

    symbol_map symbols;
    lexer lex;
public:
    bool print_debug;
    bool print_error_messages;
private:
};

} //namespace r0

#endif //ENVIROMENT_HPP_