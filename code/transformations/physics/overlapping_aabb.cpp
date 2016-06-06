#include <transformations/physics/overlapping_aabb.hpp>
#include <math/geometry/aabb.hpp>
#include <math/transform/transform.hpp>


namespace Transformation {


void
calculate_positional_aabb(const math::aabb aabb[],
                          const math::transform transform[],
                          math::aabb out_aabb[],
                          const uint32_t count)
{
  for(uint32_t i = 0; i < count; ++i)
  {
    math::aabb box_copy(aabb[i]);
    math::aabb_scale(box_copy, transform[i].scale);
    math::aabb_set_origin(box_copy, transform[i].position);
    
    out_aabb[i] = box_copy;
  }
}


void
calculate_overlapping_aabbs()
{
}


} // ns