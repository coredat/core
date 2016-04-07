#ifndef DEBUG_LINE_INCLUDED_478563A5_F3BC_4261_A07F_34F829755E95
#define DEBUG_LINE_INCLUDED_478563A5_F3BC_4261_A07F_34F829755E95


#include <math/vec/vec3.hpp>


namespace Renderer {


void debug_line(const float from[3], const float to[3], const float rgb_color[3]);
void debug_line(const math::vec3 from, const math::vec3 to, const math::vec3 rgb_color);


} // ns


#endif // inc