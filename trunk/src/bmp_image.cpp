
#include "bmp_image.hpp"

#include <iostream>
#include <fstream>
#include <cassert>
#include <cmath>

#include "util.hpp"

namespace r0 {


bmp_image::bmp_image(unsigned w, unsigned h, rgba_color_t bg_color) 
    : width(w), height(h), pixels(w, h, bg_color) {}

rgba_color_t& bmp_image::pixel(unsigned x, unsigned y) {
    return pixels(x, y);
}

const rgba_color_t& bmp_image::pixel(unsigned x, unsigned y) const {
    return pixels(x, y);
}

void bmp_image::draw_line(int x1, int y1, int x2, int y2, rgba_color_t color) {

    if ( (x1 < 0 || x1 >= width || y1 < 0 || y1 >= height) && 
        (x2 < 0 || x2 >= width || y2 < 0 || y2 >= height) ) {
            //std::cout << "Line[" << x1 << ", " << y1 << ", " << x2 << ", " << y2 << "] is of bounds" << std::endl;

            //line intersection with line (0,0) -> (w,0)


            return;
    }
    //Bresenham's line algorithm

    int dx = std::abs(x2 - x1);
    int dy = std::abs(y2 - y1);

    int sx = x1 < x2 ? 1 : -1;
    int sy = y1 < y2 ? 1 : -1;

    int err = dx-dy;

    while (true) {
        
        if ( x1 >= 0 && x1 < width && y1 >= 0 && y1 < height ) {             
            pixels(x1, y1) = color;
        }
        if ( x1 == x2 && y1 == y2 ) {
            break;
        }
        int e2 = 2*err;
        if ( e2 > -dy ) {
            err -= dy;
            x1 += sx;
        }
        if ( e2 < dx ) {
            err += dx;
            y1 += sy;
        }
    }
}

void bmp_image::draw_vertical_line(int column, int y1, int y2, rgba_color_t color) {
    if ( column < 0 || column >= width ) {
        return;
    }

    if ( y1 > y2 ) {
        std::swap(y1, y2);
    }

    y1 = std::max(y1, 0);
    y2 = std::min(y2, height-1);

    for ( int y = y1; y <= y2; ++y ) {
        pixel(column, y) = color;
    }
}

void bmp_image::draw_horizontal_line(int row, int x1, int x2, rgba_color_t color) {
    if ( row < 0 || row >= height ) {
        return;
    }

    if ( x1 > x2 ) {
        std::swap(x1, x2);
    }

    x1 = std::max(x1, 0);
    x2 = std::min(x2, width-1);

    for ( int x = x1; x <= x2; ++x ) {
        pixel(x, row) = color;
    }
}

void bmp_image::draw_anti_aliased_line(double x1, double y1, double x2, double y2, rgba_color_t color) {

    //Xiaolin Wu's line algorithm

    double dx = x2 - x1;
    double dy = y2 - y1;

    const bool reversed = std::abs(dx) < std::abs(dy);

    if ( reversed ) {
        std::swap(x1, y1);
        std::swap(x2, y2);
        std::swap(dx, dy);
    }
    if ( x2 < x1 ) {
        std::swap( x1, x2 );
        std::swap( y1, y2 );
    }
    double gradient = dy / dx;

    //first end point
    double xend = round_half_up(x1);
    double yend = y1 + gradient * (xend - x1);
    double xgap = right_fractional_part(x1 + 0.5);
    int xpxl1 = static_cast<int>(xend);
    int ypxl1 = integer_part(yend);
    antialiased_line_helper_plot(xpxl1, ypxl1, color, right_fractional_part(yend) * xgap, reversed);
    antialiased_line_helper_plot(xpxl1, ypxl1+1, color, fractional_part(yend) * xgap, reversed);
    double intery = yend + gradient;

    xend = round_half_up(x2);
    yend = y2 + gradient * (xend - x2);
    xgap = fractional_part(x2 + 0.5);
    int xpxl2 = static_cast<int>(xend);
    int ypxl2 = integer_part(yend);
    antialiased_line_helper_plot(xpxl2, ypxl2, color, right_fractional_part(yend) * xgap, reversed);
    antialiased_line_helper_plot(xpxl2, ypxl2+1, color, fractional_part(yend) * xgap, reversed);

    for (int x = xpxl1 + 1; x <= xpxl2 -1; ++x) {
        antialiased_line_helper_plot(x, integer_part(intery), color, right_fractional_part(intery), reversed);
        antialiased_line_helper_plot(x, integer_part(intery) + 1, color, fractional_part(intery), reversed);
        intery += gradient;
    }
}

void bmp_image::draw_circle(int x0, int y0, int radius, rgba_color_t color) {

    assert(radius >= 0);    

    //Midpoint circle algorithm

    //std::cout << x0 << ", " << y0 << ", " << radius << ", " << std::hex << color << std::endl;

    int f = 1 - radius;
    int ddF_x = 1;
    int ddF_y = -2 * radius;
    int x = 0;
    int y = radius;
 
    pixels.set_if_inbounds(x0, y0 + radius, color);
    pixels.set_if_inbounds(x0, y0 - radius, color);
    pixels.set_if_inbounds(x0 + radius, y0, color);
    pixels.set_if_inbounds(x0 - radius, y0, color);
 
    while(x < y) {

        if(f >= 0)  {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }

        x++;
        ddF_x += 2;
        f += ddF_x; 

        pixels.set_if_inbounds(x0 + x, y0 + y, color);
        pixels.set_if_inbounds(x0 - x, y0 + y, color);
        pixels.set_if_inbounds(x0 + x, y0 - y, color);
        pixels.set_if_inbounds(x0 - x, y0 - y, color);
        pixels.set_if_inbounds(x0 + y, y0 + x, color);
        pixels.set_if_inbounds(x0 - y, y0 + x, color);
        pixels.set_if_inbounds(x0 + y, y0 - x, color);
        pixels.set_if_inbounds(x0 - y, y0 - x, color);
    }    
}

void bmp_image::draw_filled_circle(int x0, int y0, int radius, rgba_color_t color) {

    assert(radius >= 0);    

    //Midpoint circle algorithm

    //std::cout << x0 << ", " << y0 << ", " << radius << ", " << std::hex << color << std::endl;

    int f = 1 - radius;
    int ddF_x = 1;
    int ddF_y = -2 * radius;
    int x = 0;
    int y = radius;
 

    draw_horizontal_line(y0, x0 - radius, x0 + radius, color);
    draw_vertical_line(x0, y0 - radius, y0 + radius, color);
 
    while(x < y) {

        if(f >= 0)  {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }

        x++;
        ddF_x += 2;
        f += ddF_x; 

        draw_horizontal_line(y0 + y, x0 - x, x0 + x, color);
        draw_horizontal_line(y0 - y, x0 - x, x0 + x, color);
        draw_horizontal_line(y0 + x, x0 - y, x0 + y, color);
        draw_horizontal_line(y0 - x, x0 - y, x0 + y, color);
    }    
}

void bmp_image::draw_ellipse(int x0, int y0, int x1, int y1, rgba_color_t color) {

    //http://free.pages.at/easyfilter/bresenham.html

    int a = std::abs(x1-x0);
    int b = std::abs(y1-y0);
    int b1 = b & 1; /* values of diameter */

    long dx = 4*(1-a)*b*b;
    int dy = 4*(b1+1)*a*a; /* error increment */

    long err = dx+dy+b1*a*a, e2; /* error of 1.step */

    if (x0 > x1) { /* if called with swapped points */
        x0 = x1;
        x1 += a;
    } 
    if (y0 > y1) {  /* .. exchange them */
        y0 = y1;
    }

    y0 += (b+1)/2;
    y1 = y0-b1;   /* starting pixel */

    a *= 8*a;
    b1 = 8*b*b;

    do {
        pixels.set_if_inbounds(x1, y0, color); /*   I. Quadrant */
        pixels.set_if_inbounds(x0, y0, color); /*  II. Quadrant */
        pixels.set_if_inbounds(x0, y1, color); /* III. Quadrant */
        pixels.set_if_inbounds(x1, y1, color); /*  IV. Quadrant */

        e2 = 2*err;

        if (e2 >= dx) { 
            x0++; x1--; err += dx += b1;
        } /* x step */

        if (e2 <= dy) {
            y0++; y1--; err += dy += a;
        }  /* y step */ 
    } while (x0 <= x1);
   
    while (y0-y1 < b) {  /* too early stop of flat ellipses a=1 */
        pixels.set_if_inbounds(x0-1, y0, color); /* -> finish tip of ellipse */
        pixels.set_if_inbounds(x1+1, y0++, color); 
        pixels.set_if_inbounds(x0-1, y1, color);
        pixels.set_if_inbounds(x1+1, y1--, color); 
    }
}

void bmp_image::draw_filled_ellipse(int x0, int y0, int x1, int y1, rgba_color_t color) {

    //http://free.pages.at/easyfilter/bresenham.html

    int a = std::abs(x1-x0);
    int b = std::abs(y1-y0);
    int b1 = b & 1; /* values of diameter */

    long dx = 4*(1-a)*b*b;
    int dy = 4*(b1+1)*a*a; /* error increment */

    long err = dx+dy+b1*a*a, e2; /* error of 1.step */

    if (x0 > x1) { /* if called with swapped points */
        x0 = x1;
        x1 += a;
    } 
    if (y0 > y1) {  /* .. exchange them */
        y0 = y1;
    }

    y0 += (b+1)/2;
    y1 = y0-b1;   /* starting pixel */

    a *= 8*a;
    b1 = 8*b*b;

    do {
        
        draw_horizontal_line(y0, x0, x1, color);
        draw_horizontal_line(y1, x0, x1, color);

        e2 = 2*err;

        if (e2 >= dx) { 
            x0++; x1--; err += dx += b1;
        } /* x step */

        if (e2 <= dy) {
            y0++; y1--; err += dy += a;
        }  /* y step */ 
    } while (x0 <= x1);
   
    while (y0-y1 < b) {  /* too early stop of flat ellipses a=1 */
        draw_horizontal_line(y0, x0-1, x1+1, color);
        draw_horizontal_line(y1, x0-1, x1+1, color); 
        ++y0;
        --y1;
    }

}

void bmp_image::draw_rectangle(int x_min, int y_min, int x_max, int y_max, rgba_color_t color) {
    if ( x_min > x_max ) {
        std::swap( x_min, x_max );
    }
    if ( y_min > y_max ) {
        std::swap( y_min, y_max );
    }
    for ( int col = x_min; col <= x_max; ++col ) {
        draw_vertical_line(col, y_min, y_max, color);
    }
}

void bmp_image::plot_with_brightness(int x, int y, rgba_color_t color, double brightness) {
    if ( brightness < 0.0 ) {
        brightness = 0.0;
    }
    if ( brightness > 1.0 ) {
        brightness = 1.0;
    }

    brightness = 1.0 - brightness;   

    color = set_red(color,      std::min(255, round_half_up( 255. * brightness + get_red(color) )) );
    color = set_green(color,    std::min(255, round_half_up( 255. * brightness + get_green(color) )) );
    color = set_blue(color,     std::min(255, round_half_up( 255. * brightness + get_blue(color) )) );

    pixels.set_if_inbounds(x, y, color);
}

int bmp_image::output_to_file(const std::string& file_name) const {
    std::ofstream out(file_name.c_str(), std::ios_base::binary | std::ios_base::out);

    if ( !out ) {
        return 1;
    }
    //full header size 122
    /* BMP structure
     *    File Header
     * 0   Magic number 0x42, 0x4d (2 bytes)
     * 2   File size = w*h + h*(w%4) + 122 ? (4 bytes)
     * 6   Unused (4 bytes)
     * 10  Pixel array offset = 122 (4 bytes)
     *    DIB Header
     * 14  Bytes in DIB Header = 108 (4 bytes)
     * 18  Bitmap width (4 bytes)
     * 22  Bitmap width (4 bytes)
     * 26  Color planes = 1 (2 bytes) 
     * 28  Bits/pixel = 32 (2 bytes)
     * 30  BI_BITFIELDS = 3 (no compression used) (4 bytes)
     * 34  Size of the raw data in the Pixel Array = w*h + h*(w%4) ? (incl padding) (4 bytes)
     * 38  horizonal pixels/meter = 2835 (4 bytes)
     * 42  vertival pixels/meter = 2835 (4 bytes)
     * 46  Number of colors in the palette = 0 (4 bytes)
     * 50  Important colors = 0 (4 bytes)
     * 54  Red channel bit mask = 0x00FF0000 (4 bytes)
     * 58  Green channel bit mask = 0x0000FF00 (4 bytes)
     * 62  Blue channel bit mask = 0x000000FF (4 bytes)
     * 66  Alpha channel bit mask = 0xFF000000 (4 bytes)
     * 70  Color space type = 0x206E6957 ?? (4 bytes) //LCS_WINDOWS_COLOR_SPACE
     * 74  CIEXYZTRIPLE Color Space (unused) (36 bytes)
     * 110 red gamma = unused (4 bytes)
     * 114 green gamma = unused (4 bytes)
     * 118 blue gamma = unused (4 bytes)
     * 122 <Pixel Data>
     */

    //TODO endianess

    //const boost::uint8_t unused_8 = 0;
    //const boost::uint16_t unused_16 = 0;
    const boost::uint32_t unused_32 = 0;

    //File Header
    out.write("\x42\x4d", 2); //magic number

    const boost::uint32_t file_size = width*height*4 + 122; 
    out.write( (const char *)(&file_size), 4); //file_size

    out.write( (const char *)(&unused_32), 4); //unused

    const boost::uint32_t pixel_array_offset = 122;
    out.write( (const char *)(&pixel_array_offset), 4); //pixel_array_offset

    const boost::uint32_t dib_header_size = 108;
    out.write( (const char *)(&dib_header_size), 4); //dib_header_size

    const boost::uint32_t bitmap_width = width;
    const boost::uint32_t bitmap_height = height;
    out.write( (const char *)(&bitmap_width), 4); //bitmap_width
    out.write( (const char *)(&bitmap_height), 4); //bitmap_height

    const boost::uint16_t color_planes = 1;
    out.write( (const char *)(&color_planes), 2); //color_planes

    const boost::uint16_t bits_per_pixel = 32;
    out.write( (const char *)(&bits_per_pixel), 2); //bits_per_pixel

    const boost::uint32_t bitfields = 3;
    out.write( (const char *)(&bitfields), 4); //bitfields

    const boost::uint32_t pixel_array_size = width*height*4;
    out.write( (const char *)(&pixel_array_size), 4); //pixel_array_size

    const boost::uint32_t horizontal_physical_resolution = 2835;
    const boost::uint32_t vertical_physical_resolution = 2835;
    out.write( (const char *)(&horizontal_physical_resolution), 4); //horizontal_physical_resolution
    out.write( (const char *)(&vertical_physical_resolution), 4); //vertical_physical_resolution

    out.write( (const char *)(&unused_32), 4); //num of colors on palette
    out.write( (const char *)(&unused_32), 4); //num of important colors

    const boost::uint32_t red_channel_bit_mask = bmp_impl::endian_swap( 0x00FF0000U );
    const boost::uint32_t green_channel_bit_mask = bmp_impl::endian_swap( 0x0000FF00U );
    const boost::uint32_t blue_channel_bit_mask = bmp_impl::endian_swap( 0x000000FFU );
    const boost::uint32_t alpha_channel_bit_mask = bmp_impl::endian_swap( 0xFF000000U );
    out.write( (const char *)(&red_channel_bit_mask), 4); //red_channel_bit_mask    
    out.write( (const char *)(&green_channel_bit_mask), 4); //green_channel_bit_mask
    out.write( (const char *)(&blue_channel_bit_mask), 4); //blue_channel_bit_mask
    out.write( (const char *)(&alpha_channel_bit_mask), 4); //alpha_channel_bit_mask

    const boost::uint32_t color_space_type = bmp_impl::endian_swap( 0x206E6957U );
    out.write( (const char *)(&color_space_type), 4); //color_space_type

    //CIEXYZTRIPLE Color Space (unused) (36 bytes)
    for(unsigned i = 0; i < 36/4; ++i) {
        out.write( (const char *)(&unused_32), 4);
    }

    out.write( (const char *)(&unused_32), 4); //red gamma
    out.write( (const char *)(&unused_32), 4); //green gamma
    out.write( (const char *)(&unused_32), 4); //blue gamma

    for(unsigned cy = 0; cy < pixels.height(); ++cy) {
        for(unsigned x = 0; x < pixels.width(); ++x) {

            unsigned y = pixels.height() - cy - 1;
           
            const single_color_t alpha = get_alpha(pixels(x, y));
            const single_color_t red = get_red(pixels(x, y));
            const single_color_t green = get_green(pixels(x, y));
            const single_color_t blue = get_blue(pixels(x, y));
            
            out.write((const char *)&alpha, 1);
            out.write((const char *)&red, 1);
            out.write((const char *)&green, 1);
            out.write((const char *)&blue, 1);
            
            

        }
        //no padding required as the pixels themselves are 4 bytes long
    }

    out.close();

    return 0;
}

void bmp_image::antialiased_line_helper_plot(int x, int y, rgba_color_t color, double brightness, bool reversed) {
    if (reversed) {
        plot_with_brightness(y,x,color,brightness);
    } else {
        plot_with_brightness(x,y,color,brightness);
    }
}

namespace bmp_impl {

boost::uint16_t endian_swap(const boost::uint16_t x) {
    return (x>>8) | (x<<8);
}

boost::uint32_t endian_swap(const boost::uint32_t x) {
    return (x>>24) | 
        ((x<<8) & 0x00FF0000) |
        ((x>>8) & 0x0000FF00) |
        (x<<24);
}


boost::uint64_t endian_swap(const boost::uint64_t x) {
    return (x>>56) | 
        ((x<<40) & 0x00FF000000000000) |
        ((x<<24) & 0x0000FF0000000000) |
        ((x<<8)  & 0x000000FF00000000) |
        ((x>>8)  & 0x00000000FF000000) |
        ((x>>24) & 0x0000000000FF0000) |
        ((x>>40) & 0x000000000000FF00) |
        (x<<56);
}

} //namespace bmp_impl

} //namespace r0
