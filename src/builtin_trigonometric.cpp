
#include "builtin_trigonometric.hpp"

#include "enviroment.hpp"
#include "expression_tree.hpp"

#include <cassert>
#include <cmath>

namespace r0 { namespace builtin {

expression_tree Sin(const expression_tree::operands_t& ops, enviroment& env) {
    if ( ops.size() != 1 ) {
        env.raise_error("Sin", "called with invalid arguments");
        return expression_tree::make_operator("Sin", ops);
    }

    if ( ops[0].get_type() == expression_tree::OPERATOR && ops[0].get_operator_name() == "ArcSin" ) {
        if( ops[0].get_operands().size() == 1 ) {
            return ops[0].get_operands()[0];
        }        
    }

    assert( ops[0].get_type() != expression_tree::APPROXIMATE_NUMBER );

    return expression_tree::make_operator("Sin", ops);
}

expression_tree numeric_Sin(const expression_tree::operands_t& ops, enviroment& env) {
    if ( ops.size() != 1 ) {
        env.raise_error("numeric Sin", "called with invalid arguments");
        return expression_tree::make_operator("Sin", ops);
    }

    mpf_class arg;
    switch( ops[0].get_type() ) {
    case expression_tree::APPROXIMATE_NUMBER :
        arg = ops[0].get_approximate_number();
        break;
    case expression_tree::EXACT_NUMBER :
        arg = ops[0].get_exact_number();
        break;
    case expression_tree::SYMBOL :
    case expression_tree::OPERATOR :
        return expression_tree::make_operator("Sin", ops);
    default :
        assert( false && "default case invoked" );
        return expression_tree::make_operator("Sin", ops);
    }

    //arg contains the value;
    const double n = arg.get_d();
    return expression_tree::make_approximate_number( mpf_class( std::sin(n) ) );
}



expression_tree Cos(const expression_tree::operands_t& ops, enviroment& env) {
    if ( ops.size() != 1 ) {
        env.raise_error("Cos", "called with invalid arguments");
        return expression_tree::make_operator("Cos", ops);
    }

    assert( ops[0].get_type() != expression_tree::APPROXIMATE_NUMBER );

    if ( ops[0].get_type() == expression_tree::OPERATOR && ops[0].get_operator_name() == "ArcCos" ) {
        if( ops[0].get_operands().size() == 1 ) {
            return ops[0].get_operands()[0];
        }        
    }

    return expression_tree::make_operator("Cos", ops);
}

expression_tree numeric_Cos(const expression_tree::operands_t& ops, enviroment& env) {
    if ( ops.size() != 1 ) {
        env.raise_error("numeric Cos", "called with invalid arguments");
        return expression_tree::make_operator("Cos", ops);
    }

    mpf_class arg;
    switch( ops[0].get_type() ) {
    case expression_tree::APPROXIMATE_NUMBER :
        arg = ops[0].get_approximate_number();
        break;
    case expression_tree::EXACT_NUMBER :
        arg = ops[0].get_exact_number();
        break;
    case expression_tree::SYMBOL :
    case expression_tree::OPERATOR :
        return expression_tree::make_operator("Cos", ops);
    default :
        assert( false && "default case invoked" );
        return expression_tree::make_operator("Cos", ops);
    }

    //arg contains the value;
    const double n = arg.get_d();
    return expression_tree::make_approximate_number( mpf_class( std::cos(n) ) );
}



expression_tree Tan(const expression_tree::operands_t& ops, enviroment& env) {
    if ( ops.size() != 1 ) {
        env.raise_error("Tan", "called with invalid arguments");
        return expression_tree::make_operator("Tan", ops);
    }

    assert( ops[0].get_type() != expression_tree::APPROXIMATE_NUMBER );

    if ( ops[0].get_type() == expression_tree::OPERATOR && ops[0].get_operator_name() == "ArcTan" ) {
        if( ops[0].get_operands().size() == 1 ) {
            return ops[0].get_operands()[0];
        }        
    }

    return expression_tree::make_operator("Tan", ops);
}

expression_tree numeric_Tan(const expression_tree::operands_t& ops, enviroment& env) {
    if ( ops.size() != 1 ) {
        env.raise_error("numeric Tan", "called with invalid arguments");
        return expression_tree::make_operator("Tan", ops);
    }

    mpf_class arg;
    switch( ops[0].get_type() ) {
    case expression_tree::APPROXIMATE_NUMBER :
        arg = ops[0].get_approximate_number();
        break;
    case expression_tree::EXACT_NUMBER :
        arg = ops[0].get_exact_number();
        break;
    case expression_tree::SYMBOL :
    case expression_tree::OPERATOR :
        return expression_tree::make_operator("Tan", ops);
    default :
        assert( false && "default case invoked" );
        return expression_tree::make_operator("Tan", ops);
    }

    //arg contains the value;
    const double n = arg.get_d();
    return expression_tree::make_approximate_number( mpf_class( std::tan(n) ) );
}

expression_tree Cot(const expression_tree::operands_t& ops, enviroment& env) {
    if ( ops.size() != 1 ) {
        env.raise_error("Cot", "called with invalid arguments");
        return expression_tree::make_operator("Cot", ops);
    }

    assert( ops[0].get_type() != expression_tree::APPROXIMATE_NUMBER );

    return expression_tree::make_operator("Cot", ops);
}

expression_tree numeric_Cot(const expression_tree::operands_t& ops, enviroment& env) {
    if ( ops.size() != 1 ) {
        env.raise_error("numeric Cot", "called with invalid arguments");
        return expression_tree::make_operator("Cot", ops);
    }

    mpf_class arg;
    switch( ops[0].get_type() ) {
    case expression_tree::APPROXIMATE_NUMBER :
        arg = ops[0].get_approximate_number();
        break;
    case expression_tree::EXACT_NUMBER :
        arg = ops[0].get_exact_number();
        break;
    case expression_tree::SYMBOL :
    case expression_tree::OPERATOR :
        return expression_tree::make_operator("Cot", ops);
    default :
        assert( false && "default case invoked" );
        return expression_tree::make_operator("Cot", ops);
    }

    //arg contains the value;
    const double n = arg.get_d();

    //good infinity?
    const double tmp = std::tan(n);
    if ( tmp == 0. ) {
        return expression_tree::make_symbol( "Infinity" );
    } else {
        return expression_tree::make_approximate_number( 1. / tmp );
    }
}

expression_tree Sec(const expression_tree::operands_t& ops, enviroment& env) {
    if ( ops.size() != 1 ) {
        env.raise_error("Sec", "called with invalid arguments");
        return expression_tree::make_operator("Sec", ops);
    }

    assert( ops[0].get_type() != expression_tree::APPROXIMATE_NUMBER );

    return expression_tree::make_operator("Sec", ops);
}

expression_tree numeric_Sec(const expression_tree::operands_t& ops, enviroment& env) {
    if ( ops.size() != 1 ) {
        env.raise_error("numeric Sec", "called with invalid arguments");
        return expression_tree::make_operator("Sec", ops);
    }

    mpf_class arg;
    switch( ops[0].get_type() ) {
    case expression_tree::APPROXIMATE_NUMBER :
        arg = ops[0].get_approximate_number();
        break;
    case expression_tree::EXACT_NUMBER :
        arg = ops[0].get_exact_number();
        break;
    case expression_tree::SYMBOL :
    case expression_tree::OPERATOR :
        return expression_tree::make_operator("Sec", ops);
    default :
        assert( false && "default case invoked" );
        return expression_tree::make_operator("Sec", ops);
    }

    //arg contains the value;
    const double n = arg.get_d();
    return expression_tree::make_approximate_number( 1. / mpf_class( std::cos(n) ) );
}

expression_tree Csc(const expression_tree::operands_t& ops, enviroment& env) {
    if ( ops.size() != 1 ) {
        env.raise_error("Csc", "called with invalid arguments");
        return expression_tree::make_operator("Csc", ops);
    }

    assert( ops[0].get_type() != expression_tree::APPROXIMATE_NUMBER );

    return expression_tree::make_operator("Csc", ops);
}

expression_tree numeric_Csc(const expression_tree::operands_t& ops, enviroment& env) {
    if ( ops.size() != 1 ) {
        env.raise_error("numeric Csc", "called with invalid arguments");
        return expression_tree::make_operator("Csc", ops);
    }

    mpf_class arg;
    switch( ops[0].get_type() ) {
    case expression_tree::APPROXIMATE_NUMBER :
        arg = ops[0].get_approximate_number();
        break;
    case expression_tree::EXACT_NUMBER :
        arg = ops[0].get_exact_number();
        break;
    case expression_tree::SYMBOL :
    case expression_tree::OPERATOR :
        return expression_tree::make_operator("Csc", ops);
    default :
        assert( false && "default case invoked" );
        return expression_tree::make_operator("Csc", ops);
    }

    //arg contains the value;
    const double n = arg.get_d();
    return expression_tree::make_approximate_number( 1. / mpf_class( std::sin(n) ) );
}

expression_tree ArcSin(const expression_tree::operands_t& ops, enviroment& env) {
    if ( ops.size() != 1 ) {
        env.raise_error("ArcSin", "called with invalid arguments");
        return expression_tree::make_operator("ArcSin", ops);
    }

    assert( ops[0].get_type() != expression_tree::APPROXIMATE_NUMBER );

    if ( ops[0].get_type() == expression_tree::EXACT_NUMBER ) {
        const mpq_class& val = ops[0].get_exact_number();
        if ( val < -1 || val > 1 ) {
            env.raise_error("ArcSin", "Indeterminate expression encountered");
            return expression_tree::make_symbol("Indeterminate");
        }
    }

    return expression_tree::make_operator("ArcSin", ops);
}

expression_tree numeric_ArcSin(const expression_tree::operands_t& ops, enviroment& env) {
    if ( ops.size() != 1 ) {
        env.raise_error("numeric ArcSin", "called with invalid arguments");
        return expression_tree::make_operator("ArcSin", ops);
    }

    mpf_class arg;
    switch( ops[0].get_type() ) {
    case expression_tree::APPROXIMATE_NUMBER :
        arg = ops[0].get_approximate_number();
        break;
    case expression_tree::EXACT_NUMBER :
        arg = ops[0].get_exact_number();
        break;
    case expression_tree::SYMBOL :
    case expression_tree::OPERATOR :
        return expression_tree::make_operator("ArcSin", ops);
    default :
        assert( false && "default case invoked" );
        return expression_tree::make_operator("ArcSin", ops);
    }

    if ( arg < -1 || arg > 1 ) {
        env.raise_error("numeric ArcSin", "Indeterminate expression encountered");
        return expression_tree::make_symbol("Indeterminate");
    }

    //arg contains the value;
    const double n = arg.get_d();
    return expression_tree::make_approximate_number( mpf_class( std::asin(n) ) );
}

expression_tree ArcCos(const expression_tree::operands_t& ops, enviroment& env) {
    if ( ops.size() != 1 ) {
        env.raise_error("ArcCos", "called with invalid arguments");
        return expression_tree::make_operator("ArcCos", ops);
    }

    assert( ops[0].get_type() != expression_tree::APPROXIMATE_NUMBER );

    if ( ops[0].get_type() == expression_tree::EXACT_NUMBER ) {
        const mpq_class& val = ops[0].get_exact_number();
        if ( val < -1 || val > 1 ) {
            env.raise_error("ArcCos", "Indeterminate expression encountered");
            return expression_tree::make_symbol("Indeterminate");
        }
    }

    return expression_tree::make_operator("ArcCos", ops);
}

expression_tree numeric_ArcCos(const expression_tree::operands_t& ops, enviroment& env) {
    if ( ops.size() != 1 ) {
        env.raise_error("numeric ArcCos", "called with invalid arguments");
        return expression_tree::make_operator("ArcCos", ops);
    }

    mpf_class arg;
    switch( ops[0].get_type() ) {
    case expression_tree::APPROXIMATE_NUMBER :
        arg = ops[0].get_approximate_number();
        break;
    case expression_tree::EXACT_NUMBER :
        arg = ops[0].get_exact_number();
        break;
    case expression_tree::SYMBOL :
    case expression_tree::OPERATOR :
        return expression_tree::make_operator("ArcCos", ops);
    default :
        assert( false && "default case invoked" );
        return expression_tree::make_operator("ArcCos", ops);
    }

    if ( arg < -1 || arg > 1 ) {
        env.raise_error("numeric ArcCos", "Indeterminate expression encountered");
        return expression_tree::make_symbol("Indeterminate");
    }

    //arg contains the value;
    const double n = arg.get_d();
    return expression_tree::make_approximate_number( mpf_class( std::acos(n) ) );
}

expression_tree ArcTan(const expression_tree::operands_t& ops, enviroment& env) {
    if ( ops.size() != 1 ) {
        env.raise_error("ArcTan", "called with invalid arguments");
        return expression_tree::make_operator("ArcTan", ops);
    }

    assert( ops[0].get_type() != expression_tree::APPROXIMATE_NUMBER );

    return expression_tree::make_operator("ArcTan", ops);
}

expression_tree numeric_ArcTan(const expression_tree::operands_t& ops, enviroment& env) {
    if ( ops.size() != 1 ) {
        env.raise_error("numeric ArcTan", "called with invalid arguments");
        return expression_tree::make_operator("ArcTan", ops);
    }

    mpf_class arg;
    switch( ops[0].get_type() ) {
    case expression_tree::APPROXIMATE_NUMBER :
        arg = ops[0].get_approximate_number();
        break;
    case expression_tree::EXACT_NUMBER :
        arg = ops[0].get_exact_number();
        break;
    case expression_tree::SYMBOL :
    case expression_tree::OPERATOR :
        return expression_tree::make_operator("ArcTan", ops);
    default :
        assert( false && "default case invoked" );
        return expression_tree::make_operator("ArcTan", ops);
    }

    //arg contains the value;
    const double n = arg.get_d();
    return expression_tree::make_approximate_number( mpf_class( std::atan(n) ) );
}

expression_tree Sinh(const expression_tree::operands_t& ops, enviroment& env) {
    if ( ops.size() != 1 ) {
        env.raise_error("Sinh", "called with invalid arguments");
        return expression_tree::make_operator("Sinh", ops);
    }

    assert( ops[0].get_type() != expression_tree::APPROXIMATE_NUMBER );

    return expression_tree::make_operator("Sinh", ops);
}

expression_tree numeric_Sinh(const expression_tree::operands_t& ops, enviroment& env) {
    if ( ops.size() != 1 ) {
        env.raise_error("numeric Sinh", "called with invalid arguments");
        return expression_tree::make_operator("Sinh", ops);
    }

    mpf_class arg;
    switch( ops[0].get_type() ) {
    case expression_tree::APPROXIMATE_NUMBER :
        arg = ops[0].get_approximate_number();
        break;
    case expression_tree::EXACT_NUMBER :
        arg = ops[0].get_exact_number();
        break;
    case expression_tree::SYMBOL :
    case expression_tree::OPERATOR :
        return expression_tree::make_operator("Sinh", ops);
    default :
        assert( false && "default case invoked" );
        return expression_tree::make_operator("Sinh", ops);
    }

    //arg contains the value;
    const double n = arg.get_d();
    return expression_tree::make_approximate_number( mpf_class( std::sinh(n) ) );
}

expression_tree Cosh(const expression_tree::operands_t& ops, enviroment& env) {
    if ( ops.size() != 1 ) {
        env.raise_error("Cosh", "called with invalid arguments");
        return expression_tree::make_operator("Cosh", ops);
    }

    assert( ops[0].get_type() != expression_tree::APPROXIMATE_NUMBER );

    return expression_tree::make_operator("Cosh", ops);
}

expression_tree numeric_Cosh(const expression_tree::operands_t& ops, enviroment& env) {
    if ( ops.size() != 1 ) {
        env.raise_error("numeric Cosh", "called with invalid arguments");
        return expression_tree::make_operator("Cosh", ops);
    }

    mpf_class arg;
    switch( ops[0].get_type() ) {
    case expression_tree::APPROXIMATE_NUMBER :
        arg = ops[0].get_approximate_number();
        break;
    case expression_tree::EXACT_NUMBER :
        arg = ops[0].get_exact_number();
        break;
    case expression_tree::SYMBOL :
    case expression_tree::OPERATOR :
        return expression_tree::make_operator("Cosh", ops);
    default :
        assert( false && "default case invoked" );
        return expression_tree::make_operator("Cosh", ops);
    }

    //arg contains the value;
    const double n = arg.get_d();
    return expression_tree::make_approximate_number( mpf_class( std::cosh(n) ) );
}

expression_tree Tanh(const expression_tree::operands_t& ops, enviroment& env) {
    if ( ops.size() != 1 ) {
        env.raise_error("Tanh", "called with invalid arguments");
        return expression_tree::make_operator("Tanh", ops);
    }

    assert( ops[0].get_type() != expression_tree::APPROXIMATE_NUMBER );

    return expression_tree::make_operator("Tanh", ops);
}

expression_tree numeric_Tanh(const expression_tree::operands_t& ops, enviroment& env) {
    if ( ops.size() != 1 ) {
        env.raise_error("numeric Tanh", "called with invalid arguments");
        return expression_tree::make_operator("Tanh", ops);
    }

    mpf_class arg;
    switch( ops[0].get_type() ) {
    case expression_tree::APPROXIMATE_NUMBER :
        arg = ops[0].get_approximate_number();
        break;
    case expression_tree::EXACT_NUMBER :
        arg = ops[0].get_exact_number();
        break;
    case expression_tree::SYMBOL :
    case expression_tree::OPERATOR :
        return expression_tree::make_operator("Tanh", ops);
    default :
        assert( false && "default case invoked" );
        return expression_tree::make_operator("Tanh", ops);
    }

    //arg contains the value;
    const double n = arg.get_d();
    return expression_tree::make_approximate_number( mpf_class( std::tanh(n) ) );
}

expression_tree Coth(const expression_tree::operands_t& ops, enviroment& env) {
    if ( ops.size() != 1 ) {
        env.raise_error("Coth", "called with invalid arguments");
        return expression_tree::make_operator("Coth", ops);
    }

    assert( ops[0].get_type() != expression_tree::APPROXIMATE_NUMBER );

    return expression_tree::make_operator("Coth", ops);
}

expression_tree numeric_Coth(const expression_tree::operands_t& ops, enviroment& env) {
    if ( ops.size() != 1 ) {
        env.raise_error("numeric Coth", "called with invalid arguments");
        return expression_tree::make_operator("Coth", ops);
    }

    mpf_class arg;
    switch( ops[0].get_type() ) {
    case expression_tree::APPROXIMATE_NUMBER :
        arg = ops[0].get_approximate_number();
        break;
    case expression_tree::EXACT_NUMBER :
        arg = ops[0].get_exact_number();
        break;
    case expression_tree::SYMBOL :
    case expression_tree::OPERATOR :
        return expression_tree::make_operator("Coth", ops);
    default :
        assert( false && "default case invoked" );
        return expression_tree::make_operator("Coth", ops);
    }

    //arg contains the value;
    const double n = arg.get_d();
    return expression_tree::make_approximate_number( mpf_class( 1. / std::tanh(n) ) );
}

expression_tree Sech(const expression_tree::operands_t& ops, enviroment& env) {
    if ( ops.size() != 1 ) {
        env.raise_error("Sech", "called with invalid arguments");
        return expression_tree::make_operator("Sech", ops);
    }

    assert( ops[0].get_type() != expression_tree::APPROXIMATE_NUMBER );

    return expression_tree::make_operator("Sech", ops);
}

expression_tree numeric_Sech(const expression_tree::operands_t& ops, enviroment& env) {
    if ( ops.size() != 1 ) {
        env.raise_error("numeric Sech", "called with invalid arguments");
        return expression_tree::make_operator("Sech", ops);
    }

    mpf_class arg;
    switch( ops[0].get_type() ) {
    case expression_tree::APPROXIMATE_NUMBER :
        arg = ops[0].get_approximate_number();
        break;
    case expression_tree::EXACT_NUMBER :
        arg = ops[0].get_exact_number();
        break;
    case expression_tree::SYMBOL :
    case expression_tree::OPERATOR :
        return expression_tree::make_operator("Sech", ops);
    default :
        assert( false && "default case invoked" );
        return expression_tree::make_operator("Sech", ops);
    }

    //arg contains the value;
    const double n = arg.get_d();
    return expression_tree::make_approximate_number( mpf_class( 1. / std::cosh(n) ) );
}

expression_tree Csch(const expression_tree::operands_t& ops, enviroment& env) {
    if ( ops.size() != 1 ) {
        env.raise_error("Csch", "called with invalid arguments");
        return expression_tree::make_operator("Csch", ops);
    }

    assert( ops[0].get_type() != expression_tree::APPROXIMATE_NUMBER );

    return expression_tree::make_operator("Csch", ops);
}

expression_tree numeric_Csch(const expression_tree::operands_t& ops, enviroment& env) {
    if ( ops.size() != 1 ) {
        env.raise_error("numeric Csch", "called with invalid arguments");
        return expression_tree::make_operator("Csch", ops);
    }

    mpf_class arg;
    switch( ops[0].get_type() ) {
    case expression_tree::APPROXIMATE_NUMBER :
        arg = ops[0].get_approximate_number();
        break;
    case expression_tree::EXACT_NUMBER :
        arg = ops[0].get_exact_number();
        break;
    case expression_tree::SYMBOL :
    case expression_tree::OPERATOR :
        return expression_tree::make_operator("Csch", ops);
    default :
        assert( false && "default case invoked" );
        return expression_tree::make_operator("Csch", ops);
    }

    //arg contains the value;
    const double n = arg.get_d();
    return expression_tree::make_approximate_number( mpf_class( 1. / std::sinh(n) ) );
}

}} //namespace r0::builtin