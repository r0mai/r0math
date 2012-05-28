
#ifndef SYMBOL_MAP_HPP_
#define SYMBOL_MAP_HPP_

#include <string>

#include <boost/unordered_map.hpp>

#include "symbol.hpp"

namespace r0 {

class symbol_map {
public:

    symbol_map();

    bool get_symbol( const std::string& str, symbol& result );

    //returns false if str is already in the symbol map. changes it's value eiter way
    void set_variable( const std::string& str, const expression_tree& value );
private:
    typedef boost::unordered_map<std::string, symbol> symbols_t;
    symbols_t symbols;
};

} //namespace r0

#endif //SYMBOL_MAP_HPP_
