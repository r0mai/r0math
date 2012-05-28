
#include "builtin_d.hpp"

#include <cassert>
#include <boost/assign.hpp>

#include "enviroment.hpp"

namespace r0 { namespace builtin {

expression_tree D(const expression_tree::operands_t& ops, enviroment& env) {
    if ( ops.size() != 2 ) {
        env.raise_error("D", "called with invalid arguments");
        return expression_tree::make_operator("D", ops);
    }

    

    switch( ops[1].get_type() ) {
    case expression_tree::SYMBOL :
        {

            return d_impl::derive( ops[0], ops[1].get_symbol(), env );
        }
        break;
    case expression_tree::OPERATOR :
        {
            if ( ops[1].get_operator_name() != "List" ) {
                env.raise_error("D", "variable or list expected at position 1");
                return expression_tree::make_operator("D", ops);
            }

            const expression_tree::operands_t& param_ops = ops[1].get_operands();

            if ( param_ops.size() != 2 ) {
                env.raise_error("D", "list size 2 expected position 1");
                return expression_tree::make_operator("D", ops);
            }

            if ( param_ops[0].get_type() != expression_tree::SYMBOL || 
                param_ops[1].get_type() != expression_tree::EXACT_NUMBER ||
                !is_mpq_integer(param_ops[1].get_exact_number()) || 
                param_ops[1].get_exact_number() < 0 ) 
            {
                env.raise_error("D", "list in the form {symbol, non_negative_int} expected");
                return expression_tree::make_operator("D", ops);
            }

            unsigned long res = 0;

            if ( !mpz_get_ui_checked(res, param_ops[1].get_exact_number().get_num() ) ) {
                env.raise_error("D", "integer overflow");
                return expression_tree::make_operator("D", ops);
            }

            const std::string& var = param_ops[0].get_symbol();
            expression_tree func = ops[0];
            
            for ( unsigned long i = 0; i < res; ++i ) {
                func = d_impl::derive(func, var, env);
            }
            return func;
        }
        break;
    case expression_tree::APPROXIMATE_NUMBER :
    case expression_tree::EXACT_NUMBER :
        env.raise_error("D", "variable or list expected at position 1");
        return expression_tree::make_operator("D", ops);
        break;
    default:
        assert( false && "default case invoked" );
        return expression_tree::make_invalid();
        break;
    }

}

namespace d_impl {

derive_functions_t build_derive_functions() {
    derive_functions_t res;

    res.insert( derive_functions_t::value_type( "Plus", &derive_Plus ) );
    res.insert( derive_functions_t::value_type( "Times", &derive_Times ) );
    res.insert( derive_functions_t::value_type( "Power", &derive_Power ) );
    res.insert( derive_functions_t::value_type( "Log", &derive_Log ) );
    res.insert( derive_functions_t::value_type( "Sin", &derive_Sin ) );
    res.insert( derive_functions_t::value_type( "Cos", &derive_Cos ) );
    res.insert( derive_functions_t::value_type( "Tan", &derive_Tan ) );
    res.insert( derive_functions_t::value_type( "Cot", &derive_Cot ) );
    res.insert( derive_functions_t::value_type( "Sec", &derive_Sec ) );
    res.insert( derive_functions_t::value_type( "Csc", &derive_Csc ) );
    res.insert( derive_functions_t::value_type( "ArcSin", &derive_ArcSin ) );
    res.insert( derive_functions_t::value_type( "ArcCos", &derive_ArcCos ) );
    res.insert( derive_functions_t::value_type( "ArcTan", &derive_ArcTan ) );
    res.insert( derive_functions_t::value_type( "Sinh", &derive_Sinh ) );
    res.insert( derive_functions_t::value_type( "Cosh", &derive_Cosh ) );
    res.insert( derive_functions_t::value_type( "Tanh", &derive_Tanh ) );
    res.insert( derive_functions_t::value_type( "Coth", &derive_Coth ) );
    res.insert( derive_functions_t::value_type( "Sech", &derive_Sech ) );
    res.insert( derive_functions_t::value_type( "Csch", &derive_Csch ) );

    return res;
}

expression_tree derive(const expression_tree& expr, const std::string& var, enviroment& env) {

    static derive_functions_t derive_functions = build_derive_functions();


    switch (expr.get_type()) {
    default:
        assert( false && "default case invoked" );
        return expression_tree::make_invalid();

    case expression_tree::SYMBOL :
        if ( expr.get_symbol() == var ) {
            return expression_tree::make_exact_number( 1 );
        } else { //constant
            return expression_tree::make_exact_number( 0 );
        }

    case expression_tree::EXACT_NUMBER :
    case expression_tree::APPROXIMATE_NUMBER :
        return expression_tree::make_exact_number( 0 );

    case expression_tree::OPERATOR :
        {

            const std::string& op_name = expr.get_operator_name();
            const expression_tree::operands_t ops = expr.get_operands();

            derive_functions_t::iterator it = derive_functions.find( op_name );

            if ( it != derive_functions.end() ) {
                //calls derive_<op_name>()
                if (env.print_debug) {
                    std::cout << "deriving " << it->first << std::endl;
                }
                return (*it->second)(ops, var, env);
            } else {
                //chain rule maybe?
                return expression_tree::make_operator( "D", expr, expression_tree::make_symbol(var) );
            }

        }
    }
    //never reach
}

expression_tree derive_Plus(const expression_tree::operands_t& ops, const std::string& var, enviroment& env) {
    //(f + g)' = f' + g'

    expression_tree::operands_t result_ops( ops.size() );
    for ( unsigned i = 0; i < ops.size(); ++i ) {
        result_ops[i] = derive( ops[i], var, env );
    }
    return expression_tree::make_operator( "Plus", result_ops ).evaluate(env);
}

expression_tree derive_Times(const expression_tree::operands_t& ops, const std::string& var, enviroment& env) {
    //(f g h ...)' = f' g h + f g' h + f g h' + ...

    expression_tree::operands_t plus_ops( ops.size() );
    for ( unsigned i = 0; i < ops.size(); ++i ) {

        expression_tree::operands_t times_ops( ops.size() );

        times_ops[0] = derive( ops[i], var, env );

        //c runs from 1 -> n
        //j runs form 0 -> n skipping i
        for ( unsigned j = 0, c = 1; j < ops.size(); ++j) {
            if ( j != i ) {
                times_ops[c++] = ops[j];
            }
        }
        plus_ops[i] = expression_tree::make_operator( "Times", times_ops );

    }
    return expression_tree::make_operator( "Plus", plus_ops ).evaluate(env);
}

expression_tree derive_Power(const expression_tree::operands_t& ops, const std::string& var, enviroment& env) {
    //(a^b)' = a^b ((b Dt[a])/a+Dt[b] Log[a])
    assert( ops.size() == 2 );

    const expression_tree& a = ops[0];
    const expression_tree b = ops[1];

    const expression_tree dta = derive(a, var, env);
    const expression_tree dtb = derive(b, var, env);

    return 
    expression_tree::make_operator( "Times",
        expression_tree::make_operator( "Plus",
            expression_tree::make_operator( "Times", boost::assign::list_of(
                dta)(
                b)(
                expression_tree::make_operator( "Power",
                    a,
                    expression_tree::make_exact_number( mpq_class(-1, 1) )
                ))
            ),
            expression_tree::make_operator( "Times",
                dtb,
                expression_tree::make_operator( "Log",
                    a
                )
            )
        ),
        expression_tree::make_operator( "Power",
            a,
            b
        )
    ).evaluate(env);

}

expression_tree derive_Log(const expression_tree::operands_t& ops, const std::string& var, enviroment& env) {
    //assuming no two argument version reaches here
    assert( ops.size() == 1 );

    //Log[x]' = x'/x
    const expression_tree& x = ops[0];
    const expression_tree dx = derive(x, var, env);

    return expression_tree::make_operator( "Times", dx,
        expression_tree::make_operator( "Power", x,
            expression_tree::make_exact_number( mpq_class(-1, 1) )
        )
    ).evaluate(env);

}

expression_tree derive_Sin(const expression_tree::operands_t& ops, const std::string& var, enviroment& env) {

    assert( ops.size() == 1 );    

    //Sin[x]' = x'Cos[x]
    const expression_tree& x = ops[0];
    const expression_tree dx = derive(x, var, env);

    return 
    expression_tree::make_operator( "Times", 
        dx,
        expression_tree::make_operator( "Cos", x)
    ).evaluate(env);
}

expression_tree derive_Cos(const expression_tree::operands_t& ops, const std::string& var, enviroment& env) {

    assert( ops.size() == 1 ); 

    //Cos[x]' = -x'Cos[x]
    const expression_tree& x = ops[0];
    const expression_tree dx = derive(x, var, env);

    return
    expression_tree::make_operator( "Times", boost::assign::list_of(
        expression_tree::make_exact_number( mpq_class(-1, 1) ))(
        dx)(
        expression_tree::make_operator( "Sin",
            x
        ))
    ).evaluate(env);
}

expression_tree derive_Tan(const expression_tree::operands_t& ops, const std::string& var, enviroment& env) {
    assert( ops.size() == 1 ); 

    //Tan[x]' = x' Sec[x]^2
    const expression_tree& x = ops[0];
    const expression_tree dx = derive(x, var, env);

    return
    expression_tree::make_operator( "Times",
        dx,
        expression_tree::make_operator( "Power",
            expression_tree::make_operator( "Sec",
                x
            ),
            expression_tree::make_exact_number( mpq_class(2, 1) )
        )
    ).evaluate(env);
}

expression_tree derive_Cot(const expression_tree::operands_t& ops, const std::string& var, enviroment& env) {
    assert( ops.size() == 1 ); 

    //Cot[x]' = -x' Csc[x]^2
    const expression_tree& x = ops[0];
    const expression_tree dx = derive(x, var, env);

    return
    expression_tree::make_operator( "Times", boost::assign::list_of(
        expression_tree::make_exact_number( mpq_class(-1, 1) ))(
        dx)(
        expression_tree::make_operator( "Power",
            expression_tree::make_operator( "Csc",
                x
            ),
            expression_tree::make_exact_number( mpq_class(2, 1) )
        ))
    ).evaluate(env);
}

expression_tree derive_Sec(const expression_tree::operands_t& ops, const std::string& var, enviroment& env) {
    assert( ops.size() == 1 ); 

    //Sec[x]' = x' Sec[x] Tan[x]
    const expression_tree& x = ops[0];
    const expression_tree dx = derive(x, var, env);

    return
    expression_tree::make_operator( "Times", boost::assign::list_of(
        dx)(
        expression_tree::make_operator( "Sec",
            x
        ))(
        expression_tree::make_operator( "Tan",
            x
        ))
    ).evaluate(env);
}

expression_tree derive_Csc(const expression_tree::operands_t& ops, const std::string& var, enviroment& env) {
    assert( ops.size() == 1 ); 

    //Csc[x]' = -x' Cot[x] Csc[x]
    const expression_tree& x = ops[0];
    const expression_tree dx = derive(x, var, env);

    return
    expression_tree::make_operator( "Times", boost::assign::list_of(
        expression_tree::make_exact_number( mpq_class(-1, 1) ))(
        dx)(
        expression_tree::make_operator( "Cot",
            x
        ))(
        expression_tree::make_operator( "Csc",
            x
        ))
    ).evaluate(env);
}

expression_tree derive_ArcSin(const expression_tree::operands_t& ops, const std::string& var, enviroment& env) {
    assert( ops.size() == 1 ); 

    //ArcSin[x]' = dx/Sqrt[1 - x^2]
    const expression_tree& x = ops[0];
    const expression_tree dx = derive(x, var, env);

    return
    expression_tree::make_operator( "Times",
        dx,
        expression_tree::make_operator( "Power",
            expression_tree::make_operator( "Power",
                expression_tree::make_operator( "Plus",
                    expression_tree::make_exact_number( mpq_class(1, 1) ),
                    expression_tree::make_operator( "Times",
                        expression_tree::make_exact_number( mpq_class(-1, 1) ),
                        expression_tree::make_operator( "Power",
                            x,
                            expression_tree::make_exact_number( mpq_class(2, 1) )
                        )
                    )
                ),
                expression_tree::make_exact_number( mpq_class(1, 2) )
            ),
            expression_tree::make_exact_number( mpq_class(-1, 1) )
        )
    ).evaluate(env);
}

expression_tree derive_ArcCos(const expression_tree::operands_t& ops, const std::string& var, enviroment& env) {
    assert( ops.size() == 1 ); 

    //ArcCos[x]' = -dx/Sqrt[1-x^2]
    const expression_tree& x = ops[0];
    const expression_tree dx = derive(x, var, env);

    return 
    expression_tree::make_operator( "Times", boost::assign::list_of(
        expression_tree::make_exact_number( mpq_class(-1, 1) ))(
        dx)(
        expression_tree::make_operator( "Power",
            expression_tree::make_operator( "Power",
                expression_tree::make_operator( "Plus",
                    expression_tree::make_exact_number( mpq_class(1, 1) ),
                    expression_tree::make_operator( "Times",
                        expression_tree::make_exact_number( mpq_class(-1, 1) ),
                        expression_tree::make_operator( "Power",
                            x,
                            expression_tree::make_exact_number( mpq_class(2, 1) )
                        )
                    )
                ),
                expression_tree::make_exact_number( mpq_class(1, 2) )
            ),
            expression_tree::make_exact_number( mpq_class(-1, 1) )
        ))
    ).evaluate(env);
}

expression_tree derive_ArcTan(const expression_tree::operands_t& ops, const std::string& var, enviroment& env) {
    assert( ops.size() == 1 ); 

    //ArcTan[x]' = dx/(1+x^2)
    const expression_tree& x = ops[0];
    const expression_tree dx = derive(x, var, env);

    return 
    expression_tree::make_operator( "Times",
        dx,
        expression_tree::make_operator( "Power",
            expression_tree::make_operator( "Plus",
                expression_tree::make_exact_number( mpq_class(1, 1) ),
                expression_tree::make_operator( "Power",
                    x,
                    expression_tree::make_exact_number( mpq_class(2, 1) )
                )
            ),
            expression_tree::make_exact_number( mpq_class(-1, 1) )
        )
    ).evaluate(env);
}

expression_tree derive_Sinh(const expression_tree::operands_t& ops, const std::string& var, enviroment& env) {
    assert( ops.size() == 1 ); 

    //Sinh[x]' = dx Cosh[x]
    const expression_tree& x = ops[0];
    const expression_tree dx = derive(x, var, env);

    return 
    expression_tree::make_operator( "Times", 
        dx,
        expression_tree::make_operator( "Cosh", x)
    ).evaluate(env);
}

expression_tree derive_Cosh(const expression_tree::operands_t& ops, const std::string& var, enviroment& env) {
    assert( ops.size() == 1 ); 

    //Cosh[x]' = dx Sinh[x]
    const expression_tree& x = ops[0];
    const expression_tree dx = derive(x, var, env);

    return
    expression_tree::make_operator( "Times", 
        dx,
        expression_tree::make_operator( "Sinh", x)
    ).evaluate(env);
}

expression_tree derive_Tanh(const expression_tree::operands_t& ops, const std::string& var, enviroment& env) {
    assert( ops.size() == 1 ); 

    //Tanh[x]' = x' Sech[x]^2
    const expression_tree& x = ops[0];
    const expression_tree dx = derive(x, var, env);

    return
    expression_tree::make_operator( "Times",
        dx,
        expression_tree::make_operator( "Power",
            expression_tree::make_operator( "Sech",
                x
            ),
            expression_tree::make_exact_number( mpq_class(2, 1) )
        )
    ).evaluate(env);
}

expression_tree derive_Coth(const expression_tree::operands_t& ops, const std::string& var, enviroment& env) {
    assert( ops.size() == 1 ); 

    //Coth[x]' = -x' Csch[x]^2
    const expression_tree& x = ops[0];
    const expression_tree dx = derive(x, var, env);

    return
    expression_tree::make_operator( "Times", boost::assign::list_of(
        expression_tree::make_exact_number( mpq_class(-1, 1) ))(
        dx)(
        expression_tree::make_operator( "Power",
            expression_tree::make_operator( "Csch",
                x
            ),
            expression_tree::make_exact_number( mpq_class(2, 1) )
        ))
    ).evaluate(env);
}

expression_tree derive_Sech(const expression_tree::operands_t& ops, const std::string& var, enviroment& env) {
    assert( ops.size() == 1 ); 

    //Sech[x]' = -x' Sech[x] Tanh[x]
    const expression_tree& x = ops[0];
    const expression_tree dx = derive(x, var, env);

    return
    expression_tree::make_operator( "Times", boost::assign::list_of(
        dx)(
        expression_tree::make_exact_number( mpq_class(-1, 1) ))(
        expression_tree::make_operator( "Sech",
            x
        ))(
        expression_tree::make_operator( "Tanh",
            x
        ))
    ).evaluate(env);
}

expression_tree derive_Csch(const expression_tree::operands_t& ops, const std::string& var, enviroment& env) {
    assert( ops.size() == 1 ); 

    //Csch[x]' = -x' Coth[x] Csch[x]
    const expression_tree& x = ops[0];
    const expression_tree dx = derive(x, var, env);

    return
    expression_tree::make_operator( "Times", boost::assign::list_of(
        dx)(
        expression_tree::make_exact_number( mpq_class(-1, 1) ))(
        expression_tree::make_operator( "Coth",
            x
        ))(
        expression_tree::make_operator( "Csch",
            x
        ))
    ).evaluate(env);
}

} //namespace d_impl

}} //namespace r0::builtin