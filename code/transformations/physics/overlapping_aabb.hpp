#ifndef OVERLAPPING_AABB_INCLUDED_829931D8_5640_43B3_A480_76038E5373A3
#define OVERLAPPING_AABB_INCLUDED_829931D8_5640_43B3_A480_76038E5373A3


#include <math/geometry/geometry_types.hpp>
#include <math/transform/transform_types.hpp>
#include <systems/physics_engine/collision/axis_collidable.hpp>
#include <stdint.h>


namespace Transformation {


/*
  Returns a list of ids that whoes aabbs are overlapping.
  the results are only valid for one frame.
*/
void
get_overlapping(const uint32_t ids[],
                const uint64_t collision_ids[],
                const math::aabb colliders[],
                const math::transform transforms[],
                const size_t number_of_bounds,
                Physics::Collision::Axis_collidable *volatile_out_axis_collidables[],
                uint32_t *volatile_out_ids[],
                size_t *volatile_out_size);


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
