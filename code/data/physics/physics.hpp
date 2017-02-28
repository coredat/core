#ifndef PHYSICS_INCLUDED_07C2F5DF_B376_4BF3_ACC4_3B9BA46D879F
#define PHYSICS_INCLUDED_07C2F5DF_B376_4BF3_ACC4_3B9BA46D879F


#include "fwd.hpp"
#include "config_physics.hpp"
#include <math/math.hpp>
#include <utilities/alloc.hpp>
#include <stdint.h>
#include <stdlib.h>


namespace Data {
namespace Physics {


// ---------------------------------------------------[ Lifetime Management ]--


Physics_data*
initialize(
  const Physics_config config = physics_default(),
  const lib::malloc_fn malloc_fn = malloc,
  const lib::realloc_fn realloc_fn = realloc,
  const lib::free_fn free_fn = free);
  
  
bool
destroy(Physics_data *phys);


// ------------------------------------------------------[ World Management ]--


void
world_set_gravity(Physics_data *phys, const math::vec3 gravity);


void
world_think(Physics_data *phys);


void*
world_get_colliding_items(Physics_data *phys);


void*
world_find_with_ray(
  Physics_data *data,
  const math::vec3 origin,
  const math::vec3 direction,
  const uint32_t max_return = -1);


// --------------------------------------------------[ Rigidbody Management ]--


void
rigidbody_add(
  Physics_data *phys,
  const uint32_t id,
  const Rigidbody_config *rb,
  const Collider_config *collider,
  const math::transform *transform,
  const math::aabb *aabb
);


void
rigidbody_update(
  Physics_data *phys,
  const uint32_t id,
  const Rigidbody_config *rb = nullptr,
  const Collider_config *collider = nullptr,
  const math::transform *transform = nullptr,
  const math::aabb *aabb = nullptr
);


void
rigidbody_remove(Physics_data *phys, const uint32_t id);


// ----------------------------------------------------[ Trigger Management ]--


void
trigger_add(
  Physics_data *phys,
  const uint32_t id,
  const Trigger_config *trig,
  const Collider_config *collider,
  const math::transform *transform,
  const math::aabb *aabb
);


void
trigger_remove(Physics_data *phys, const uint32_t id);


void
trigger_udpate(
  Physics_data *phys,
  const uint32_t id,
  const Trigger_config *trig = nullptr,
  const Collider_config *collider = nullptr,
  const math::transform *transform = nullptr,
  const math::aabb *aabb = nullptr
);


} // ns
} // ns


#endif // inc guard
