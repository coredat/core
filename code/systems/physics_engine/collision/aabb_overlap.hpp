#ifndef AABB_OVERLAP_INCLUDED_C5ABC9A2_4B95_4D21_92C5_5CB0CBAD6D18
#define AABB_OVERLAP_INCLUDED_C5ABC9A2_4B95_4D21_92C5_5CB0CBAD6D18


#include "collision_fwd.hpp"
#include <math/geometry/geometry_types.hpp>
#include <stdint.h>


namespace Physics {
namespace Collision {


void
aabb_calculate_overlaps_pairs(Pairs *out_pairs,
                              const math::aabb aabb[],
                              const uint32_t number_of_transforms);


} // ns
} // ns


#endif // inc guard