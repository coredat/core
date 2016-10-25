
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

    // Alloc space for collision_point
    if(all_alloc)
    {
      data->field_collision_point = new Physics_transform::Collision_point[size_hint * 1];
      assert(data->field_collision_point);
      if(!data->field_collision_point) { all_alloc = false; }
      else { memset(data->field_collision_point, 0, sizeof(Physics_transform::Collision_point) * size_hint * 1); }
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

    // Remove collision_point
    if(data->field_collision_point) { delete[] data->field_collision_point; }
    data->field_collision_point = nullptr;
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
    memmove(&data->field_collision_point[index_to_erase * 1], &data->field_collision_point[start_index * 1], size_to_end * sizeof(*data->field_collision_point) * 1);

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
    memcpy(new_data.field_collision_point, data->field_collision_point, sizeof(Physics_transform::Collision_point) * data->size * 1);
  }

  // Swap ptrs
  {
    uint32_t *old_keys = data->keys;
    data->keys = new_data.keys;
    new_data.keys = old_keys;

    uint64_t *old_entity_pair = data->field_entity_pair;
    data->field_entity_pair = new_data.field_entity_pair;
    new_data.field_entity_pair = old_entity_pair;

    Physics_transform::Collision_point *old_collision_point = data->field_collision_point;
    data->field_collision_point = new_data.field_collision_point;
    new_data.field_collision_point = old_collision_point;
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


const Physics_transform::Collision_point*
collision_get_const_collision_point_data(const Collision_data *data)
{
  assert(data);
  assert(data->field_collision_point);

  return data->field_collision_point;
}


Physics_transform::Collision_point*
collision_get_collision_point_data(Collision_data *data)
{
  assert(data);
  assert(data->field_collision_point);

  return data->field_collision_point;
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
collision_get_collision_point(const Collision_data *data, const uint32_t key, Physics_transform::Collision_point *return_value)
{
  assert(data);
  assert(key != 0);
  assert(data->field_collision_point);
  assert(return_value);

  // Search for its index.
  // If we find it we can return the value.

  size_t index = 0;

  if(collision_exists(data, key, &index))
  {
    assert(index < data->size);

    if(index < data->size)
    {
      *return_value = data->field_collision_point[index];

      return true;
    }
  }

  return false;
}


bool
collision_set_collision_point(const Collision_data *data, const uint32_t key, const Physics_transform::Collision_point *set_value)
{
  assert(data);
  assert(key != 0);
  assert(data->field_collision_point);
  assert(set_value);

  // Search for its index.
  // If we find it we can set the value.

  size_t index = 0;

  if(collision_exists(data, key, &index))
  {
    assert(index < data->size);
    if(index < data->size)
    {
      data->field_collision_point[index] = *set_value;

      return true;
    }
  }

  return false;
}


} // Data ns
