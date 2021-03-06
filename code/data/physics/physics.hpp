#ifndef PHYSICS_INCLUDED_07C2F5DF_B376_4BF3_ACC4_3B9BA46D879F
#define PHYSICS_INCLUDED_07C2F5DF_B376_4BF3_ACC4_3B9BA46D879F


#include "fwd.hpp"
#include "config_physics.hpp"
#include <math/fwd.hpp>
#include <utilities/alloc.hpp>
#include <stdint.h>
#include <stdlib.h>


namespace Data {
namespace Physics {


// ------------------------------------------------------------ [ Callbacks ]--


using draw_line_fn = void(*)(
  const math::vec3 start,
  const math::vec3 end,
  const math::vec3 color);


using draw_point_fn = void(*)(
  const math::vec3 point,
  const math::vec3 color);


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


size_t
world_get_number_of_colliding_items(const Physics_data *phys);


const Contact*
world_get_colliding_items(const Physics_data *phys);


void
world_find_with_ray(
  Physics_data *phys,
  const math::vec3 origin,
  const math::vec3 scaled_direction,
  Contact *out_contacts,
  const uint32_t contacts_count,
  uint32_t *out_number_of_contacts);


// ------------------------------------------------------------ [ Debugging ]--


void
debug_draw_bounding_boxes(Physics_data *phys, const bool set);


void
debug_draw_wireframes(Physics_data *phys, const bool set);


void
debug_draw_contact_points(Physics_data *phys, const bool set);


void
debug_draw_line_cb(Physics_data *phys, const draw_line_fn cb);


void
debug_draw_point_cb(Physics_data *phys, const draw_point_fn cb);


// ------------------------------------------------------[ Transform Setter ]--


void
transform_set(
  Physics_data *phys,
  const uint32_t id,
  const math::transform *transform
);


void
transform_callback_id(Physics_data *phys, const uint32_t callback_id);


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


void
rigidbody_apply_force(
  const Physics_data *phys,
  const uint32_t id,
  const math::vec3 world_direction,
  const float force
);


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
