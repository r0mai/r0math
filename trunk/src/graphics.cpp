
#include "graphics.hpp"

#include <cassert>
#include <ctime>

#include <boost/lexical_cast.hpp>
#include <boost/array.hpp>

#include "expression_tree.hpp"
#include "graphics_object.hpp"
#include "enviroment.hpp"
#include "function_options.hpp"
#include "util.hpp"

namespace r0 {

//change this to take only operands_t
std::string expression_tree_to_graphics(const expression_tree& expr, enviroment& env) {
    assert( expr.get_type() == expression_tree::OPERATOR && expr.get_operator_name() == "Graphics" );

    if (env.print_debug) {
        std::cout << "\ngraphicsfull : " << expr.to_inputform(env) << std::endl;
    }

    function_options funcops;

    //default values
    funcops.set_option("Axes", expression_tree::make_symbol("False"));
    funcops.set_option("AxesOrigin", expression_tree::make_operator("List", 
        expression_tree::make_exact_number(0), expression_tree::make_exact_number(0)));
    
    funcops.set_option("ImageSize", expression_tree::make_operator("List",
        expression_tree::make_exact_number(640), expression_tree::make_exact_number(640)));
    funcops.set_option("PlotRange", //{{-1,1},{-1,1}}
        expression_tree::make_operator("List",
            expression_tree::make_operator("List",
                expression_tree::make_exact_number(-1),
                expression_tree::make_exact_number(1)),
            expression_tree::make_operator("List",
                expression_tree::make_exact_number(-1),
                expression_tree::make_exact_number(1))
            )
        );
    //end default values


    expression_tree::operands_t ops = funcops.get_options_from_argument_list( expr.get_operands() );

    bool axes = false;

    const bool axes_success = graphics_impl::pick_out_boolean(funcops.get_option("Axes"), axes);

    if (!axes_success) {
        env.raise_error("output Graphics", "option Axes format error");
        return "-- Graphics -- (error)";
    }

    mpf_class axes_origin_x;
    mpf_class axes_origin_y;

    const bool axes_origin_success = graphics_impl::extract_point(funcops.get_option("AxesOrigin"), axes_origin_x, axes_origin_y);

    if (!axes_origin_success) {
        env.raise_error("output Graphics", "option AxesOrigin format error");
        return "-- Graphics -- (error)";
    }

    unsigned long image_size_width = 0;
    unsigned long image_size_height = 0;

    const bool image_size_success = graphics_impl::pick_out_image_size(funcops.get_option("ImageSize"), image_size_width, image_size_height);

    if (!image_size_success) {
        env.raise_error("output Graphics", "option ImageSize format error");
        return "-- Graphics -- (error)";
    }

    mpf_class plot_range_x_min;
    mpf_class plot_range_x_max;
    mpf_class plot_range_y_min;
    mpf_class plot_range_y_max;

    const bool plot_range_success = 
        graphics_impl::pick_out_plot_range(funcops.get_option("PlotRange"),
        plot_range_x_min,
        plot_range_x_max,
        plot_range_y_min,
        plot_range_y_max);

    if (!plot_range_success) {
        env.raise_error("output Graphics", "option PlotRange format error");
        return "-- Graphics -- (error)";
    }

    graphics_object obj;

    obj.set_image_width(image_size_width);
    obj.set_image_height(image_size_height);

    obj.set_plot_range(plot_range_x_min, plot_range_x_max, plot_range_y_min, plot_range_y_max);

    obj.set_current_color(0x000000FF);

    //Parsing the graphics argument
    graphics_impl::collect_primitives( ops, obj, env );
    
    //drawing Axes
    if (axes) {

        

        //if x_origin is in range
        if ( axes_origin_x > plot_range_x_min && axes_origin_x < plot_range_x_max ) {
            obj.add_line( axes_origin_x, plot_range_y_min, axes_origin_x, plot_range_y_max, 0x000000FF );

            //drawing ticks
#if 0   
            
            const mpf_class plot_x_range = plot_range_x_max - plot_range_x_min;

            const unsigned tick_y_size = round_half_up( plot_x_range.get_d() / 10.0 );

            const long min_y_tick = std::ceil(plot_range_y_min.get_d());
            const long max_y_tick = std::floor(plot_range_y_max.get_d());

            std::cout << min_y_tick << ", " << max_y_tick << std::endl;

            for ( long tick_c = min_y_tick; tick_c <= max_y_tick; ++tick_c ) {
                std::cout << axes_origin_x - tick_y_size << ", " << tick_c << ", " << axes_origin_x + tick_y_size << ", " << tick_c << std::endl;

                obj.add_line( axes_origin_x - tick_y_size, tick_c, axes_origin_x + tick_y_size, tick_c, 0x000000FF );
            }
#endif

        }
        //if y_origin is in range
        if ( axes_origin_y > plot_range_y_min && axes_origin_y < plot_range_y_max ) {
            obj.add_line( plot_range_x_min, axes_origin_y, plot_range_x_max, axes_origin_y, 0x000000FF );
        }
    }

    std::string file_name = "graphics\\graphics_";
    file_name += boost::lexical_cast<std::string>(time(0));
    file_name += "_";
    file_name += boost::lexical_cast<std::string>(clock());
    file_name += ".bmp";

    obj.to_bmp_image().output_to_file(file_name);

    if (env.print_debug) {
        std::cout << "ops:\n" << funcops.to_string() << std::endl;
    }

    std::string ret_val = "-- Graphics -- (";
    ret_val += file_name;
    ret_val += ")";

    return ret_val;
}

namespace graphics_impl {

bool pick_out_boolean(const expression_tree& expr, bool& result) {
    if ( expr.get_type() != expression_tree::SYMBOL ) {
        return false;
    }
    if ( expr.get_symbol() == "True" ) {
        result = true;
        return true;
    } else if ( expr.get_symbol() == "False" ) {
        result = false;
        return true;
    } else {
        return false;
    }
}

bool pick_out_image_size(const expression_tree& expr, unsigned long& width_res, unsigned long& height_res) {
    if ( expr.get_type() != expression_tree::OPERATOR || 
        expr.get_operator_name() != "List" || 
        expr.get_operands().size() != 2) {
            return false;
    }

    const expression_tree::operands_t& ops = expr.get_operands();

    if ( ops[0].get_type() != expression_tree::EXACT_NUMBER || 
        !is_mpq_integer(ops[0].get_exact_number()) ||
        ops[1].get_type() != expression_tree::EXACT_NUMBER || 
        !is_mpq_integer(ops[1].get_exact_number()) ) {
            return false;
    }
    
    bool width_success = mpz_get_ui_checked(width_res, ops[0].get_exact_number().get_num());
    bool height_success = mpz_get_ui_checked(height_res, ops[1].get_exact_number().get_num());

    return width_success && height_success;
}

bool pick_out_plot_range(const expression_tree& expr, mpf_class& x_min, mpf_class& x_max, mpf_class& y_min, mpf_class& y_max) {
    if ( expr.get_type() != expression_tree::OPERATOR || 
        expr.get_operator_name() != "List" || 
        expr.get_operands().size() != 2) {
            return false;
    }
    
    return extract_point( expr.get_operands()[0], x_min, x_max ) &&
            extract_point( expr.get_operands()[1], y_min, y_max );
}

bool extract_point(const expression_tree& expr, mpf_class& min, mpf_class& max) {
    if ( expr.get_type() != expression_tree::OPERATOR || 
        expr.get_operator_name() != "List" || 
        expr.get_operands().size() != 2) {
            return false;
    }

    const expression_tree::operands_t& ops = expr.get_operands();

    if ( (ops[0].get_type() != expression_tree::APPROXIMATE_NUMBER &&
        ops[0].get_type() != expression_tree::EXACT_NUMBER) ||
        
        (ops[1].get_type() != expression_tree::APPROXIMATE_NUMBER &&
        ops[1].get_type() != expression_tree::EXACT_NUMBER) ) {

            return false;
    }

    min = ops[0].get_number_as_mpf();
    max = ops[1].get_number_as_mpf();

    return true;
}

void collect_primitives(const expression_tree::operands_t& ops, graphics_object& obj, enviroment& env) {

    rgba_color_t color_before = obj.get_current_color();

    for ( unsigned i = 0; i < ops.size(); ++i ) {
        if ( ops[i].get_type() == expression_tree::OPERATOR ) {

            const std::string& op_name = ops[i].get_operator_name();
            const expression_tree::operands_t& current_ops = ops[i].get_operands();

            if ( op_name == "List" ) {
                collect_primitives(current_ops, obj, env);
            } else if ( op_name == "Line" ) {
                collect_line_primitve(current_ops, obj, env);
            } else if ( op_name == "Circle" || op_name == "Disk" ) {
                collect_circle_disk_primitve(current_ops, obj, env, op_name);            
            } else if ( op_name == "Point" ) {
                collect_point_primitve(current_ops, obj, env);
            } else if ( op_name == "Rectangle" ) {
                collect_rectangle_primitve(current_ops, obj, env);
            } else if ( op_name == "RGBColor" ) {
                collect_rgbcolor(current_ops, obj, env);
            } else if ( op_name == "Hue" ) {
                collect_hue(current_ops, obj, env);
            } else {
                env.raise_error("collect_primitives", "Primitive not found : \"" + op_name + "\"");
            }
        }
    }

    //restoring the color
    obj.set_current_color(color_before);
}

void collect_line_primitve(const expression_tree::operands_t& ops, graphics_object& obj, enviroment& env) {
    if ( ops.size() != 1 ) {
        env.raise_error("graphics Line", "invalid line format, 1 argument expected");
        return;
    }
    
    const expression_tree& arg = ops[0];
    
    if ( arg.get_type() != expression_tree::OPERATOR || arg.get_operator_name() != "List" ) {
        env.raise_error("graphics Line", "invalid line format, list expected");
        return;
    }
    
    const expression_tree::operands_t& line_elements = arg.get_operands();

    if ( line_elements.size() < 2 ) {
        env.raise_error("graphics Line", "point list too small");
        return;
    }

    std::vector< std::pair<mpf_class, mpf_class> > points(line_elements.size());

    for (unsigned i = 0; i < line_elements.size(); ++i) {
        const expression_tree& line_point = line_elements[i];


        mpf_class x, y;
        bool extract_point_success = extract_point(line_point, x, y);

        if ( !extract_point_success ) {
            env.raise_error("graphics Line", "invalid point format");
            return;
        }

        points[i] = std::make_pair(x, y);

    }

    for (unsigned i = 0; i < points.size()-1; ++i) {
        obj.add_line( points[i].first, points[i].second, points[i+1].first, points[i+1].second, obj.get_current_color() );
    }
}

void collect_circle_disk_primitve(const expression_tree::operands_t& ops, graphics_object& obj, enviroment& env, const std::string& head) {
    assert( head == "Disk" || head == "Circle" );

    if ( ops.size() > 2 ) {
        env.raise_error("graphics Circle/Disk", "invalid circle format");
        return;
    }

    mpf_class x = 0;
    mpf_class y = 0;
    mpf_class r = 1;

    if ( ops.size() != 0 ) { //Circle[] -> x=0, y=0, r=1

        bool extraction_success = extract_point( ops[0], x, y );

        if ( !extraction_success ) {
            env.raise_error("graphics Circle/Disk", "invalid circle format");
            return;
        }
        if ( ops.size() == 2 ) {
            if ( ops[1].get_type() == expression_tree::APPROXIMATE_NUMBER || ops[1].get_type() == expression_tree::EXACT_NUMBER ) {
                r = ops[1].get_number_as_mpf();
            }
        }
    }

    if (head == "Circle") {
        obj.add_circle( x, y, r, obj.get_current_color() );
    } else {
        assert( head == "Disk" );
        obj.add_disk( x, y, r, obj.get_current_color() );
    }
}

void collect_point_primitve(const expression_tree::operands_t& ops, graphics_object& obj, enviroment& env) {
    if ( ops.size() != 1 ) {
        env.raise_error("graphics Point", "invalid Point format, 1 argument expected");
        return;
    }
    
    const expression_tree& arg = ops[0];
    
    if ( arg.get_type() != expression_tree::OPERATOR || arg.get_operator_name() != "List" ) {
        env.raise_error("graphics Point", "invalid Point format, list expected");
        return;
    }
    
    const expression_tree::operands_t& point_elements = arg.get_operands();

    std::vector< std::pair<mpf_class, mpf_class> > points(point_elements.size());

    for (unsigned i = 0; i < point_elements.size(); ++i) {
        const expression_tree& point = point_elements[i];

        mpf_class x, y;
        bool extract_point_success = extract_point(point, x, y);

        if ( !extract_point_success ) {
            env.raise_error("graphics Point", "invalid point format");
            return;
        }

        points[i] = std::make_pair(x, y);

    }

    for (unsigned i = 0; i < points.size(); ++i) {
        obj.add_point( points[i].first, points[i].second, obj.get_current_color() );
    }
    /*if ( ops.size() != 2 ) {
        env.raise_error("graphics Point", "invalid Point format");
        return;
    }
    if ( (ops[0].get_type() != expression_tree::APPROXIMATE_NUMBER && ops[0].get_type() != expression_tree::EXACT_NUMBER) ||
        (ops[1].get_type() != expression_tree::APPROXIMATE_NUMBER && ops[1].get_type() != expression_tree::EXACT_NUMBER)) {
        env.raise_error("graphics Point", "invalid Point format");
        return;
    }
    mpf_class x = ops[0].get_number_as_mpf();
    mpf_class y = ops[1].get_number_as_mpf();

    obj.add_point(x, y, obj.get_current_color());*/
}

void collect_rectangle_primitve(const expression_tree::operands_t& ops, graphics_object& obj, enviroment& env) {

    if (ops.size() > 2) {
        env.raise_error("graphics Rectangle", "invalid form");
        return;
    }

    if (ops.size() == 0) {
        obj.add_rectangle(0, 0, 1, 1, obj.get_current_color());
    } else if (ops.size() == 1) {

        mpf_class x_min, y_min;
        bool point_extract_success = extract_point(ops[0], x_min, y_min);

        if (!point_extract_success) {
            env.raise_error("graphics Rectangle", "invalid Point form");
            return;
        }
        obj.add_rectangle(x_min, y_min, x_min+1, y_min+1, obj.get_current_color());

    } else { //ops.size() == 2

        mpf_class x_min, y_min, x_max, y_max;
        bool point_extract_success_min = extract_point(ops[0], x_min, y_min);
        bool point_extract_success_max = extract_point(ops[1], x_max, y_max);

        if (!point_extract_success_min || !point_extract_success_max) {
            env.raise_error("graphics Rectangle", "invalid Point form");
            return;
        }
        obj.add_rectangle(x_min, y_min, x_max, y_max, obj.get_current_color());
    }
}

void collect_rgbcolor(const expression_tree::operands_t& ops, graphics_object& obj, enviroment& env) {

    int red = 255;
    int green = 255;
    int blue = 255;
    int alpha = 255;

    if ( ops.size() != 3 ) {
        env.raise_error("graphics RGBColor", "invalid format, 3 arg expected");
        return;
    }

    if ( true /*ops.size() == 3*/ ) {
        if ( (ops[0].get_type() != expression_tree::APPROXIMATE_NUMBER && 
            ops[0].get_type() != expression_tree::EXACT_NUMBER) ||
            (ops[1].get_type() != expression_tree::APPROXIMATE_NUMBER && 
            ops[1].get_type() != expression_tree::EXACT_NUMBER) ||
            (ops[2].get_type() != expression_tree::APPROXIMATE_NUMBER && 
            ops[2].get_type() != expression_tree::EXACT_NUMBER) )
        {
                env.raise_error("graphics RGBColor", "invalid format");
                return;
        }
        mpf_class tmp = ops[0].get_number_as_mpf() * 255;
        red = tmp.get_si();

        tmp = ops[1].get_number_as_mpf() * 255;
        green = tmp.get_si();

        tmp = ops[2].get_number_as_mpf() * 255;
        blue = tmp.get_si();
    } 

    red = clip_to_range( red, 0, 255 );
    green = clip_to_range( green, 0, 255 );
    blue = clip_to_range( blue, 0, 255 );
    alpha = clip_to_range( alpha, 0, 255 );

    rgba_color_t result = 0;
    result = set_red(result, red);
    result = set_green(result, green);
    result = set_blue(result, blue);
    result = set_alpha(result, alpha);

    if (env.print_debug) {
        std::cout << "new color " << std::hex << result << " (" << red << ", " << green << ", " << blue << ")" << std::dec << std::endl;
    }

    obj.set_current_color(result);

}

void collect_hue(const expression_tree::operands_t& ops, graphics_object& obj, enviroment& env) {
    if ( ops.size() != 3 ) {
        env.raise_error("graphics Hue", "invalid format, 3 arg expected");
        return;
    }

    double hue = 0.;
    double sat = 1.;
    double bri = 1.;
    double alpha = 1.;

    if ( true /*ops.size() == 3*/ ) {
        if ( (ops[0].get_type() != expression_tree::APPROXIMATE_NUMBER && 
            ops[0].get_type() != expression_tree::EXACT_NUMBER) ||
            (ops[1].get_type() != expression_tree::APPROXIMATE_NUMBER && 
            ops[1].get_type() != expression_tree::EXACT_NUMBER) ||
            (ops[2].get_type() != expression_tree::APPROXIMATE_NUMBER && 
            ops[2].get_type() != expression_tree::EXACT_NUMBER) )
        {
                env.raise_error("graphics Hue", "invalid format");
                return;
        }
        hue = ops[0].get_number_as_mpf().get_d();
        sat = ops[1].get_number_as_mpf().get_d();
        bri = ops[2].get_number_as_mpf().get_d();
    }

    hue = clip_to_range( hue, 0.0, 1.0 );
    sat = clip_to_range( sat, 0.0, 1.0 );
    bri = clip_to_range( bri, 0.0, 1.0 );
    alpha = clip_to_range( bri, 0.0, 1.0 );

    double hue2, sat2, lum2;
    double red, green, blue;

    hsb_to_hsl(hue, sat, bri, hue2, sat2, lum2);
    hsl_to_rgb(hue2, sat2, lum2, red, green, blue);

    assert( red >= 0.0 && red <= 1.0 );
    assert( green >= 0.0 && green <= 1.0 );
    assert( blue >= 0.0 && blue <= 1.0 );
    assert( alpha >= 0.0 && alpha <= 1.0 );

    rgba_color_t result = 0;
    result = set_red(result, static_cast<single_color_t>(red * 255.));
    result = set_green(result, static_cast<single_color_t>(green * 255.));
    result = set_blue(result, static_cast<single_color_t>(blue * 255.));
    result = set_alpha(result, static_cast<single_color_t>(alpha * 255.));    

    if (env.print_debug) {
        std::cout << "new hue color " << std::hex << result << " (" << red << ", " << green << ", " << blue << ")" << std::dec << std::endl;
    }

    obj.set_current_color(result);

}

void hsb_to_hsl(double h, double s, double b, double& hh, double& ss, double& ll) {
    hh = h;
    ll = (2 - s) * b;
    ss = s * b;
    ss /= (ll <= 1) ? (ll) : 2 - (ll);
    ll /= 2;
}

void hsl_to_rgb(double h, double s, double l, double& red, double& green, double& blue) {
    if ( l == 0.0 ) {
        red = green = blue = 0.;
        return;
    }
    if ( s == 0. ) {
        red = green = blue = l;
        return;
    }

    double tmp2 = ((l<=0.5) ? l*(1.0+s) : l+s-(l*s));
    double tmp1 = 2.0*l-tmp2;

    boost::array<double, 3> t3 = { h+1.0/3.0, h, h-1.0/3.0 };
    boost::array<double, 3> clr = { 0., 0., 0. };

    for ( int i = 0; i < 3; ++i ) {
        if (t3[i] < 0) {
            t3[i] += 1.0;
        }
        if (t3[i] > 1) {
            t3[i] -= 1.0;
        }
 
        if (6.0*t3[i] < 1.0) {
            clr[i] = tmp1 + (tmp2-tmp1) * t3[i]*6.0;
        } else if (2.0*t3[i] < 1.0) {
            clr[i] = tmp2;
        } else if (3.0*t3[i] < 2.0) {
            clr[i] = (tmp1 + (tmp2-tmp1) * ((2.0/3.0) - t3[i])*6.0);
        } else {
            clr[i] = tmp1;
        }
    }
    red = clr[0];
    green = clr[1];
    blue = clr[2];
    
    
}

void hsl_to_hsb(double hh, double ss, double ll,
 double* h, double* s, double *b)
 {
    *h = hh;
     ll *= 2;     ss *= (ll <= 1) ? ll : 2 - ll;
    *b = (ll + ss) / 2;
   *s = (2 * ss) / (ll + ss);
}

} //graphics_impl

} //namespace r0