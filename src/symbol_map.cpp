
#include "symbol_map.hpp"

#include <boost/assign.hpp>

#include "include_builtin_functions.hpp"

#include "constant_pi.hpp"
#include "constant_e.hpp"

namespace r0 {


symbol_map::symbol_map() {
    using boost::assign::list_of;

    // Attributes
    // NumericFunction : function builtin::F also have a builtin::numeric_F returning a numeric approximation
    // Orderless : function arguments sorted after evaulation
    // HoldAll : no argument evaluation
    // HoldFirst : first argument not evaluated
    // RightLeftAssociativity : if called with more then 2 args, then f[x,y,z] -> f[x,f[y,z]]


    //Functions
    symbols.insert( symbols_t::value_type("Plus", symbol::make_function("Plus", &builtin::Plus, &builtin::numeric_Plus, list_of("Orderless")("NumericFunction")("Listable")("Flat") ) ) );
    symbols.insert( symbols_t::value_type("Minus", symbol::make_function("Minus", &builtin::Minus, 0 ) ) );
    symbols.insert( symbols_t::value_type("Times", symbol::make_function("Times", &builtin::Times,  &builtin::numeric_Times, list_of("Orderless")("NumericFunction")("Flat") ) ) );
    symbols.insert( symbols_t::value_type("Divide", symbol::make_function("Divide", &builtin::Divide, 0 ) ) );
    symbols.insert( symbols_t::value_type("Power", symbol::make_function("Power", &builtin::Power, &builtin::numeric_Power, list_of("NumericFunction")("RightLeftAssociativity")  ) ) );
    symbols.insert( symbols_t::value_type("Total", symbol::make_function("Total", &builtin::Total, 0 ) ) ); //doesn't need NumericFunction => resolves to Plus anyway
    symbols.insert( symbols_t::value_type("List", symbol::make_function("List", &builtin::List, 0 ) ) );
    symbols.insert( symbols_t::value_type("Part", symbol::make_function("Part", &builtin::Part, 0 ) ) );
    symbols.insert( symbols_t::value_type("HoldForm", symbol::make_function("HoldForm", &builtin::HoldForm, 0, list_of("HoldAll") ) ) );
    symbols.insert( symbols_t::value_type("IntegerDigits", symbol::make_function("IntegerDigits", &builtin::IntegerDigits, 0 ) ) );
    symbols.insert( symbols_t::value_type("Factorial", symbol::make_function("Factorial", &builtin::Factorial, 0, list_of("NumericFunction") ) ) );
    symbols.insert( symbols_t::value_type("GCD", symbol::make_function("GCD", &builtin::GCD, 0, list_of("Orderless")("Flat") ) ) );
    symbols.insert( symbols_t::value_type("LCM", symbol::make_function("LCM", &builtin::LCM,  0, list_of("Orderless")("Flat") ) ) );
    symbols.insert( symbols_t::value_type("ByteCount", symbol::make_function("ByteCount", &builtin::ByteCount, 0 ) ) );
    symbols.insert( symbols_t::value_type("Length", symbol::make_function("Length", &builtin::Length, 0 ) ) );
    symbols.insert( symbols_t::value_type("Set", symbol::make_function("Set", &builtin::Set,   0,         list_of("HoldFirst")("RightLeftAssociativity") ) ) );
    symbols.insert( symbols_t::value_type("N", symbol::make_function("N", &builtin::N, 0 ) ) );    
    symbols.insert( symbols_t::value_type("Table", symbol::make_function("Table", &builtin::Table, 0, list_of("HoldFirst") ) ) );
    symbols.insert( symbols_t::value_type("Sin", symbol::make_function("Sin", &builtin::Sin, &builtin::numeric_Sin, list_of("NumericFunction") ) ) );
    symbols.insert( symbols_t::value_type("Cos", symbol::make_function("Cos", &builtin::Cos, &builtin::numeric_Cos, list_of("NumericFunction") ) ) );
    symbols.insert( symbols_t::value_type("Tan", symbol::make_function("Tan", &builtin::Tan, &builtin::numeric_Tan, list_of("NumericFunction") ) ) );
    symbols.insert( symbols_t::value_type("Cot", symbol::make_function("Cot", &builtin::Cot, &builtin::numeric_Cot, list_of("NumericFunction") ) ) );
    symbols.insert( symbols_t::value_type("Sec", symbol::make_function("Sec", &builtin::Sec, &builtin::numeric_Sec, list_of("NumericFunction") ) ) );
    symbols.insert( symbols_t::value_type("Csc", symbol::make_function("Csc", &builtin::Csc, &builtin::numeric_Csc, list_of("NumericFunction") ) ) );
    symbols.insert( symbols_t::value_type("ArcSin", symbol::make_function("ArcSin", &builtin::ArcSin, &builtin::numeric_ArcSin, list_of("NumericFunction") ) ) );
    symbols.insert( symbols_t::value_type("ArcCos", symbol::make_function("ArcCos", &builtin::ArcCos, &builtin::numeric_ArcCos, list_of("NumericFunction") ) ) );
    symbols.insert( symbols_t::value_type("ArcTan", symbol::make_function("ArcTan", &builtin::ArcTan, &builtin::numeric_ArcTan, list_of("NumericFunction") ) ) );
    symbols.insert( symbols_t::value_type("Sinh", symbol::make_function("Sinh", &builtin::Sinh, &builtin::numeric_Sinh, list_of("NumericFunction") ) ) );
    symbols.insert( symbols_t::value_type("Cosh", symbol::make_function("Cosh", &builtin::Cosh, &builtin::numeric_Cosh, list_of("NumericFunction") ) ) );
    symbols.insert( symbols_t::value_type("Tanh", symbol::make_function("Tanh", &builtin::Tanh, &builtin::numeric_Tanh, list_of("NumericFunction") ) ) );
    symbols.insert( symbols_t::value_type("Coth", symbol::make_function("Coth", &builtin::Coth, &builtin::numeric_Coth, list_of("NumericFunction") ) ) );
    symbols.insert( symbols_t::value_type("Sech", symbol::make_function("Sech", &builtin::Sech, &builtin::numeric_Sech, list_of("NumericFunction") ) ) );
    symbols.insert( symbols_t::value_type("Csch", symbol::make_function("Csch", &builtin::Csch, &builtin::numeric_Csch, list_of("NumericFunction") ) ) );
    symbols.insert( symbols_t::value_type("Abs", symbol::make_function("Abs", &builtin::Abs, &builtin::numeric_Abs, list_of("NumericFunction") ) ) );
    symbols.insert( symbols_t::value_type("Apply", symbol::make_function("Apply", &builtin::Apply, 0 ) ) );
    symbols.insert( symbols_t::value_type("Log", symbol::make_function("Log", &builtin::Log, &builtin::numeric_Log, list_of("NumericFunction") ) ) );
    symbols.insert( symbols_t::value_type("Rational", symbol::make_function("Rational", &builtin::Rational, 0 ) ) );
    symbols.insert( symbols_t::value_type("Sqrt", symbol::make_function("Sqrt", &builtin::Sqrt, 0 ) ) );
    symbols.insert( symbols_t::value_type("Plot", symbol::make_function("Plot", &builtin::Plot, 0, list_of("HoldFirst") ) ) );
    symbols.insert( symbols_t::value_type("InputForm", symbol::make_function("InputForm", &builtin::InputForm, 0 ) ) );
    symbols.insert( symbols_t::value_type("FullForm", symbol::make_function("FullForm", &builtin::FullForm, 0 ) ) );
    symbols.insert( symbols_t::value_type("CodeForm", symbol::make_function("CodeForm", &builtin::CodeForm, 0 ) ) );
    symbols.insert( symbols_t::value_type("Rule", symbol::make_function("Rule", &builtin::Rule, 0, list_of("RightLeftAssociativity") ) ) );
    symbols.insert( symbols_t::value_type("Graphics", symbol::make_function("Graphics", &builtin::Graphics, 0) ) );
    symbols.insert( symbols_t::value_type("Line", symbol::make_function("Line", &builtin::Line, 0) ) );
    symbols.insert( symbols_t::value_type("Circle", symbol::make_function("Circle", &builtin::Circle, 0) ) );
    symbols.insert( symbols_t::value_type("Point", symbol::make_function("Point", &builtin::Point, 0) ) );
    symbols.insert( symbols_t::value_type("Disk", symbol::make_function("Disk", &builtin::Disk, 0) ) );
    symbols.insert( symbols_t::value_type("Rectangle", symbol::make_function("Rectangle", &builtin::Rectangle, 0) ) );
    symbols.insert( symbols_t::value_type("Map", symbol::make_function("Map", &builtin::Map, 0) ) );
    symbols.insert( symbols_t::value_type("RGBColor", symbol::make_function("RGBColor", &builtin::RGBColor, 0) ) );
    symbols.insert( symbols_t::value_type("Hue", symbol::make_function("Hue", &builtin::Hue, 0) ) );
    symbols.insert( symbols_t::value_type("Accumulate", symbol::make_function("Accumulate", &builtin::Accumulate, 0) ) );
    symbols.insert( symbols_t::value_type("RandomChoice", symbol::make_function("RandomChoice", &builtin::RandomChoice, 0) ) );
    symbols.insert( symbols_t::value_type("NIntegrate", symbol::make_function("NIntegrate", &builtin::NIntegrate, 0, list_of("HoldFirst")) ) );
    symbols.insert( symbols_t::value_type("D", symbol::make_function("D", &builtin::D, 0 ) ) );
    symbols.insert( symbols_t::value_type("NumericQ", symbol::make_function("NumericQ", &builtin::NumericQ, 0 ) ) );
    symbols.insert( symbols_t::value_type("Expand", symbol::make_function("Expand", &builtin::Expand, 0 ) ) );
    symbols.insert( symbols_t::value_type("Nest", symbol::make_function("Nest", &builtin::Nest, 0 ) ) );
    symbols.insert( symbols_t::value_type("NestList", symbol::make_function("NestList", &builtin::NestList, 0 ) ) );
    symbols.insert( symbols_t::value_type("FindRoot", symbol::make_function("FindRoot", &builtin::FindRoot, 0 ) ) );
    symbols.insert( symbols_t::value_type("Exp", symbol::make_function("Exp", &builtin::Exp, 0 ) ) );
    symbols.insert( symbols_t::value_type("Less", symbol::make_function("Less", &builtin::Less, 0 ) ) );
    symbols.insert( symbols_t::value_type("LessEqual", symbol::make_function("LessEqual", &builtin::LessEqual, 0 ) ) );
    symbols.insert( symbols_t::value_type("Greater", symbol::make_function("Greater", &builtin::Greater, 0 ) ) );
    symbols.insert( symbols_t::value_type("GreaterEqual", symbol::make_function("GreaterEqual", &builtin::GreaterEqual, 0 ) ) );
    symbols.insert( symbols_t::value_type("Not", symbol::make_function("Not", &builtin::Not, 0 ) ) );
    symbols.insert( symbols_t::value_type("And", symbol::make_function("And", &builtin::And, 0, list_of("HoldAll")("Flat") ) ) );
    symbols.insert( symbols_t::value_type("Or", symbol::make_function("Or", &builtin::Or, 0, list_of("HoldAll")("Flat") ) ) );
    symbols.insert( symbols_t::value_type("Together", symbol::make_function("Together", &builtin::Together, 0 ) ) );
    symbols.insert( symbols_t::value_type("FromDigits", symbol::make_function("FromDigits", &builtin::FromDigits, 0 ) ) );
    symbols.insert( symbols_t::value_type("PrimeQ", symbol::make_function("PrimeQ", &builtin::PrimeQ, 0 ) ) );
    symbols.insert( symbols_t::value_type("Prime", symbol::make_function("Prime", &builtin::Prime, 0 ) ) );
    symbols.insert( symbols_t::value_type("PrimePi", symbol::make_function("PrimePi", &builtin::PrimePi, 0 ) ) );
    symbols.insert( symbols_t::value_type("FactorInteger", symbol::make_function("FactorInteger", &builtin::FactorInteger, 0 ) ) );
    symbols.insert( symbols_t::value_type("ExtendedGCD", symbol::make_function("ExtendedGCD", &builtin::ExtendedGCD, 0 ) ) );
    symbols.insert( symbols_t::value_type("Det", symbol::make_function("Det", &builtin::Det, 0 ) ) );


    //Variables

    //constant exact values

    //Degree = Pi/180
    symbols.insert( symbols_t::value_type("Degree", symbol::make_variable("Degree", 
        expression_tree::make_operator( "Times",
            expression_tree::make_exact_number( mpq_class(1,180) ),
            expression_tree::make_symbol( "Pi" )
        )
    )));

    //GoldenRatio = (Sqrt[5]+1)/2
    symbols.insert( symbols_t::value_type("GoldenRatio", symbol::make_variable("GoldenRatio", 
        expression_tree::make_operator( "Times",
            expression_tree::make_exact_number( mpq_class(1, 2) ),
            expression_tree::make_operator( "Plus",
                expression_tree::make_exact_number( mpq_class(1, 1) ),
                expression_tree::make_operator( "Power",
                    expression_tree::make_exact_number( mpq_class(5, 1) ),
                    expression_tree::make_exact_number( mpq_class(1, 2) )
                )
            )
        )
    )));

    //colors
    symbols.insert( symbols_t::value_type("Red", symbol::make_variable("Red", expression_tree::make_operator( "RGBColor", 
            list_of
            (expression_tree::make_exact_number(1))
            (expression_tree::make_exact_number(0))
            (expression_tree::make_exact_number(0))
            )) ) );
    symbols.insert( symbols_t::value_type("Green", symbol::make_variable("Green", expression_tree::make_operator( "RGBColor", 
            list_of
            (expression_tree::make_exact_number(0))
            (expression_tree::make_exact_number(1))
            (expression_tree::make_exact_number(0))
            )) ) );
    symbols.insert( symbols_t::value_type("Blue", symbol::make_variable("Blue", expression_tree::make_operator( "RGBColor", 
            list_of
            (expression_tree::make_exact_number(0))
            (expression_tree::make_exact_number(0))
            (expression_tree::make_exact_number(1))
            )) ) );
    symbols.insert( symbols_t::value_type("Yellow", symbol::make_variable("Yellow", expression_tree::make_operator( "RGBColor", 
            list_of
            (expression_tree::make_exact_number(1))
            (expression_tree::make_exact_number(1))
            (expression_tree::make_exact_number(0))
            )) ) );
    symbols.insert( symbols_t::value_type("Black", symbol::make_variable("Black", expression_tree::make_operator( "RGBColor", 
            list_of
            (expression_tree::make_exact_number(0))
            (expression_tree::make_exact_number(0))
            (expression_tree::make_exact_number(0))
            )) ) );
    symbols.insert( symbols_t::value_type("White", symbol::make_variable("White", expression_tree::make_operator( "RGBColor", 
            list_of
            (expression_tree::make_exact_number(1))
            (expression_tree::make_exact_number(1))
            (expression_tree::make_exact_number(1))
            )) ) );
    symbols.insert( symbols_t::value_type("LightBlue", symbol::make_variable("LightBlue", expression_tree::make_operator( "RGBColor", 
            list_of
            (expression_tree::make_approximate_number( 56. / 255. ))
            (expression_tree::make_approximate_number( 56. / 255. ))
            (expression_tree::make_approximate_number( 170. / 255. ))
            )) ) );
    symbols.insert( symbols_t::value_type("LightRed", symbol::make_variable("LightRed", expression_tree::make_operator( "RGBColor", 
            list_of
            (expression_tree::make_approximate_number( 170. / 255. ))
            (expression_tree::make_approximate_number( 56. / 255. ))
            (expression_tree::make_approximate_number( 56. / 255. ))
            )) ) );
    symbols.insert( symbols_t::value_type("LightGreen", symbol::make_variable("LightGreen", expression_tree::make_operator( "RGBColor", 
            list_of
            (expression_tree::make_approximate_number( 56. / 255. ))
            (expression_tree::make_approximate_number( 170. / 255. ))
            (expression_tree::make_approximate_number( 56. / 255. ))
            )) ) );

    //Constants
    symbols.insert( symbols_t::value_type("Pi", symbol::make_constant("Pi", &constants::Pi) ) );
    symbols.insert( symbols_t::value_type("E", symbol::make_constant("E", &constants::E) ) );
}


bool symbol_map::get_symbol( const std::string& str, symbol& result ) {
    symbols_t::iterator it = symbols.find( str );
    if ( it != symbols.end() ) {
        result = it->second;
        return true;
    } else {
        return false;
    }
}

void symbol_map::set_variable( const std::string& str, const expression_tree& value ) {
    /*const std::pair<symbols_t::iterator, bool> insert_res = 
        symbols.insert( symbols_t::value_type(str, symbol::make_variable(str, value) ) );*/
    symbols[str] = symbol::make_variable(str, value);
}

} //namespace r0