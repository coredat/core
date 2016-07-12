#ifndef RENDERER_AABB_INCLUDED_40F10446_9595_41DC_9139_C5F3AD81275F
#define RENDERER_AABB_INCLUDED_40F10446_9595_41DC_9139_C5F3AD81275F


#include <math/geometry/geometry_types.hpp>
#include <stdint.h>


/*!
  Renders a box.
  Mainly good for debugging.
*/
namespace Aabb_renderer {


void
initialize();


void
render(const math::aabb boxes[],
       const uint32_t number_of_boxes);


} // ns


#endif // inc guard