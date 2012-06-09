
#ifndef COLOR_HPP_
#define COLOR_HPP_

#include <boost/cstdint.hpp>

namespace r0 {

//rgb_color_t format : 0xRRGGBBAA
typedef boost::uint8_t  single_color_t;
typedef boost::uint32_t rgba_color_t;

single_color_t get_red  (rgba_color_t);
single_color_t get_green(rgba_color_t);
single_color_t get_blue (rgba_color_t);
single_color_t get_alpha(rgba_color_t);

rgba_color_t set_red    (rgba_color_t, single_color_t);
rgba_color_t set_green  (rgba_color_t, single_color_t);
rgba_color_t set_blue   (rgba_color_t, single_color_t);
rgba_color_t set_alpha  (rgba_color_t, single_color_t);

} //namespace r0

#endif //COLOR_HPP_