
#ifndef GRAPHICS_OBJECT_HPP_
#define GRAPHICS_OBJECT_HPP_

#include <vector>

#include "mpirxx_include.hpp"
#include "bmp_image.hpp"
#include "graphics_primitive.hpp"

namespace r0 {

class graphics_object {
public:
    graphics_object();

    //primitives :
    void add_line(const mpf_class& x1, const mpf_class& y1, const mpf_class& x2, const mpf_class& y2, rgba_color_t col);
    void add_circle(const mpf_class& x, const mpf_class& y, const mpf_class& r, rgba_color_t col);
    void add_disk(const mpf_class& x, const mpf_class& y, const mpf_class& r, rgba_color_t col);
    void add_point(const mpf_class& x, const mpf_class& y, rgba_color_t col);
    void add_rectangle(const mpf_class& x_min, const mpf_class& y_min, const mpf_class& x_max, const mpf_class& y_max, rgba_color_t col);

    unsigned get_image_width() const;
    unsigned get_image_height() const;

    void set_axes(bool a);
    void set_plot_range(mpf_class x_min, mpf_class x_max, mpf_class y_min, mpf_class y_max);
    void set_image_width(unsigned w);
    void set_image_height(unsigned h);

    rgba_color_t get_current_color() const;
    void set_current_color(rgba_color_t new_color);

    bmp_image to_bmp_image() const;

    //returns the coordinates as those appear on the bmp_image
    int adjust_point_x( const mpf_class& x ) const;
    int adjust_point_y( const mpf_class& y ) const;
    int adjust_distance_x( const mpf_class& d ) const;
    int adjust_distance_y( const mpf_class& d ) const;
private:
    bool axes;

    unsigned image_width;
    unsigned image_height;

    mpf_class plot_range_x_min;
    mpf_class plot_range_x_max;
    mpf_class plot_range_y_min;
    mpf_class plot_range_y_max;

    rgba_color_t current_color;

    std::vector<graphics_primitive> primitives;

};

} //namespace r0

#endif //GRAPHICS_OBJECT_HPP_