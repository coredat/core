#ifndef COLOR_UTILS_INCLUDED_8DC54AB5_90F6_4A6B_B758_AE55942043C2
#define COLOR_UTILS_INCLUDED_8DC54AB5_90F6_4A6B_B758_AE55942043C2


#include <core/common/core_fwd.hpp>
#include <stdint.h>


namespace Core {
namespace Color_utils {


/*
  Get a component as a float.
*/
float       get_red_f(const Color color);
float       get_green_f(const Color color);
float       get_blue_f(const Color color);
float       get_alpha_f(const Color color);


/*
  Get the components as an array.
*/
void        to_float_array(const Color color, float *out_array);


/*
  Get a component as a int8
*/
uint8_t     get_red_i(const Color color);
uint8_t     get_green_i(const Color color);
uint8_t     get_blue_i(const Color color);
uint8_t     get_alpha_i(const Color color);


} // ns
} // ns


#endif // inc guard