#include "../physics.hpp"
#include "physics_data.hpp"



namespace Data {
namespace Physics {


void
debug_draw_bounding_boxes(Physics_data *phys, const bool set)
{
  constexpr uint32_t flag = btIDebugDraw::DBG_DrawAabb;
  set ? phys->debug_drawer.debug_mode |= flag : phys->debug_drawer.debug_mode &= ~(flag);
}


void
debug_draw_wireframes(Physics_data *phys, const bool set)
{
  constexpr uint32_t flag = btIDebugDraw::DBG_DrawWireframe;
  set ? phys->debug_drawer.debug_mode |= flag : phys->debug_drawer.debug_mode &= ~(flag);
}


void
debug_draw_contact_points(Physics_data *phys, const bool set)
{
  constexpr uint32_t flag = btIDebugDraw::DBG_DrawContactPoints;
  set ? phys->debug_drawer.debug_mode |= flag : phys->debug_drawer.debug_mode &= ~(flag);
}


void
debug_draw_line_cb(Physics_data *phys, const draw_line_fn cb)
{
  phys->debug_drawer.draw_line_cb = cb;
}


void
debug_draw_point_cb(Physics_data *phys, const draw_point_fn cb)
{
  phys->debug_drawer.draw_point_cb = cb;
}


} // ns
} // ns
