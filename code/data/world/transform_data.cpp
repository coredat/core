
/*
  Warning
  --
  This file is auto generated changes here may be overwritten.
*/


#include "transform_data.hpp"
#include <assert.h>
#include <cstring>


namespace Data {


// ====================== //
// COMMON MODEL FUNCTIONS //
// ====================== //


void
data_lock(const Transform_data *data)
{
  // Not yet impl.
}


void
data_unlock(const Transform_data *data)
{
  // Not yet impl.
}


bool
transform_create(Transform_data *data, const size_t size_hint)
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

    // Alloc space for transform
    if(all_alloc)
    {
      data->field_transform = new math::transform[size_hint * 1];
      assert(data->field_transform);
      if(!data->field_transform) { all_alloc = false; }
      else { memset(data->field_transform, 0, sizeof(math::transform) * size_hint * 1); }
    }

    // Alloc space for aabb
    if(all_alloc)
    {
      data->field_aabb = new math::aabb[size_hint * 1];
      assert(data->field_aabb);
      if(!data->field_aabb) { all_alloc = false; }
      else { memset(data->field_aabb, 0, sizeof(math::aabb) * size_hint * 1); }
    }
  }

  // Failed so clean up.
  if(!all_alloc)
  {
    transform_destroy(data);
  }

  return all_alloc;
}


void
transform_destroy(Transform_data *data)
{
  // Free up the memory.
  {
    // Remove keys
    if(data->keys) { delete[] data->keys; }
    data->keys = nullptr;

    // Remove transform
    if(data->field_transform) { delete[] data->field_transform; }
    data->field_transform = nullptr;

    // Remove aabb
    if(data->field_aabb) { delete[] data->field_aabb; }
    data->field_aabb = nullptr;
  }

  // Zero capacity and size
  {
    data->size = 0;

    size_t *capacity = const_cast<size_t*>(&data->capacity);
    *capacity = 0;
  }
}


uint32_t
transform_push(Transform_data *data, const uint32_t key)
{
  assert(data);
  assert(data->keys);

  // Do we need to resize?
  if(data->size >= data->capacity)
  {
    transform_resize_capacity(data, data->capacity << 1);
  }

  // Push key at the back
  {
    const uint32_t new_key = key;
    data->keys[data->size++] = new_key;

    return new_key;
  }


  return 0;
}

bool
transform_insert(Transform_data *data, const uint32_t key, const size_t insert_point)
{
  assert(data);
  assert(data->keys);
  assert(key);

  // Do we need to resize?
  if(data->size >= data->capacity)
  {
    transform_resize_capacity(data, data->capacity << 1);
  }

  // Shuffle memory up
  {
    const size_t insert_index = insert_point + 1;
    const size_t size_to_end = data->size - insert_point;

    // Shuffle the data down
    memmove(&data->keys[insert_index], &data->keys[insert_point], size_to_end * sizeof(*data->keys));
    memmove(&data->field_transform[insert_index * 1], &data->field_transform[insert_point * 1], size_to_end * sizeof(*data->field_transform) * 1);
    memmove(&data->field_aabb[insert_index * 1], &data->field_aabb[insert_point * 1], size_to_end * sizeof(*data->field_aabb) * 1);

    data->keys[insert_point] = key;
  }

  // Insert new data
  {
    ++data->size;
  }

  return true;
}


bool
transform_remove(Transform_data *data, const uint32_t key)
{
  size_t index_to_erase = 0;

  if(transform_exists(data, key, &index_to_erase))
  {
    assert(index_to_erase < data->size);

    const size_t start_index = index_to_erase + 1;
    const size_t size_to_end = data->size - index_to_erase - 1;

    --(data->size);

    // Shuffle the data down
    memmove(&data->keys[index_to_erase], &data->keys[start_index], size_to_end * sizeof(*data->keys));
    memmove(&data->field_transform[index_to_erase * 1], &data->field_transform[start_index * 1], size_to_end * sizeof(*data->field_transform) * 1);
    memmove(&data->field_aabb[index_to_erase * 1], &data->field_aabb[start_index * 1], size_to_end * sizeof(*data->field_aabb) * 1);

    return true;
  }

  return false;
}


bool
transform_exists(const Transform_data *data, const uint32_t key, size_t *out_index)
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
transform_clear(Transform_data *data)
{
  assert(data);

  data->size = 0;
}


bool
transform_is_empty(const Transform_data *data, const size_t size_hint)
{
  assert(data);

  if(!data) { return false; }

  return !!data->size;
}


size_t
transform_get_size(const Transform_data *data)
{
  assert(data);

  if(!data) { return 0; }

  return data->size;
}


size_t
transform_get_capacity(const Transform_data *data)
{
  assert(data);

  if(!data) { return 0; }

  return data->capacity;
}


bool
transform_resize_capacity(Transform_data *data, const size_t size_hint)
{
  assert(data);
  assert(size_hint > data->size); // Will slice data

  // Create new data.
  Transform_data new_data;
  const bool created_new = transform_create(&new_data, size_hint);

  // Failed to resize.
  if(!created_new)
  {
    transform_destroy(&new_data);
    return false;
  }

  // Copy over data
  {
    memcpy(new_data.keys, data->keys, sizeof(uint32_t) * data->size);
    memcpy(new_data.field_transform, data->field_transform, sizeof(math::transform) * data->size * 1);
    memcpy(new_data.field_aabb, data->field_aabb, sizeof(math::aabb) * data->size * 1);
  }

  // Swap ptrs
  {
    uint32_t *old_keys = data->keys;
    data->keys = new_data.keys;
    new_data.keys = old_keys;

    math::transform *old_transform = data->field_transform;
    data->field_transform = new_data.field_transform;
    new_data.field_transform = old_transform;

    math::aabb *old_aabb = data->field_aabb;
    data->field_aabb = new_data.field_aabb;
    new_data.field_aabb = old_aabb;
  }

  // Set the Capacity
  {
    size_t *capacity = const_cast<size_t*>(&data->capacity);
    *capacity = new_data.capacity;
  }

  // Destroy new data
  transform_destroy(&new_data);

  return true;
}



// ===================== //
// DATA GETTER FUNCTIONS //
// ===================== //


const math::transform*
transform_get_const_transform_data(const Transform_data *data)
{
  assert(data);
  assert(data->field_transform);

  return data->field_transform;
}


math::transform*
transform_get_transform_data(Transform_data *data)
{
  assert(data);
  assert(data->field_transform);

  return data->field_transform;
}


const math::aabb*
transform_get_const_aabb_data(const Transform_data *data)
{
  assert(data);
  assert(data->field_aabb);

  return data->field_aabb;
}


math::aabb*
transform_get_aabb_data(Transform_data *data)
{
  assert(data);
  assert(data->field_aabb);

  return data->field_aabb;
}


// =============== //
// FIELD FUNCTIONS //
// =============== //


bool
transform_get_transform(const Transform_data *data, const uint32_t key, math::transform *return_value)
{
  assert(data);
  assert(key != 0);
  assert(data->field_transform);
  assert(return_value);

  // Search for its index.
  // If we find it we can return the value.

  size_t index = 0;

  if(transform_exists(data, key, &index))
  {
    assert(index < data->size);

    if(index < data->size)
    {
      *return_value = data->field_transform[index];

      return true;
    }
  }

  return false;
}


bool
transform_set_transform(const Transform_data *data, const uint32_t key, const math::transform *set_value)
{
  assert(data);
  assert(key != 0);
  assert(data->field_transform);
  assert(set_value);

  // Search for its index.
  // If we find it we can set the value.

  size_t index = 0;

  if(transform_exists(data, key, &index))
  {
    assert(index < data->size);
    if(index < data->size)
    {
      data->field_transform[index] = *set_value;

      return true;
    }
  }

  return false;
}


bool
transform_get_aabb(const Transform_data *data, const uint32_t key, math::aabb *return_value)
{
  assert(data);
  assert(key != 0);
  assert(data->field_aabb);
  assert(return_value);

  // Search for its index.
  // If we find it we can return the value.

  size_t index = 0;

  if(transform_exists(data, key, &index))
  {
    assert(index < data->size);

    if(index < data->size)
    {
      *return_value = data->field_aabb[index];

      return true;
    }
  }

  return false;
}


bool
transform_set_aabb(const Transform_data *data, const uint32_t key, const math::aabb *set_value)
{
  assert(data);
  assert(key != 0);
  assert(data->field_aabb);
  assert(set_value);

  // Search for its index.
  // If we find it we can set the value.

  size_t index = 0;

  if(transform_exists(data, key, &index))
  {
    assert(index < data->size);
    if(index < data->size)
    {
      data->field_aabb[index] = *set_value;

      return true;
    }
  }

  return false;
}


} // Data ns
