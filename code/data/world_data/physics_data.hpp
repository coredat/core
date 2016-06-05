
/*
  Warning:
  This file is auto_generated any changes here may be overwritten.
*/

#ifndef PHYSICS_DATA_INCLUDED_46688CF0_3990_4441_98A3_4AA045AE6C68
#define PHYSICS_DATA_INCLUDED_46688CF0_3990_4441_98A3_4AA045AE6C68


#include <utilities/generic_id.hpp>
#include <utilities/memory_pool.hpp>
#include <stddef.h>
#include <math/transform/transform.hpp>
#include <math/geometry/aabb.hpp>
#include <stdint.h>


namespace World_data {





struct Physics_data
{
  // Data Key
  util::generic_id *data_key = nullptr;

  // Properties
  math::transform *property_transform = nullptr;
  math::aabb *property_aabb_collider = nullptr;
  uint64_t *property_collision_id = nullptr;

  // Size information
  size_t size = 0;
  const size_t capacity = 0;

  // Memory chunk
  const util::memory_chunk memory = util::memory_chunk();

}; // struct


void
physics_data_init(Physics_data *data, const size_t size_hint);


void
physics_data_free(Physics_data *data);


size_t
physics_data_get_size(const Physics_data *data);


size_t
physics_data_get_capacity(const Physics_data *data);


void
data_lock(Physics_data *data);


void
data_unlock(Physics_data *data);


bool
physics_data_push_back(Physics_data *data, const util::generic_id key, size_t *out_index = nullptr);


bool
physics_data_erase(Physics_data *data, const util::generic_id key);


bool
physics_data_exists(const Physics_data *data, const util::generic_id key, size_t *out_index = nullptr);


bool
physics_data_get_property_transform(const Physics_data *data, const util::generic_id key, math::transform *value);


bool
physics_data_set_property_transform(Physics_data *data,  const util::generic_id key, const math::transform value);


bool
physics_data_get_property_aabb_collider(const Physics_data *data, const util::generic_id key, math::aabb *value);


bool
physics_data_set_property_aabb_collider(Physics_data *data,  const util::generic_id key, const math::aabb value);


bool
physics_data_get_property_collision_id(const Physics_data *data, const util::generic_id key, uint64_t *value);


bool
physics_data_set_property_collision_id(Physics_data *data,  const util::generic_id key, const uint64_t value);


} // ns


#endif // inc guard
