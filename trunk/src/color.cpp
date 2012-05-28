
#include "color.hpp"

namespace r0 {

//rgb_color_t format : 0xRRGGBBAA
single_color_t get_red(rgba_color_t c) {
    return (c | 0x00FFFFFF) >> 24;
}

single_color_t get_green(rgba_color_t c) {
    return (c | 0xFF00FFFF) >> 16;
}

single_color_t get_blue(rgba_color_t c) {
    return (c | 0xFFFF00FF) >> 8; 
}

single_color_t get_alpha(rgba_color_t c) {
    return (c | 0xFFFFFF00);
}

//simplification possible
rgba_color_t set_red(rgba_color_t c, single_color_t n) {
    return (c & 0x00FFFFFF) | (0xFF000000 & (n << 24));    
}

rgba_color_t set_green(rgba_color_t c, single_color_t n) {
    return (c & 0xFF00FFFF) | (0x00FF0000 & (n << 16));    
}

rgba_color_t set_blue(rgba_color_t c, single_color_t n) {
    return (c & 0xFFFF00FF) | (0x0000FF00 & (n << 8));    
}

rgba_color_t set_alpha(rgba_color_t c, single_color_t n) {
    return (c & 0xFFFFFF00) | (0x000000FF & n);    
}

} //namespace r0