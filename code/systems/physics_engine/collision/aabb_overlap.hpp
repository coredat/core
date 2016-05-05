#ifndef AABB_OVERLAP_INCLUDED_C5ABC9A2_4B95_4D21_92C5_5CB0CBAD6D18
#define AABB_OVERLAP_INCLUDED_C5ABC9A2_4B95_4D21_92C5_5CB0CBAD6D18


#include "collision_fwd.hpp"
#include <math/geometry/geometry_types.hpp>
#include <stdint.h>


namespace Physics {
namespace Collision {


/*!
  Calculates which aabb's are overlapping.
  This will take the Axis_collidable mask into account.
  \param out_pairs The result.
  \param axis_colliders[] The aabb's and masks to test.
  \param number_of_colliders The size of the axis_colliders.
*/
void
aabb_calculate_overlaps_pairs(Pairs *out_pairs,
                              const Axis_collidable axis_colliders[],
                              const uint32_t number_of_colliders);


} // ns
} // ns


#endif // inc guard