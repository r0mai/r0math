
#include "builtin_findroot.hpp"

#include "enviroment.hpp"

namespace r0 { namespace builtin {

expression_tree FindRoot(const expression_tree::operands_t& ops, enviroment& env) {
    if ( ops.size() != 2 ) {
        env.raise_error("FindRoot", "called with invalid arguments");
        return expression_tree::make_operator("FindRoot", ops);
    }

    if ( !ops[1].is_operator("List") ) {
        env.raise_error("FindRoot", "List expected as second argument");
        return expression_tree::make_operator("FindRoot", ops);
    }

    bool success = false;
    mpf_class result;

    switch ( ops[1].get_operands().size() ) {
    default :
        env.raise_error("FindRoot", "list with 3 or 2 elements expected at position 2");
        return expression_tree::make_operator("FindRoot", ops);

    case 2 : //Newton method
        {
            const expression_tree& expr = ops[0];
            const expression_tree& variable_expr = ops[1].get_operands()[0];
            const expression_tree& x_expr = ops[1].get_operands()[1];

            if ( variable_expr.get_type() != expression_tree::SYMBOL ||
                !x_expr.is_number_type()  )
            {
                env.raise_error("FindRoot", "list with format {sym, num} expected at position 2");
                return expression_tree::make_operator("FindRoot", ops);
            }

            const std::string& variable = variable_expr.get_symbol();
            const mpf_class x = x_expr.get_number_as_mpf(); //x_(n-1)

            success = findroot_impl::newton_method(expr, env, variable, x, result);

        }
        break;

    case 3 : //Secant method
        {
            const expression_tree& expr = ops[0];
            const expression_tree& variable_expr = ops[1].get_operands()[0];
            const expression_tree& x1_expr = ops[1].get_operands()[1];
            const expression_tree& x2_expr = ops[1].get_operands()[2];

            if ( variable_expr.get_type() != expression_tree::SYMBOL ||
                !x1_expr.is_number_type() ||
                !x2_expr.is_number_type()  )
            {
                env.raise_error("FindRoot", "list with format {sym, num, num} expected at position 2");
                return expression_tree::make_operator("FindRoot", ops);
            }

            const std::string& variable = variable_expr.get_symbol();
            const mpf_class x1 = x1_expr.get_number_as_mpf(); //x_(n-1)
            const mpf_class x2 = x2_expr.get_number_as_mpf(); //x_(n-2)

            success = findroot_impl::secant_method(expr, env, variable, x1, x2, result);
        }
        break;
    }
    

    if ( success ) {
        return expression_tree::make_approximate_number(result);
    } else {
        return expression_tree::make_operator("FindRoot", ops);
    }    

}

namespace findroot_impl {

bool execute_function(expression_tree expr, enviroment& scoped_env, const std::string& var, const mpf_class& x, mpf_class& result) {

    scoped_env.get_symbols().set_variable( var, expression_tree::make_approximate_number(x) );
    expr.evaluate(scoped_env);
    
    assert(expr.get_type() != expression_tree::EXACT_NUMBER);

    if ( expr.get_type() == expression_tree::APPROXIMATE_NUMBER ) {
        result = expr.get_approximate_number();
        return true;
    }
    return false;

}

bool secant_method(
    const expression_tree& func,
    enviroment& env,
    const std::string& variable,
    mpf_class x1,
    mpf_class x2,
    mpf_class& result)
{

    enviroment scoped_env(env);

    //Secant method
    //x_n = x_(n-1)-f(x_(n-1)) * ( x_(n-1)-x_(n-2) )/( f(x_(n-1))-f(x_(n-2)) )

    mpf_class fx1; //f(x_(n-1))
    mpf_class fx2; //f(x_(n-2))

    if (!findroot_impl::execute_function(func, scoped_env, variable, x1, fx1) ||
        !findroot_impl::execute_function(func, scoped_env, variable, x2, fx2) ) 
    {
        return false;
    }

    const unsigned max_iterator = 100;    

    for ( unsigned i = 0; i < max_iterator; ++i ) {

        //std::cout << "i: " << i << " -> x1 = " << x1 << ", x2 = " << x2 << ", fx1 = " << fx1 << ", fx2 = " << fx2 << std::endl;

        if ( fx1 == 0 || fx1 == fx2 /*div by zero*/) {
            if (env.print_debug) {
                std::cout << "secant stopped @ " << i << ". iteration" << std::endl;
            }
            //close enough
            break;
        }

        mpf_class x = x1 - fx1 * ( x1-x2 )/( fx1 - fx2 );
        mpf_class fx;

        if ( !findroot_impl::execute_function(func, scoped_env, variable, x, fx) ) {
            return false;
        }

        x2 = x1;
        x1 = x;

        fx2 = fx1;
        fx1 = fx;


    }

    result = x1;

    return true;
}

bool newton_method(
    const expression_tree& func, 
    enviroment& env,
    const std::string& variable, 
    mpf_class x1, //x_(n-1)
    mpf_class& result) 
{
    
    enviroment scoped_env(env);

    //Newton–Raphson method
    //x_n = x_(n-1)- f(x_(n-1))/f'(x_(n-1))

    const expression_tree dfunc = expression_tree::make_operator("D", func, expression_tree::make_symbol( variable ) ).evaluate(env);

    mpf_class fx1; //f(x_(n-1))
    mpf_class fdx1; //f'(x_(n-1))

    if (!findroot_impl::execute_function(func, scoped_env, variable, x1, fx1) ||
        !findroot_impl::execute_function(dfunc, scoped_env, variable, x1, fdx1) ) 
    {
        return false;
    }

    const unsigned max_iterator = 100;

    for ( unsigned i = 0; i < max_iterator; ++i ) {

        const mpf_class x = x1 - fx1/fdx1;
        mpf_class fx;
        mpf_class fdx;

        if (!findroot_impl::execute_function(func, scoped_env, variable, x, fx) ||
            !findroot_impl::execute_function(dfunc, scoped_env, variable, x, fdx) ) 
        {
            return false;
        }

        x1 = x;
        fx1 = fx;
        fdx1 = fdx;

        const mpf_class epsilon = 1.e-10;

        if ( fx1 > -epsilon && fx1 < epsilon ) {
            if ( env.print_debug ) {
                std::cout << "newton stopped @ " << i << ". iteration" << std::endl;
            }
            //close enough
            break;
        }

    }

    result = x1;
    return true;

}

} //findroot_impl

}} //namespace r0::builtin