
#include "graphics_object.hpp"

#include "util.hpp"

#include <cassert>
#include <iostream>

namespace r0 {


graphics_object::graphics_object()
    : image_width(), image_height() {}

void graphics_object::add_line(const mpf_class& x1, const mpf_class& y1, const mpf_class& x2, const mpf_class& y2, rgba_color_t col) {
    primitives.push_back( graphics_primitive::make_line(x1, y1, x2, y2, col) );
}

void graphics_object::add_circle(const mpf_class& x, const mpf_class& y, const mpf_class& r, rgba_color_t col) {
    primitives.push_back( graphics_primitive::make_circle(x, y, r, col) );
}

void graphics_object::add_disk(const mpf_class& x, const mpf_class& y, const mpf_class& r, rgba_color_t col) {
    primitives.push_back( graphics_primitive::make_disk(x, y, r, col) );
}

void graphics_object::add_point(const mpf_class& x, const mpf_class& y, rgba_color_t col) {
    primitives.push_back( graphics_primitive::make_point(x, y, col) );
}

void graphics_object::add_rectangle(const mpf_class& x_min, const mpf_class& y_min, const mpf_class& x_max, const mpf_class& y_max, rgba_color_t col) {
    primitives.push_back( graphics_primitive::make_rectangle(x_min, y_min, x_max, y_max, col) );
}

unsigned graphics_object::get_image_width() const {
    return image_width;
}

unsigned graphics_object::get_image_height() const {
    return image_height;
}

void graphics_object::set_axes(bool a) {
    axes = a;
}

void graphics_object::set_plot_range(mpf_class x_min, mpf_class x_max, mpf_class y_min, mpf_class y_max) {
    assert( x_max > x_min && y_max > y_min );

    plot_range_x_min = x_min;
    plot_range_x_max = x_max;
    plot_range_y_min = y_min;
    plot_range_y_max = y_max;
}

void graphics_object::set_image_width(unsigned w) {
    image_width = w;
}

void graphics_object::set_image_height(unsigned h) {
    image_height = h;
}

rgba_color_t graphics_object::get_current_color() const {
    return current_color;
}

void graphics_object::set_current_color(rgba_color_t new_color) {
    current_color = new_color;
}

bmp_image graphics_object::to_bmp_image() const {

    bmp_image img(image_width, image_height);

    for(unsigned i = 0; i < primitives.size(); ++i) {
        switch( primitives[i].get_type() ) {
        case graphics_primitive::LINE :
            {
                const int x1 = adjust_point_x( primitives[i].get_line_x1() );
                const int y1 = adjust_point_y( primitives[i].get_line_y1() ); 
                const int x2 = adjust_point_x( primitives[i].get_line_x2() );                 
                const int y2 = adjust_point_y( primitives[i].get_line_y2() );
                const rgba_color_t color = primitives[i].get_color();
                
                //img.draw_anti_aliased_line(x1, y1, x2, y2, color);
                img.draw_line(x1, y1, x2, y2, color);
            }
            break;
        case graphics_primitive::CIRCLE :
            {
                const int x = adjust_point_x( primitives[i].get_circle_x() );
                const int y = adjust_point_y( primitives[i].get_circle_y() ); 
                const int rx = adjust_distance_x( primitives[i].get_circle_r() );
                const int ry = adjust_distance_y( primitives[i].get_circle_r() );
                const rgba_color_t color = primitives[i].get_color();
                
                //img.draw_ellipse(x - rx, y - ry, x + rx, y + rx, color);
                img.draw_ellipse(x - rx, y + ry, x + rx, y - rx, color);

            }           
            break;
        case graphics_primitive::DISK :
            {
                const int x = adjust_point_x( primitives[i].get_disk_x() );
                const int y = adjust_point_y( primitives[i].get_disk_y() ); 
                const int rx = adjust_distance_x( primitives[i].get_disk_r() );
                const int ry = adjust_distance_y( primitives[i].get_disk_r() );
                const rgba_color_t color = primitives[i].get_color();
                
                //img.draw_filled_ellipse(x - rx, y - ry, x + rx, y + rx, color);
                img.draw_filled_ellipse(x - rx, y + ry, x + rx, y - rx, color);

            }           
            break;
        case graphics_primitive::POINT :
            {
                const int x = adjust_point_x( primitives[i].get_point_x() );
                const int y = adjust_point_y( primitives[i].get_point_y() );
                const rgba_color_t color = primitives[i].get_color();
                
                img.draw_filled_circle(x, y, 1, color);
            }
            break;
        case graphics_primitive::RECTANGLE :
            {
                const int x_min = adjust_point_x( primitives[i].get_rectangle_x_min() );
                const int y_min = adjust_point_y( primitives[i].get_rectangle_y_min() );
                const int x_max = adjust_point_x( primitives[i].get_rectangle_x_max() );
                const int y_max = adjust_point_y( primitives[i].get_rectangle_y_max() );

                const rgba_color_t color = primitives[i].get_color();
                
                img.draw_rectangle(x_min, y_min, x_max, y_max, color);
            }
            break;
        case graphics_primitive::INVALID :
            assert(false && "graphics primitive of type invalid found");
            return img;
        default:
            assert(false && "graphics default branch invoked");
            return img;

        }
    }
    return img;
}

int graphics_object::adjust_point_x( const mpf_class& x ) const {
    //return (w*(-min + x))/(max - min)
    const mpf_class res = image_width*(x-plot_range_x_min)/(plot_range_x_max - plot_range_x_min);
    return round_half_away_from_zero(res.get_d());
}

int graphics_object::adjust_point_y( const mpf_class& y ) const {
    //return h-(h*(-min + y))/(max - min)
    const mpf_class res = image_height-(image_height*(y-plot_range_y_min)/(plot_range_y_max - plot_range_y_min));
    return round_half_away_from_zero(res.get_d());
}

int graphics_object::adjust_distance_x( const mpf_class& d ) const {

    const mpf_class res = d * (image_width / (plot_range_x_max - plot_range_x_min));

    return round_half_away_from_zero(res.get_d());
}

int graphics_object::adjust_distance_y( const mpf_class& d ) const {

    const mpf_class res = d * (image_height / (plot_range_y_max - plot_range_y_min));

    return round_half_away_from_zero(res.get_d());
}

} //namespace r0