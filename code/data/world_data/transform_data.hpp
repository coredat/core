
/*
  Warning:
  This file is auto_generated any changes here may be overwritten.
*/

#ifndef TRANSFORM_DATA_INCLUDED_D20FCABA_BE20_48C3_90CA_4703D98759F2
#define TRANSFORM_DATA_INCLUDED_D20FCABA_BE20_48C3_90CA_4703D98759F2


#include <utilities/generic_id.hpp>
#include <utilities/memory_pool.hpp>
#include <stddef.h>
#include <math/transform/transform.hpp>
#include <math/geometry/aabb.hpp>


namespace World_data {





struct Transform_data
{
  // Data Key
  util::generic_id *data_key = nullptr;

  // Properties
  math::transform *property_transform = nullptr;
  math::aabb *property_aabb = nullptr;

  // Size information
  size_t size = 0;
  const size_t capacity = 0;

  // Memory chunk
  const util::memory_chunk memory = util::memory_chunk();

}; // struct


void
transform_data_init(Transform_data *data, const size_t size_hint);


void
transform_data_free(Transform_data *data);


size_t
transform_data_get_size(const Transform_data *data);


size_t
transform_data_get_capacity(const Transform_data *data);


void
data_lock(Transform_data *data);


void
data_unlock(Transform_data *data);


bool
transform_data_push_back(Transform_data *data, const util::generic_id key, size_t *out_index = nullptr);


bool
transform_data_erase(Transform_data *data, const util::generic_id key);


bool
transform_data_exists(const Transform_data *data, const util::generic_id key, size_t *out_index = nullptr);


bool
transform_data_get_property_transform(const Transform_data *data, const util::generic_id key, math::transform *value);


bool
transform_data_set_property_transform(Transform_data *data,  const util::generic_id key, const math::transform value);


bool
transform_data_get_property_aabb(const Transform_data *data, const util::generic_id key, math::aabb *value);


bool
transform_data_set_property_aabb(Transform_data *data,  const util::generic_id key, const math::aabb value);


} // ns


#endif // inc guard
