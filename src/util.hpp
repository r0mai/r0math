
#ifndef UTIL_HPP_
#define UTIL_HPP_

#include <string>
#include <sstream>
#include <vector>
#include <cmath>
#include <iostream>

#include "mpirxx_include.hpp"

namespace r0 {

//PF should have a signature std::string print_function(C::value_type)
template<class C, class PF>
std::string container_to_string( const C& container, PF print_function, const std::string& delimiter = ", " ) {
    std::stringstream ss;

    if ( !container.empty() ) {
        typename C::const_iterator end = container.end();
        --end;
        for( typename C::const_iterator it = container.begin(); it != end; ++it) {
            ss << print_function(*it) << delimiter;
        }
        ss << print_function(container.back());
    }

    return ss.str();
}

//end is one past the end
template<class T>
std::vector<T> range_in(const T& begin, const T& end) {
    std::vector<T> res(end-begin);
    for ( T i = begin; i < end; ++i ) {
        res[i] = i;
    }
    return res;
}

inline int sign(const double d) {
    if ( d > 0.0 ) {
        return 1;
    } else if ( d < 0.0 ) {
        return -1;
    } else {
        return 0;
    }
}

inline int integer_part(double d) {
    return static_cast<int>(std::floor(d));
}

inline double fractional_part(double d) {
    return d - std::floor(d);
}

inline double right_fractional_part(double d) {
    return 1. - fractional_part(d);
}

inline int round_half_up(double d) {
    return static_cast<int>(std::floor(d + 0.5));
}

inline int round_half_away_from_zero(double d) {
    return static_cast<int>(sign(d)*std::floor(std::abs(d)+0.5));
}

inline double math_vector_length( double x, double y ) {
    return std::sqrt( x*x + y*y );
}

inline double math_dot_product( double a1, double a2, double b1, double b2 ) {
    return a1*b1 + a2*b2;
}

template<class T>
T clip_to_range(const T& n, const T& min, const T& max) {
    assert( min <= max );
    if ( n < min ) { 
        return min;
    }
    if ( n > max ) {
        return max;
    }
    return n;
}

//returns an iterator pointing to greatest element which is e <= value
//[begin, end) should be a shorted range
template<class Iterator, class T>
Iterator binary_find(Iterator begin, Iterator end, const T& value) {

    while ( end - begin > 1 ) {

        Iterator middle = begin + (end - begin) / 2;

        if ( *middle < value ) {
            begin = middle;
        } else if ( *middle > value ) {
            end = middle;
        } else { //middle == value
            return middle;
        }
    }

    return begin;
}

#define PRINT(x) std::cout << #x << " : " << x << std::endl

//line intersection (x1, y1)->(x2, y2) with (x3, y3)->(x4, y4)
//! not line segment intersection
template<class T>
bool line_intersection( 
    const T& x1, const T& y1,
    const T& x2, const T& y2,
    const T& x3, const T& y3, 
    const T& x4, const T& y4,
    T& resx, T& resy) 
{
   const T a = x1*y2 - y1*x2;
   const T b = x3*y4 - y3*x4;
   const T c = x1-x2;
   const T d = y1-y2;
   const T e = x3-x4;
   const T f = y3-y4;

   const T s = c*f - d*e;

   PRINT(a);
   PRINT(b);
   PRINT(c);
   PRINT(d);
   PRINT(e);
   PRINT(f);
   PRINT(s);
   PRINT((a*e - c*b) / s);
   PRINT((a*f - d*b) / s);

   if ( s == 0 ) {
       //parallel, the line could be equal
       return false;
   }

   

   resx = (a*e - c*b) / s;
   resy = (a*f - d*b) / s;

   return true;
}

#undef PRINT

} //namespace

#endif //UTIL_HPP_
