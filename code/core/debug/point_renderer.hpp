#ifndef POINT_RENDERER_INCLUDED_88DE1535_F824_417B_A97C_0DCAF5E94A6B
#define POINT_RENDERER_INCLUDED_88DE1535_F824_417B_A97C_0DCAF5E94A6B


#include <core/common/core_fwd.hpp>
#include <core/common/core_types.hpp>


namespace Core {
namespace Debug {


/*!
  Draw a point with a color.
  This may perform subpar, its only meaent as an aide for debugging.
*/
void
draw_point(const math::vec3 pos, const Core::Color color);


} // ns
} // ns


#endif // inc guard