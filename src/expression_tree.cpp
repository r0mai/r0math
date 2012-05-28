
#include "expression_tree.hpp"

#include <cassert>
#include <iostream>
#include <sstream>

#include "util.hpp"
#include "inputform.hpp"
#include "graphics.hpp"
#include "fullform.hpp"
#include "codeform.hpp"
#include "enviroment.hpp"

namespace r0 {

expression_tree::expression_tree() : type(INVALID) {}

expression_tree::expression_tree(const expression_tree& other) {
    copy_to_this(other);
}

expression_tree& expression_tree::operator=(const expression_tree& other) {
    if ( this != &other ) {
        copy_to_this(other);
    }
    return *this;
}

void expression_tree::copy_to_this(const expression_tree& other) {
    

    type = other.type;
    switch(type) {
    case EXACT_NUMBER :
        exact_number = other.exact_number;
        break;
    case APPROXIMATE_NUMBER :
        approximate_number = other.approximate_number;
        break;
    case SYMBOL :
        symbol_name = other.symbol_name;
        break;
    case OPERATOR :
        symbol_name = other.symbol_name;
        operands = other.operands;
        break;
    case INVALID : //invalid is allowed here (no asserting)
        break;
    default:
        std::cout << "type : " << type << std::endl;
        assert(false && "default branch invoked");
    }
}

expression_tree expression_tree::make_invalid() {
    return expression_tree();
}

expression_tree expression_tree::make_symbol(const std::string& var_name) {
    expression_tree res;
    res.type = SYMBOL;
    res.symbol_name = var_name;
    return res;
}

expression_tree expression_tree::make_exact_number(const mpq_class& n) {
    expression_tree res;
    res.type = EXACT_NUMBER;
    res.exact_number = n;
    return res;
}

expression_tree expression_tree::make_approximate_number(const mpf_class& n) {
    expression_tree res;
    res.type = APPROXIMATE_NUMBER;
    res.approximate_number = n;
    return res;
}

expression_tree expression_tree::make_operator(const std::string& op_name) {
    return make_operator(op_name, operands_t());
}

expression_tree expression_tree::make_operator(const std::string& op_name, const expression_tree& single_op) {
    return make_operator(op_name, operands_t(1, single_op));
}

expression_tree expression_tree::make_operator(const std::string& op_name, const expression_tree& rhs, const expression_tree& lhs) {
    operands_t ops;
    ops.push_back(rhs);
    ops.push_back(lhs);
    return make_operator(op_name, ops);
}

expression_tree expression_tree::make_operator(const std::string& op_name, const operands_t& ops) {
    expression_tree res;
    res.type = OPERATOR;
    res.symbol_name = op_name;
    res.operands = ops;
    return res;
}

expression_tree::type_t expression_tree::get_type() const {
    return type;
}

const mpq_class& expression_tree::get_exact_number() const {
    assert(type == EXACT_NUMBER);
    return exact_number;
}

mpq_class& expression_tree::get_exact_number() {
    assert(type == EXACT_NUMBER);
    return exact_number;
}

const mpf_class& expression_tree::get_approximate_number() const {
    assert(type == APPROXIMATE_NUMBER);
    return approximate_number;
}

mpf_class& expression_tree::get_approximate_number() {
    assert(type == APPROXIMATE_NUMBER);
    return approximate_number;
}

const std::string& expression_tree::get_symbol() const {
    assert(type == SYMBOL);
    return symbol_name;
}

const std::string& expression_tree::get_operator_name() const {
    assert(type == OPERATOR);
    return symbol_name;
}

const expression_tree::operands_t& expression_tree::get_operands() const {
    assert(type == OPERATOR);
    return operands;
}

expression_tree::operands_t& expression_tree::get_operands() {
    assert(type == OPERATOR);
    return operands;
}

mpf_class expression_tree::get_number_as_mpf() const {
    assert(type == EXACT_NUMBER || type == APPROXIMATE_NUMBER);
    if ( type == EXACT_NUMBER ) {
        return mpf_class( exact_number );
    } else { //type == APPROXIMATE_NUMBER, undefined behaviour otherwise
        return approximate_number;
    } 
}

bool expression_tree::is_number_type() const {
    return type == EXACT_NUMBER || type == APPROXIMATE_NUMBER;
}

bool expression_tree::is_integer() const {
    return type == EXACT_NUMBER && is_mpq_integer(exact_number);
}

bool expression_tree::is_operator(const std::string& name) const {
    return type == OPERATOR && symbol_name == name;
}



expression_tree& expression_tree::evaluate(enviroment& env, const bool is_numeric) {
  
    assert(type != INVALID);   

    std::string before_eva;
    if (env.print_debug) {
        before_eva = to_fullform(env);
    }

    bool call_numeric_debug = is_numeric;

    switch(type) {
    case EXACT_NUMBER:
        //dont do this??
#if 0
        if (is_numeric) {
            type = APPROXIMATE_NUMBER;
            approximate_number = exact_number;
        }
#endif

        break;
    case APPROXIMATE_NUMBER:
        //we are done ??
        break;
    case SYMBOL:
        {
            symbol symres;
            if (env.get_symbols().get_symbol( symbol_name, symres )) {
                switch ( symres.get_type() ) {
                case symbol::SYMBOL :
                    *this = symres.get_variable_value();
                    //evaluate(env); is_numeric eval?
                    break;
                case symbol::CONSTANT :
                    if ( is_numeric ) {
                        type = APPROXIMATE_NUMBER;
                        approximate_number = symres.get_numeric_constant(30, env);
                    }
                    break;
                case symbol::FUNCTION :
                    break;
                default:
                    assert(false && "default case invoked");
                    break;
                }
            }
            break;                   
        }
    case OPERATOR:
        {
        
        symbol opsym;
        bool success = env.get_symbols().get_symbol( symbol_name, opsym );        


        const bool is_builtin_function = success && opsym.get_type() == symbol::FUNCTION;
        if (!is_builtin_function && env.print_debug ) {
            std::cout << "failed lookup in builtin_funtions : \"" << symbol_name << "\"\n";
        }        


        //TODO implement NHoldAll
        const bool holdall = opsym.has_attribute("HoldAll");
        const bool holdfirst = opsym.has_attribute("HoldFirst");
        const bool orderless = opsym.has_attribute("Orderless");
        const bool numericfunction = opsym.has_attribute("NumericFunction");
        const bool listable = opsym.has_attribute("Listable");
        const bool rightleftassociativity = opsym.has_attribute("RightLeftAssociativity");
        const bool flat = opsym.has_attribute("Flat");

        

        if ( rightleftassociativity ) {
            if ( operands.size() > 2 ) {
                expression_tree::operands_t ops2;
                std::copy(operands.begin() + 1, operands.end(), std::back_inserter(ops2));
                operands.resize(1);
                operands.push_back( expression_tree::make_operator( symbol_name, ops2 ) );
            }
        }



        if ( !holdall ) {
            operands_t::iterator begin = operands.begin();
            if ( holdfirst ) {
                ++begin;
            }                
            std::for_each( begin, operands.end(), [is_numeric, &env](expression_tree& expr) { expr.evaluate(env, is_numeric); } );
                                
        }

        //if called with exact args, but after eval, we find an approximate number, then repeat the process with numeric eval 

        const bool has_numeric_argument = std::find_if(operands.begin(), operands.end(),
            [](const expression_tree& expr){ return expr.get_type() == expression_tree::APPROXIMATE_NUMBER; }) != operands.end();           

        if ( !is_numeric && has_numeric_argument ) {
            if ( !holdall ) {
                operands_t::iterator begin = operands.begin();
                if ( holdfirst ) {
                    ++begin;
                }                
                std::for_each( begin, operands.end(), [is_numeric, &env](expression_tree& expr) { expr.evaluate(env, true); } );                                
            }                
        }
        // If Exists[Indeterminate in operands] => result is Indeterminate
        if ( numericfunction && std::find_if(operands.begin(), operands.end(), [](const expression_tree& expr)
            { return expr.get_type() == expression_tree::SYMBOL && expr.get_symbol() == "Indeterminate"; } )
            != operands.end() )
        {
            *this = expression_tree::make_symbol("Indeterminate");            
        } else {
        
            //not used right now
            bool done_listable_refactor = false; //skip *this evaulation, since *this gets reassingned
#if 0
        //Listable functions
        if ( listable ) {
            //-1 if no lists, size of list, >=0 if have lists
            int have_list = -1;

            for (unsigned i = 0; i < operands.size(); ++i) {

                const expression_tree& expr = operands[i];

                if ( expr.get_type() == OPERATOR && expr.get_operator_name() == "List" ) {
                    if ( have_list == -1 || expr.get_operands().size() == have_list ) {
                        have_list = expr.get_operands().size();
                    } else {
                        env.raise_error("Main", "listable function's arguments cannot be combined");
                        have_list = -1; //to break out of if(){}
                        break;
                    }
                }
            }
            if ( have_list != -1 ) {
                //create a have_list length list for the results
                operands_t listable_res(have_list, make_operator(symbol_name));

                for (unsigned i = 0; i < operands.size(); ++i) {
                    //if list, push back each element, if not then push back the same each time...
                    if ( operands[i].get_type() == OPERATOR && operands[i].get_operator_name() == "List" ) {
                        assert( have_list == operands[i].get_operands().size() );
                        for (unsigned j = 0; j < listable_res.size(); ++j) {
                            listable_res.push_back( operands[i].get_operands()[j] );
                        }
                    } else {
                        for (unsigned j = 0; j < listable_res.size(); ++j) {
                            listable_res.push_back( operands[i] );
                        }
                    }
                }
                done_listable_refactor = true;
                *this = expression_tree::make_operator("List", listable_res).evaluate(env); //possibly BUGGYY!!

            } //else nothing to do
        }
#endif
            

            if (env.print_debug) {
                //debug good here?
                before_eva = to_fullform(env);
            }

            if ( !done_listable_refactor ) {

                //TODO flat might not be good here
                if ( flat ) {
                    //std::cout << "BEFORE FLAT : " << this->to_fullform(env) << std::endl;
                    flatten();
                    //std::cout << "AFTER  FLAT : " << this->to_fullform(env) << std::endl;
                }

                bool call_numeric_version_of_this = numericfunction && (is_numeric || has_numeric_argument);

                if ( is_builtin_function ) { //if it is a builtin function
                    if ( call_numeric_version_of_this ) {
                        *this = opsym.call_numeric_function( operands, env );
                    } else {
                        *this = opsym.call_function( operands, env );
                    }
                }
            }
        }
            


        break;       
        }
    default:
        break;
    }

    //If we are an orderless operator, then sort
    if ( type == OPERATOR ) {
        symbol postopsym;
        bool success = env.get_symbols().get_symbol( symbol_name, postopsym );
        if ( success && postopsym.get_type() == symbol::FUNCTION && postopsym.has_attribute("Orderless") ) {
            std::sort( operands.begin(), operands.end() );                
        }
    }

    std::string after_eva;
    if (env.print_debug) {
        after_eva = to_fullform(env);
        std::cout << "Eva " << (call_numeric_debug ? 'N' : ' ') << " : " << before_eva << "  ->  " << after_eva << std::endl;
    }

    return *this;
}

std::string expression_tree::output(enviroment& env) const {
    if ( type == OPERATOR ) {
        if ( symbol_name == "Graphics" ) {
            return expression_tree_to_graphics(*this, env);
        }
        if ( operands.size() == 1 ) {
            if ( symbol_name == "FullForm" ) {
                return operands[0].to_fullform(env);
            } else if ( symbol_name == "InputForm" ) {
                return operands[0].to_inputform(env);
            } else if ( symbol_name == "CodeForm" ) {
                return operands[0].to_codeform(env);
            }
        }
    }

    return to_inputform(env);
}

std::string expression_tree::to_fullform(enviroment& env) const {
    return expression_tree_to_fullform(*this, env);
}

std::string expression_tree::to_inputform(enviroment& env) const {
    return expression_tree_to_inputform(*this, env);
}

std::string expression_tree::to_codeform(enviroment& env) const {
    return expression_tree_to_codeform(*this, env);
}

std::size_t expression_tree::get_memory_size() const {
    std::size_t size = sizeof(expression_tree);
    switch ( type ) {
        case OPERATOR :
            std::for_each( operands.begin(), operands.end(),
                [&size]( const expression_tree& expr ) { size += expr.get_memory_size(); } );
            break;
        default:
            break;
    } 
    return size;
}

void expression_tree::flatten() {
    assert( type == OPERATOR );

    //TODO fix f[f[a,b]]->f[a,b]

    operands_t flat_ops;
    
    for ( unsigned i = 0; i < operands.size(); ++i ) {                
        if ( operands[i].get_type() == OPERATOR && operands[i].get_operator_name() == symbol_name ) {
            operands[i].flatten();
            flat_ops.insert( flat_ops.end(), operands[i].get_operands().begin(), operands[i].get_operands().end() );
        } else {
            flat_ops.push_back( operands[i] );
        }
    }
    operands = flat_ops;
}

bool operator<(const expression_tree& lhs, const expression_tree& rhs) {
    assert(lhs.get_type() != expression_tree::INVALID && rhs.get_type() != expression_tree::INVALID);
    if ( lhs.get_type() != rhs.get_type() ) {
        return lhs.get_type() < rhs.get_type();
    } else {
        switch( lhs.get_type() ) {
        case expression_tree::EXACT_NUMBER :
            return lhs.get_exact_number() < rhs.get_exact_number();
        case expression_tree::APPROXIMATE_NUMBER :
            return lhs.get_approximate_number() < rhs.get_approximate_number();
        case expression_tree::SYMBOL :
            return lhs.get_symbol() < rhs.get_symbol();
        case expression_tree::OPERATOR :
            if ( lhs.get_operator_name() != rhs.get_operator_name() ) {
                return lhs.get_operator_name() < rhs.get_operator_name();
            } else {
                return std::lexicographical_compare( 
                    lhs.get_operands().begin(), 
                    lhs.get_operands().end(), 
                    rhs.get_operands().begin(), 
                    rhs.get_operands().end() );
            }
        default:
            assert(false && "default branch invoked"); 
            return false;
        }
    }
}

bool operator==(const expression_tree& lhs, const expression_tree& rhs) {
    if ( lhs.get_type() != rhs.get_type() ) {
        return false;
    } else {
        switch( lhs.get_type() ) {
        case expression_tree::EXACT_NUMBER :
            return lhs.get_exact_number() == rhs.get_exact_number();
        case expression_tree::APPROXIMATE_NUMBER :
            return lhs.get_approximate_number() == rhs.get_approximate_number();
        case expression_tree::SYMBOL :
            return lhs.get_symbol() == rhs.get_symbol();
        case expression_tree::OPERATOR :
            return lhs.get_operator_name() == rhs.get_operator_name() && lhs.get_operands() == rhs.get_operands();
        case expression_tree::INVALID :
            return true;
        default:
            assert(false && "default branch invoked"); 
            return false;        
        }
    }
}

bool operator!=(const expression_tree& lhs, const expression_tree& rhs) {
    return !(lhs == rhs);
}

bool operator>(const expression_tree& lhs, const expression_tree& rhs) {
    return rhs < lhs;
}

bool operator<=(const expression_tree& lhs, const expression_tree& rhs) {
    return !(rhs < lhs);
}

bool operator>=(const expression_tree& lhs, const expression_tree& rhs) {
    return !(lhs < rhs);
}




} //namespace r0