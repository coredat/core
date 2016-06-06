#ifndef OVERLAPPING_AABB_INCLUDED_829931D8_5640_43B3_A480_76038E5373A3
#define OVERLAPPING_AABB_INCLUDED_829931D8_5640_43B3_A480_76038E5373A3


#include <math/geometry/geometry_types.hpp>
#include <math/transform/transform_types.hpp>
#include <stdint.h>


namespace Transformation {


/*
  Takes untransformed aabbs and applys a transform to them.
  All input and output arrays must be the same length.
*/
void
calculate_positional_aabb(const math::aabb aabb[],
                          const math::transform transform[],
                          math::aabb out_aabb[],
                          const uint32_t count);


void
find_overlapping_aabbs();


} // ns


#endif // inc guard