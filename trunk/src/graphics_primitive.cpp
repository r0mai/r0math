
#include "graphics_primitive.hpp"

#include <cassert>

namespace r0 {

graphics_primitive::graphics_primitive() : type(INVALID) {}

graphics_primitive graphics_primitive::make_line(
        const mpf_class& x1, const mpf_class& y1,
        const mpf_class& x2, const mpf_class& y2, rgba_color_t col) 
{
    graphics_primitive res;
    res.type = LINE;
    res.parameters.resize(4);
    res.parameters[0] = x1;
    res.parameters[1] = y1;
    res.parameters[2] = x2;
    res.parameters[3] = y2;
    res.color = col;

    return res;
}

graphics_primitive graphics_primitive::make_circle(
    const mpf_class& x, const mpf_class& y,
    const mpf_class& r, rgba_color_t col)
{
    graphics_primitive res;
    res.type = CIRCLE;
    res.parameters.resize(3);
    res.parameters[0] = x;
    res.parameters[1] = y;
    res.parameters[2] = r;
    res.color = col;

    return res;
}

graphics_primitive graphics_primitive::make_disk(
    const mpf_class& x, const mpf_class& y,
    const mpf_class& r, rgba_color_t col)
{
    graphics_primitive res;
    res.type = DISK;
    res.parameters.resize(3);
    res.parameters[0] = x;
    res.parameters[1] = y;
    res.parameters[2] = r;
    res.color = col;

    return res;
}

graphics_primitive graphics_primitive::make_point(
    const mpf_class& x, const mpf_class& y, rgba_color_t col)
{
    graphics_primitive res;
    res.type = POINT;
    res.parameters.resize(2);
    res.parameters[0] = x;
    res.parameters[1] = y;
    res.color = col;

    return res;
}

graphics_primitive graphics_primitive::make_rectangle(
    const mpf_class& x_min, const mpf_class& y_min, const mpf_class& x_max, const mpf_class& y_max, rgba_color_t col)
{
    graphics_primitive res;
    res.type = RECTANGLE;
    res.parameters.resize(4);
    res.parameters[0] = x_min;
    res.parameters[1] = y_min;
    res.parameters[2] = x_max;
    res.parameters[3] = y_max;
    res.color = col;

    return res;
}

graphics_primitive::type_t graphics_primitive::get_type() const {
    return type;
}

const mpf_class& graphics_primitive::get_line_x1() const {
    assert( type == LINE );
    return parameters[0];
}

const mpf_class& graphics_primitive::get_line_y1() const {
    assert( type == LINE );
    return parameters[1];
}

const mpf_class& graphics_primitive::get_line_x2() const {
    assert( type == LINE );
    return parameters[2];
}

const mpf_class& graphics_primitive::get_line_y2() const {
    assert( type == LINE );
    return parameters[3];
}

const mpf_class& graphics_primitive::get_circle_x() const {
    assert( type == CIRCLE );
    return parameters[0];
}

const mpf_class& graphics_primitive::get_circle_y() const {
    assert( type == CIRCLE );
    return parameters[1];
}

const mpf_class& graphics_primitive::get_circle_r() const {
    assert( type == CIRCLE );
    return parameters[2];
}

const mpf_class& graphics_primitive::get_disk_x() const {
    assert( type == DISK );
    return parameters[0];
}

const mpf_class& graphics_primitive::get_disk_y() const {
    assert( type == DISK );
    return parameters[1];
}

const mpf_class& graphics_primitive::get_disk_r() const {
    assert( type == DISK );
    return parameters[2];
}

const mpf_class& graphics_primitive::get_point_x() const {
    assert( type == POINT );
    return parameters[0];
}

const mpf_class& graphics_primitive::get_point_y() const {
    assert( type == POINT );
    return parameters[1];
}

const mpf_class& graphics_primitive::get_rectangle_x_min() const {
    assert( type == RECTANGLE );
    return parameters[0];
}

const mpf_class& graphics_primitive::get_rectangle_y_min() const {
    assert( type == RECTANGLE );
    return parameters[1];
}

const mpf_class& graphics_primitive::get_rectangle_x_max() const {
    assert( type == RECTANGLE );
    return parameters[2];
}

const mpf_class& graphics_primitive::get_rectangle_y_max() const {
    assert( type == RECTANGLE );
    return parameters[3];
}

rgba_color_t graphics_primitive::get_color() const {
    assert( type != INVALID );
    return color;
}

} //namespace r0