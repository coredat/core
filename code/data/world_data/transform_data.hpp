
/*
 Warning: This file is auto generated. Any changes could be overwritten!
*/

#ifndef TRANSFORM_DATA_INCLUDED_56435B4C_2003_45F9_B3B2_3289BB50567E
#define TRANSFORM_DATA_INCLUDED_56435B4C_2003_45F9_B3B2_3289BB50567E


#include <utilities/generic_id.hpp>
#include <math/transform/transform.hpp>
#include <math/geometry/aabb.hpp>
#include <stddef.h>
#include <utilities/memory_pool.hpp>


namespace World_data {


struct Transform_data
{
  // Index keys;
  util::generic_id *data_key = nullptr;
  // Properties;
  math::transform *property_transform = nullptr;
  math::aabb *property_aabb = nullptr;
  // Size and capacity;
  size_t size = 0;
  const size_t capacity = 0;
  //Memory Pool;
  const util::memory_chunk memory = util::memory_chunk();
};


// Insert a new entry to the resource
bool
transform_data_push_back(Transform_data *data, const util::generic_id key);


// Remove an entry from the resource
bool
transform_data_erase(Transform_data *data, const util::generic_id key);


// Checks to see if a entry exists for the key
bool
transform_data_exists(const Transform_data *data, const util::generic_id key, size_t *out_index = nullptr);


// Is data valid.
bool
transform_data_is_valid(const Transform_data *data);


// Locks the resources
void
data_lock(Transform_data *data);


// Unlocks the resource
void
data_unlock(Transform_data *data);


bool
transform_data_get_transform(const Transform_data *data, const util::generic_id key, math::transform *out_value);


bool
transform_data_set_transform(Transform_data *data, const util::generic_id key, const math::transform *value);


bool
transform_data_get_aabb(const Transform_data *data, const util::generic_id key, math::aabb *out_value);


bool
transform_data_set_aabb(Transform_data *data, const util::generic_id key, const math::aabb *value);


// Current size of the data store.
size_t
transform_data_get_size(const Transform_data *data);


// Current capacity of the data store.
size_t
transform_data_get_capacity(const Transform_data *data);


// Initilizes the data resource.
void
transform_data_init(Transform_data *data, const size_t size_hint);


// Frees the resources.
void
transform_data_free(Transform_data *data);


} // ns


#endif // inc guard