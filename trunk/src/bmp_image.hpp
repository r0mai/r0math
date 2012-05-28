
#ifndef BMP_IMAGE_HPP_
#define BMP_IMAGE_HPP_

#include "matrix.hpp"
#include "color.hpp"

namespace r0 {

class bmp_image {
public:

    bmp_image(unsigned width, unsigned height, rgba_color_t bg_color = 0xFFFFFFFF);

    rgba_color_t& pixel(unsigned x, unsigned y);
    const rgba_color_t& pixel(unsigned x, unsigned y) const;

    void draw_line(int x1, int y1, int x2, int y2, rgba_color_t color);

    void draw_vertical_line(int column, int y1, int y2, rgba_color_t color);
    void draw_horizontal_line(int row, int x1, int x2, rgba_color_t color);

    void draw_anti_aliased_line(double x1, double y1, double x2, double y2, rgba_color_t color);

    void draw_circle(int x, int y, int r, rgba_color_t color);
    void draw_filled_circle(int x, int y, int r, rgba_color_t color);

    void draw_ellipse(int x0, int y0, int x1, int y1, rgba_color_t color); //rectangular
    void draw_filled_ellipse(int x0, int y0, int x1, int y1, rgba_color_t color); //rectangular

    void draw_rectangle(int x_min, int y_min, int x_max, int y_max, rgba_color_t color);

    //checked for inbounds
    void plot_with_brightness(int x, int y, rgba_color_t color, double brightness);

    //return 0 on success
    int output_to_file(const std::string& file_name) const;

    
private:
    
    int width;
    int height;

    matrix<rgba_color_t> pixels;

    void antialiased_line_helper_plot(int x, int y, rgba_color_t color, double brightness, bool reversed);
};

namespace bmp_impl {

boost::uint16_t endian_swap(boost::uint16_t x);
boost::uint32_t endian_swap(boost::uint32_t x);
boost::uint64_t endian_swap(boost::uint64_t x);

} //namespace bmp_impl

} //namespace r0

#endif //BMP_IMAGE_HPP_