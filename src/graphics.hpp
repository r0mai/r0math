
#ifndef GRAPHICS_HPP_
#define GRAPHICS_HPP_

#include <string>

#include "color.hpp"
#include "expression_tree.hpp"
#include "enviroment_fwd.hpp"
#include "mpirxx_include.hpp"
#include "graphics_object.hpp"

namespace r0 {

std::string expression_tree_to_graphics(const expression_tree& expr, enviroment& env);

namespace graphics_impl {

bool pick_out_boolean(const expression_tree& expr, bool& result);

bool pick_out_image_size(const expression_tree& expr, unsigned long& width_res, unsigned long& height_res);

bool pick_out_plot_range(const expression_tree& expr, mpf_class& x_min, mpf_class& x_max, mpf_class& y_min, mpf_class& y_max);

//Parses {x, y}
bool extract_point(const expression_tree& expr, mpf_class& x, mpf_class& y);


//primitives
void collect_primitives(const expression_tree::operands_t& ops, graphics_object& obj, enviroment& env);
void collect_line_primitve(const expression_tree::operands_t& ops, graphics_object& obj, enviroment& env);
void collect_circle_disk_primitve(const expression_tree::operands_t& ops, graphics_object& obj, enviroment& env, const std::string& head); //head = "Circle", or "Disk"
void collect_point_primitve(const expression_tree::operands_t& ops, graphics_object& obj, enviroment& env);
void collect_rectangle_primitve(const expression_tree::operands_t& ops, graphics_object& obj, enviroment& env);

//directives
void collect_rgbcolor(const expression_tree::operands_t& ops, graphics_object& obj, enviroment& env);
void collect_hue(const expression_tree::operands_t& ops, graphics_object& obj, enviroment& env); //hsb

void hsb_to_hsl(double h, double s, double b, double& hh, double& ss, double& ll);
void hsl_to_rgb(double h, double s, double l, double& red, double& green, double& blue);

} //namespace graphics_impl

} //namespace r0

#endif //GRAPHICS_HPP_