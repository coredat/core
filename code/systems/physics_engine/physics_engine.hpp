#ifndef PHYSICS_ENGINE_INCLUDED_937163DD_0E64_4054_85E9_C6415B66AE1A
#define PHYSICS_ENGINE_INCLUDED_937163DD_0E64_4054_85E9_C6415B66AE1A


#include <math/math.hpp>
#include <core/entity_id.hpp>


namespace Physics_engine {


struct Collision_pair
{
  Core::Entity_id obj_a;
  Core::Entity_id obj_b;
};


void
get_collisions(
  const Core::Entity_id ids[],
  const math::transform transforms[],
  const math::aabb aabbs[],
  const uint32_t number_of_entities_to_test,
  Collision_pair out_collision[],
  const uint32_t size_of_out_buffer,
  uint32_t *number_of_collisions
);


} // ns


#endif // inc guardt