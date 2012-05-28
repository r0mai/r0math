
#ifndef GRAPHICS_PRIMITIVE_HPP_
#define GRAPHICS_PRIMITIVE_HPP_

#include <vector>

#include "color.hpp"
#include "mpirxx_include.hpp"

namespace r0 {

class graphics_primitive {
public:
    enum type_t {
        INVALID,

        //size = 4
        //0 -> x1
        //1 -> y1
        //2 -> x2
        //3 -> y2
        LINE,

        //size = 3
        //0 -> x
        //1 -> y
        //2 -> r
        CIRCLE,
        DISK,

        //0 -> x
        //1 -> y
        POINT,

        //0 -> x_min
        //1 -> y_min
        //2 -> x_max
        //3 -> y_max
        RECTANGLE
    };

    graphics_primitive();

    static graphics_primitive make_line(
        const mpf_class& x1, const mpf_class& y1,
        const mpf_class& x2, const mpf_class& y2, rgba_color_t col);

    static graphics_primitive make_circle(
        const mpf_class& x, const mpf_class& y,
        const mpf_class& r, rgba_color_t col);

    static graphics_primitive make_disk(
        const mpf_class& x, const mpf_class& y,
        const mpf_class& r, rgba_color_t col);

    static graphics_primitive make_point(
        const mpf_class& x, const mpf_class& y, rgba_color_t col);

    static graphics_primitive make_rectangle(
        const mpf_class& x_min, const mpf_class& y_min, const mpf_class& x_max, const mpf_class& y_max, rgba_color_t col);

    type_t get_type() const;

    const mpf_class& get_line_x1() const;
    const mpf_class& get_line_y1() const;
    const mpf_class& get_line_x2() const;
    const mpf_class& get_line_y2() const;

    const mpf_class& get_circle_x() const;
    const mpf_class& get_circle_y() const;
    const mpf_class& get_circle_r() const;

    const mpf_class& get_disk_x() const;
    const mpf_class& get_disk_y() const;
    const mpf_class& get_disk_r() const;

    const mpf_class& get_point_x() const;
    const mpf_class& get_point_y() const;

    const mpf_class& get_rectangle_x_min() const;
    const mpf_class& get_rectangle_y_min() const;
    const mpf_class& get_rectangle_x_max() const;
    const mpf_class& get_rectangle_y_max() const;

    rgba_color_t get_color() const;

private:
    type_t type;
    std::vector<mpf_class> parameters;
    rgba_color_t color;
};

} //namespace r0

#endif //GRAPHICS_PRIMITIVE_HPP_