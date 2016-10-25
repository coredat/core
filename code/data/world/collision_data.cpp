
/*
  Warning
  --
  This file is auto generated changes here may be overwritten.
*/


#include "collision_data.hpp"
#include <assert.h>
#include <cstring>


namespace Data {


// ====================== //
// COMMON MODEL FUNCTIONS //
// ====================== //

namespace {

// Key our keys unique.
uint32_t collision_model_key_instance_number{0};

} // anon ns


void
data_lock(const Collision_data *data)
{
  // Not yet impl.
}


void
data_unlock(const Collision_data *data)
{
  // Not yet impl.
}


bool
collision_create(Collision_data *data, const size_t size_hint)
{
  assert(data);
  assert(size_hint);

  // Size up the capacity
  {
    size_t *capacity = const_cast<size_t*>(&data->capacity);
    *capacity = size_hint;
  }

  // Allocate memory
  bool all_alloc = true;
  {
    // Alloc keys
    if(all_alloc)
    {
      data->keys = new uint32_t[size_hint];
      assert(data->keys);
      if(!data->keys) { all_alloc = false; }
      else { memset(data->keys, 0, sizeof(uint32_t) * size_hint); }
    }

    // Alloc space for entity_pair
    if(all_alloc)
    {
      data->field_entity_pair = new uint64_t[size_hint * 1];
      assert(data->field_entity_pair);
      if(!data->field_entity_pair) { all_alloc = false; }
      else { memset(data->field_entity_pair, 0, sizeof(uint64_t) * size_hint * 1); }
    }

    // Alloc space for penitration
    if(all_alloc)
    {
      data->field_penitration = new float[size_hint * 1];
      assert(data->field_penitration);
      if(!data->field_penitration) { all_alloc = false; }
      else { memset(data->field_penitration, 0, sizeof(float) * size_hint * 1); }
    }

    // Alloc space for position
    if(all_alloc)
    {
      data->field_position = new math::vec3[size_hint * 1];
      assert(data->field_position);
      if(!data->field_position) { all_alloc = false; }
      else { memset(data->field_position, 0, sizeof(math::vec3) * size_hint * 1); }
    }

    // Alloc space for normal
    if(all_alloc)
    {
      data->field_normal = new math::vec3[size_hint * 1];
      assert(data->field_normal);
      if(!data->field_normal) { all_alloc = false; }
      else { memset(data->field_normal, 0, sizeof(math::vec3) * size_hint * 1); }
    }
  }

  // Failed so clean up.
  if(!all_alloc)
  {
    collision_destroy(data);
  }

  return all_alloc;
}


void
collision_destroy(Collision_data *data)
{
  // Free up the memory.
  {
    // Remove keys
    if(data->keys) { delete[] data->keys; }
    data->keys = nullptr;

    // Remove entity_pair
    if(data->field_entity_pair) { delete[] data->field_entity_pair; }
    data->field_entity_pair = nullptr;

    // Remove penitration
    if(data->field_penitration) { delete[] data->field_penitration; }
    data->field_penitration = nullptr;

    // Remove position
    if(data->field_position) { delete[] data->field_position; }
    data->field_position = nullptr;

    // Remove normal
    if(data->field_normal) { delete[] data->field_normal; }
    data->field_normal = nullptr;
  }

  // Zero capacity and size
  {
    data->size = 0;

    size_t *capacity = const_cast<size_t*>(&data->capacity);
    *capacity = 0;
  }
}


uint32_t
collision_push(Collision_data *data)
{
  assert(data);
  assert(data->keys);

  // Do we need to resize?
  if(data->size >= data->capacity)
  {
    collision_resize_capacity(data, data->capacity << 1);
  }

  // Push key at the back
  {
    const uint32_t new_key = ++collision_model_key_instance_number;
    data->keys[data->size++] = new_key;

    return new_key;
  }


  return 0;
}



bool
collision_remove(Collision_data *data, const uint32_t key)
{
  size_t index_to_erase = 0;

  if(collision_exists(data, key, &index_to_erase))
  {
    assert(index_to_erase < data->size);

    const size_t start_index = index_to_erase + 1;
    const size_t size_to_end = data->size - index_to_erase - 1;

    --(data->size);

    // Shuffle the data down
    memmove(&data->keys[index_to_erase], &data->keys[start_index], size_to_end * sizeof(*data->keys));
    memmove(&data->field_entity_pair[index_to_erase * 1], &data->field_entity_pair[start_index * 1], size_to_end * sizeof(*data->field_entity_pair) * 1);
    memmove(&data->field_penitration[index_to_erase * 1], &data->field_penitration[start_index * 1], size_to_end * sizeof(*data->field_penitration) * 1);
    memmove(&data->field_position[index_to_erase * 1], &data->field_position[start_index * 1], size_to_end * sizeof(*data->field_position) * 1);
    memmove(&data->field_normal[index_to_erase * 1], &data->field_normal[start_index * 1], size_to_end * sizeof(*data->field_normal) * 1);

    return true;
  }

  return false;
}


bool
collision_exists(const Collision_data *data, const uint32_t key, size_t *out_index)
{
  assert(data);
  assert(data != 0);

  for(size_t i = 0; i < data->size; ++i)
  {
    if(data->keys[i] == key)
    {
      if(out_index)
      {
        *out_index = i;
      }

      return true;
    }
  }

  return false;
}


void
collision_clear(Collision_data *data)
{
  assert(data);

  data->size = 0;
}


bool
collision_is_empty(const Collision_data *data, const size_t size_hint)
{
  assert(data);

  if(!data) { return false; }

  return !!data->size;
}


size_t
collision_get_size(const Collision_data *data)
{
  assert(data);

  if(!data) { return 0; }

  return data->size;
}


size_t
collision_get_capacity(const Collision_data *data)
{
  assert(data);

  if(!data) { return 0; }

  return data->capacity;
}


bool
collision_resize_capacity(Collision_data *data, const size_t size_hint)
{
  assert(data);
  assert(size_hint > data->size); // Will slice data

  // Create new data.
  Collision_data new_data;
  const bool created_new = collision_create(&new_data, size_hint);

  // Failed to resize.
  if(!created_new)
  {
    collision_destroy(&new_data);
    return false;
  }

  // Copy over data
  {
    memcpy(new_data.keys, data->keys, sizeof(uint32_t) * data->size);
    memcpy(new_data.field_entity_pair, data->field_entity_pair, sizeof(uint64_t) * data->size * 1);
    memcpy(new_data.field_penitration, data->field_penitration, sizeof(float) * data->size * 1);
    memcpy(new_data.field_position, data->field_position, sizeof(math::vec3) * data->size * 1);
    memcpy(new_data.field_normal, data->field_normal, sizeof(math::vec3) * data->size * 1);
  }

  // Swap ptrs
  {
    uint32_t *old_keys = data->keys;
    data->keys = new_data.keys;
    new_data.keys = old_keys;

    uint64_t *old_entity_pair = data->field_entity_pair;
    data->field_entity_pair = new_data.field_entity_pair;
    new_data.field_entity_pair = old_entity_pair;

    float *old_penitration = data->field_penitration;
    data->field_penitration = new_data.field_penitration;
    new_data.field_penitration = old_penitration;

    math::vec3 *old_position = data->field_position;
    data->field_position = new_data.field_position;
    new_data.field_position = old_position;

    math::vec3 *old_normal = data->field_normal;
    data->field_normal = new_data.field_normal;
    new_data.field_normal = old_normal;
  }

  // Set the Capacity
  {
    size_t *capacity = const_cast<size_t*>(&data->capacity);
    *capacity = new_data.capacity;
  }

  // Destroy new data
  collision_destroy(&new_data);

  return true;
}



// ===================== //
// DATA GETTER FUNCTIONS //
// ===================== //


const uint64_t*
collision_get_const_entity_pair_data(const Collision_data *data)
{
  assert(data);
  assert(data->field_entity_pair);

  return data->field_entity_pair;
}


uint64_t*
collision_get_entity_pair_data(Collision_data *data)
{
  assert(data);
  assert(data->field_entity_pair);

  return data->field_entity_pair;
}


const float*
collision_get_const_penitration_data(const Collision_data *data)
{
  assert(data);
  assert(data->field_penitration);

  return data->field_penitration;
}


float*
collision_get_penitration_data(Collision_data *data)
{
  assert(data);
  assert(data->field_penitration);

  return data->field_penitration;
}


const math::vec3*
collision_get_const_position_data(const Collision_data *data)
{
  assert(data);
  assert(data->field_position);

  return data->field_position;
}


math::vec3*
collision_get_position_data(Collision_data *data)
{
  assert(data);
  assert(data->field_position);

  return data->field_position;
}


const math::vec3*
collision_get_const_normal_data(const Collision_data *data)
{
  assert(data);
  assert(data->field_normal);

  return data->field_normal;
}


math::vec3*
collision_get_normal_data(Collision_data *data)
{
  assert(data);
  assert(data->field_normal);

  return data->field_normal;
}


// =============== //
// FIELD FUNCTIONS //
// =============== //


bool
collision_get_entity_pair(const Collision_data *data, const uint32_t key, uint64_t *return_value)
{
  assert(data);
  assert(key != 0);
  assert(data->field_entity_pair);
  assert(return_value);

  // Search for its index.
  // If we find it we can return the value.

  size_t index = 0;

  if(collision_exists(data, key, &index))
  {
    assert(index < data->size);

    if(index < data->size)
    {
      *return_value = data->field_entity_pair[index];

      return true;
    }
  }

  return false;
}


bool
collision_set_entity_pair(const Collision_data *data, const uint32_t key, const uint64_t *set_value)
{
  assert(data);
  assert(key != 0);
  assert(data->field_entity_pair);
  assert(set_value);

  // Search for its index.
  // If we find it we can set the value.

  size_t index = 0;

  if(collision_exists(data, key, &index))
  {
    assert(index < data->size);
    if(index < data->size)
    {
      data->field_entity_pair[index] = *set_value;

      return true;
    }
  }

  return false;
}


bool
collision_get_penitration(const Collision_data *data, const uint32_t key, float *return_value)
{
  assert(data);
  assert(key != 0);
  assert(data->field_penitration);
  assert(return_value);

  // Search for its index.
  // If we find it we can return the value.

  size_t index = 0;

  if(collision_exists(data, key, &index))
  {
    assert(index < data->size);

    if(index < data->size)
    {
      *return_value = data->field_penitration[index];

      return true;
    }
  }

  return false;
}


bool
collision_set_penitration(const Collision_data *data, const uint32_t key, const float *set_value)
{
  assert(data);
  assert(key != 0);
  assert(data->field_penitration);
  assert(set_value);

  // Search for its index.
  // If we find it we can set the value.

  size_t index = 0;

  if(collision_exists(data, key, &index))
  {
    assert(index < data->size);
    if(index < data->size)
    {
      data->field_penitration[index] = *set_value;

      return true;
    }
  }

  return false;
}


bool
collision_get_position(const Collision_data *data, const uint32_t key, math::vec3 *return_value)
{
  assert(data);
  assert(key != 0);
  assert(data->field_position);
  assert(return_value);

  // Search for its index.
  // If we find it we can return the value.

  size_t index = 0;

  if(collision_exists(data, key, &index))
  {
    assert(index < data->size);

    if(index < data->size)
    {
      *return_value = data->field_position[index];

      return true;
    }
  }

  return false;
}


bool
collision_set_position(const Collision_data *data, const uint32_t key, const math::vec3 *set_value)
{
  assert(data);
  assert(key != 0);
  assert(data->field_position);
  assert(set_value);

  // Search for its index.
  // If we find it we can set the value.

  size_t index = 0;

  if(collision_exists(data, key, &index))
  {
    assert(index < data->size);
    if(index < data->size)
    {
      data->field_position[index] = *set_value;

      return true;
    }
  }

  return false;
}


bool
collision_get_normal(const Collision_data *data, const uint32_t key, math::vec3 *return_value)
{
  assert(data);
  assert(key != 0);
  assert(data->field_normal);
  assert(return_value);

  // Search for its index.
  // If we find it we can return the value.

  size_t index = 0;

  if(collision_exists(data, key, &index))
  {
    assert(index < data->size);

    if(index < data->size)
    {
      *return_value = data->field_normal[index];

      return true;
    }
  }

  return false;
}


bool
collision_set_normal(const Collision_data *data, const uint32_t key, const math::vec3 *set_value)
{
  assert(data);
  assert(key != 0);
  assert(data->field_normal);
  assert(set_value);

  // Search for its index.
  // If we find it we can set the value.

  size_t index = 0;

  if(collision_exists(data, key, &index))
  {
    assert(index < data->size);
    if(index < data->size)
    {
      data->field_normal[index] = *set_value;

      return true;
    }
  }

  return false;
}


} // Data ns
