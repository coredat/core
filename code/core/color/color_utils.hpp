#ifndef COLOR_UTILS_INCLUDED_8DC54AB5_90F6_4A6B_B758_AE55942043C2
#define COLOR_UTILS_INCLUDED_8DC54AB5_90F6_4A6B_B758_AE55942043C2


#include <core/color/color.hpp>
#include <stdint.h>


namespace Core {
namespace Color_utils {


float       get_red_f(const Color color);
float       get_green_f(const Color color);
float       get_blue_f(const Color color);
float       get_alpha_f(const Color color);


uint8_t     get_red_i(const Color color);
uint8_t     get_green_i(const Color color);
uint8_t     get_blue_i(const Color color);
uint8_t     get_alpha_i(const Color color);


} // ns
} // ns


#endif // inc guard