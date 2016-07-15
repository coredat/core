#ifndef OVERLAPPING_AABB_INCLUDED_829931D8_5640_43B3_A480_76038E5373A3
#define OVERLAPPING_AABB_INCLUDED_829931D8_5640_43B3_A480_76038E5373A3


#include <math/geometry/geometry_types.hpp>
#include <math/transform/transform_types.hpp>
#include <systems/physics_engine/collision/axis_collidable.hpp>
#include <utilities/generic_id.hpp>
#include <stdint.h>


namespace Transformation {


/*

*/
void
get_overlapping(const util::generic_id ids[],
                const uint64_t collision_ids[],
                const math::aabb colliders[],
                const math::transform transforms[],
                const size_t number_of_bounds,
                Physics::Collision::Axis_collidable *out_axis_collidables[],
                util::generic_id *out_ids[],
                size_t *out_size);


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