
/*
  Warning
  --
  This file is auto generated changes here may be overwritten.
*/


#include "physics_data.hpp"
#include <assert.h>
#include <cstring>


namespace Data {


// ====================== //
// COMMON MODEL FUNCTIONS //
// ====================== //



void
data_lock(const Rigidbody_data *data)
{
  // Not yet impl.
}


void
data_unlock(const Rigidbody_data *data)
{
  // Not yet impl.
}


bool
rigidbody_create(Rigidbody_data *data, const size_t size_hint)
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

    // Alloc space for aabb_collider
    if(all_alloc)
    {
      data->field_aabb_collider = new math::aabb[size_hint * 1];
      assert(data->field_aabb_collider);
      if(!data->field_aabb_collider) { all_alloc = false; }
      else { memset(data->field_aabb_collider, 0, sizeof(math::aabb) * size_hint * 1); }
    }

    // Alloc space for transformed_aabb_collider
    if(all_alloc)
    {
      data->field_transformed_aabb_collider = new math::aabb[size_hint * 1];
      assert(data->field_transformed_aabb_collider);
      if(!data->field_transformed_aabb_collider) { all_alloc = false; }
      else { memset(data->field_transformed_aabb_collider, 0, sizeof(math::aabb) * size_hint * 1); }
    }

    // Alloc space for collision_id
    if(all_alloc)
    {
      data->field_collision_id = new uint64_t[size_hint * 1];
      assert(data->field_collision_id);
      if(!data->field_collision_id) { all_alloc = false; }
      else { memset(data->field_collision_id, 0, sizeof(uint64_t) * size_hint * 1); }
    }

    // Alloc space for rigidbody
    if(all_alloc)
    {
      data->field_rigidbody = new uintptr_t[size_hint * 1];
      assert(data->field_rigidbody);
      if(!data->field_rigidbody) { all_alloc = false; }
      else { memset(data->field_rigidbody, 0, sizeof(uintptr_t) * size_hint * 1); }
    }
  }

  // Failed so clean up.
  if(!all_alloc)
  {
    rigidbody_destroy(data);
  }

  return all_alloc;
}


void
rigidbody_destroy(Rigidbody_data *data)
{
  // Free up the memory.
  {
    // Remove keys
    if(data->keys) { delete[] data->keys; }
    data->keys = nullptr;

    // Remove transform
    if(data->field_transform) { delete[] data->field_transform; }
    data->field_transform = nullptr;

    // Remove aabb_collider
    if(data->field_aabb_collider) { delete[] data->field_aabb_collider; }
    data->field_aabb_collider = nullptr;

    // Remove transformed_aabb_collider
    if(data->field_transformed_aabb_collider) { delete[] data->field_transformed_aabb_collider; }
    data->field_transformed_aabb_collider = nullptr;

    // Remove collision_id
    if(data->field_collision_id) { delete[] data->field_collision_id; }
    data->field_collision_id = nullptr;

    // Remove rigidbody
    if(data->field_rigidbody) { delete[] data->field_rigidbody; }
    data->field_rigidbody = nullptr;
  }

  // Zero capacity and size
  {
    data->size = 0;

    size_t *capacity = const_cast<size_t*>(&data->capacity);
    *capacity = 0;
  }
}


uint32_t
rigidbody_insert(Rigidbody_data *data, const uint32_t key)
{
  assert(data);
  assert(data->keys);

  // Do we need to resize?
  if(data->size >= data->capacity)
  {
    rigidbody_resize_capacity(data, data->capacity << 1);
  }

  // Insert key at the back
  {
    const uint32_t new_key = key;
    data->keys[data->size++] = new_key;

    return new_key;
  }


  return 0;
}


bool
rigidbody_remove(Rigidbody_data *data, const uint32_t key)
{
  size_t index_to_erase = 0;

  if(rigidbody_exists(data, key, &index_to_erase))
  {
    assert(index_to_erase < data->size);

    const size_t start_index = index_to_erase + 1;
    const size_t size_to_end = data->size - index_to_erase - 1;

    --(data->size);

    // Shuffle the data down
    memmove(&data->keys[index_to_erase], &data->keys[start_index], size_to_end * sizeof(*data->keys));
    memmove(&data->field_transform[index_to_erase * 1], &data->field_transform[start_index * 1], size_to_end * sizeof(*data->field_transform) * 1);
    memmove(&data->field_aabb_collider[index_to_erase * 1], &data->field_aabb_collider[start_index * 1], size_to_end * sizeof(*data->field_aabb_collider) * 1);
    memmove(&data->field_transformed_aabb_collider[index_to_erase * 1], &data->field_transformed_aabb_collider[start_index * 1], size_to_end * sizeof(*data->field_transformed_aabb_collider) * 1);
    memmove(&data->field_collision_id[index_to_erase * 1], &data->field_collision_id[start_index * 1], size_to_end * sizeof(*data->field_collision_id) * 1);
    memmove(&data->field_rigidbody[index_to_erase * 1], &data->field_rigidbody[start_index * 1], size_to_end * sizeof(*data->field_rigidbody) * 1);

    return true;
  }

  return false;
}


bool
rigidbody_exists(const Rigidbody_data *data, const uint32_t key, size_t *out_index)
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
rigidbody_clear(Rigidbody_data *data)
{
  assert(data);

  data->size = 0;
}


bool
rigidbody_is_empty(const Rigidbody_data *data, const size_t size_hint)
{
  assert(data);

  if(!data) { return false; }

  return !!data->size;
}


size_t
rigidbody_get_size(const Rigidbody_data *data)
{
  assert(data);

  if(!data) { return 0; }

  return data->size;
}


size_t
rigidbody_get_capacity(const Rigidbody_data *data)
{
  assert(data);

  if(!data) { return 0; }

  return data->capacity;
}


bool
rigidbody_resize_capacity(Rigidbody_data *data, const size_t size_hint)
{
  assert(data);
  assert(size_hint > data->size); // Will slice data

  // Create new data.
  Rigidbody_data new_data;
  const bool created_new = rigidbody_create(&new_data, size_hint);

  // Failed to resize.
  if(!created_new)
  {
    rigidbody_destroy(&new_data);
    return false;
  }

  // Copy over data
  {
    memcpy(new_data.keys, data->keys, sizeof(uint32_t) * data->size);
    memcpy(new_data.field_transform, data->field_transform, sizeof(math::transform) * data->size * 1);
    memcpy(new_data.field_aabb_collider, data->field_aabb_collider, sizeof(math::aabb) * data->size * 1);
    memcpy(new_data.field_transformed_aabb_collider, data->field_transformed_aabb_collider, sizeof(math::aabb) * data->size * 1);
    memcpy(new_data.field_collision_id, data->field_collision_id, sizeof(uint64_t) * data->size * 1);
    memcpy(new_data.field_rigidbody, data->field_rigidbody, sizeof(uintptr_t) * data->size * 1);
  }

  // Swap ptrs
  {
    uint32_t *old_keys = data->keys;
    data->keys = new_data.keys;
    new_data.keys = old_keys;

    math::transform *old_transform = data->field_transform;
    data->field_transform = new_data.field_transform;
    new_data.field_transform = old_transform;

    math::aabb *old_aabb_collider = data->field_aabb_collider;
    data->field_aabb_collider = new_data.field_aabb_collider;
    new_data.field_aabb_collider = old_aabb_collider;

    math::aabb *old_transformed_aabb_collider = data->field_transformed_aabb_collider;
    data->field_transformed_aabb_collider = new_data.field_transformed_aabb_collider;
    new_data.field_transformed_aabb_collider = old_transformed_aabb_collider;

    uint64_t *old_collision_id = data->field_collision_id;
    data->field_collision_id = new_data.field_collision_id;
    new_data.field_collision_id = old_collision_id;

    uintptr_t *old_rigidbody = data->field_rigidbody;
    data->field_rigidbody = new_data.field_rigidbody;
    new_data.field_rigidbody = old_rigidbody;
  }

  // Set the Capacity
  {
    size_t *capacity = const_cast<size_t*>(&data->capacity);
    *capacity = new_data.capacity;
  }

  // Destroy new data
  rigidbody_destroy(&new_data);

  return true;
}



// ===================== //
// DATA GETTER FUNCTIONS //
// ===================== //


const math::transform*
rigidbody_get_const_transform_data(const Rigidbody_data *data)
{
  assert(data);
  assert(data->field_transform);

  return data->field_transform;
}


math::transform*
rigidbody_get_transform_data(Rigidbody_data *data)
{
  assert(data);
  assert(data->field_transform);

  return data->field_transform;
}


const math::aabb*
rigidbody_get_const_aabb_collider_data(const Rigidbody_data *data)
{
  assert(data);
  assert(data->field_aabb_collider);

  return data->field_aabb_collider;
}


math::aabb*
rigidbody_get_aabb_collider_data(Rigidbody_data *data)
{
  assert(data);
  assert(data->field_aabb_collider);

  return data->field_aabb_collider;
}


const math::aabb*
rigidbody_get_const_transformed_aabb_collider_data(const Rigidbody_data *data)
{
  assert(data);
  assert(data->field_transformed_aabb_collider);

  return data->field_transformed_aabb_collider;
}


math::aabb*
rigidbody_get_transformed_aabb_collider_data(Rigidbody_data *data)
{
  assert(data);
  assert(data->field_transformed_aabb_collider);

  return data->field_transformed_aabb_collider;
}


const uint64_t*
rigidbody_get_const_collision_id_data(const Rigidbody_data *data)
{
  assert(data);
  assert(data->field_collision_id);

  return data->field_collision_id;
}


uint64_t*
rigidbody_get_collision_id_data(Rigidbody_data *data)
{
  assert(data);
  assert(data->field_collision_id);

  return data->field_collision_id;
}


const uintptr_t*
rigidbody_get_const_rigidbody_data(const Rigidbody_data *data)
{
  assert(data);
  assert(data->field_rigidbody);

  return data->field_rigidbody;
}


uintptr_t*
rigidbody_get_rigidbody_data(Rigidbody_data *data)
{
  assert(data);
  assert(data->field_rigidbody);

  return data->field_rigidbody;
}


// =============== //
// FIELD FUNCTIONS //
// =============== //


bool
rigidbody_get_transform(const Rigidbody_data *data, const uint32_t key, math::transform *return_value)
{
  assert(data);
  assert(key != 0);
  assert(data->field_transform);
  assert(return_value);

  // Search for its index.
  // If we find it we can return the value.

  size_t index = 0;

  if(rigidbody_exists(data, key, &index))
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
rigidbody_set_transform(const Rigidbody_data *data, const uint32_t key, const math::transform *set_value)
{
  assert(data);
  assert(key != 0);
  assert(data->field_transform);
  assert(set_value);

  // Search for its index.
  // If we find it we can set the value.

  size_t index = 0;

  if(rigidbody_exists(data, key, &index))

  index = index * 1;

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
rigidbody_get_aabb_collider(const Rigidbody_data *data, const uint32_t key, math::aabb *return_value)
{
  assert(data);
  assert(key != 0);
  assert(data->field_aabb_collider);
  assert(return_value);

  // Search for its index.
  // If we find it we can return the value.

  size_t index = 0;

  if(rigidbody_exists(data, key, &index))
  {
    assert(index < data->size);

    if(index < data->size)
    {
      *return_value = data->field_aabb_collider[index];

      return true;
    }
  }

  return false;
}


bool
rigidbody_set_aabb_collider(const Rigidbody_data *data, const uint32_t key, const math::aabb *set_value)
{
  assert(data);
  assert(key != 0);
  assert(data->field_aabb_collider);
  assert(set_value);

  // Search for its index.
  // If we find it we can set the value.

  size_t index = 0;

  if(rigidbody_exists(data, key, &index))

  index = index * 1;

  {
    assert(index < data->size);
    if(index < data->size)
    {
      data->field_aabb_collider[index] = *set_value;

      return true;
    }
  }

  return false;
}


bool
rigidbody_get_transformed_aabb_collider(const Rigidbody_data *data, const uint32_t key, math::aabb *return_value)
{
  assert(data);
  assert(key != 0);
  assert(data->field_transformed_aabb_collider);
  assert(return_value);

  // Search for its index.
  // If we find it we can return the value.

  size_t index = 0;

  if(rigidbody_exists(data, key, &index))
  {
    assert(index < data->size);

    if(index < data->size)
    {
      *return_value = data->field_transformed_aabb_collider[index];

      return true;
    }
  }

  return false;
}


bool
rigidbody_set_transformed_aabb_collider(const Rigidbody_data *data, const uint32_t key, const math::aabb *set_value)
{
  assert(data);
  assert(key != 0);
  assert(data->field_transformed_aabb_collider);
  assert(set_value);

  // Search for its index.
  // If we find it we can set the value.

  size_t index = 0;

  if(rigidbody_exists(data, key, &index))

  index = index * 1;

  {
    assert(index < data->size);
    if(index < data->size)
    {
      data->field_transformed_aabb_collider[index] = *set_value;

      return true;
    }
  }

  return false;
}


bool
rigidbody_get_collision_id(const Rigidbody_data *data, const uint32_t key, uint64_t *return_value)
{
  assert(data);
  assert(key != 0);
  assert(data->field_collision_id);
  assert(return_value);

  // Search for its index.
  // If we find it we can return the value.

  size_t index = 0;

  if(rigidbody_exists(data, key, &index))
  {
    assert(index < data->size);

    if(index < data->size)
    {
      *return_value = data->field_collision_id[index];

      return true;
    }
  }

  return false;
}


bool
rigidbody_set_collision_id(const Rigidbody_data *data, const uint32_t key, const uint64_t *set_value)
{
  assert(data);
  assert(key != 0);
  assert(data->field_collision_id);
  assert(set_value);

  // Search for its index.
  // If we find it we can set the value.

  size_t index = 0;

  if(rigidbody_exists(data, key, &index))

  index = index * 1;

  {
    assert(index < data->size);
    if(index < data->size)
    {
      data->field_collision_id[index] = *set_value;

      return true;
    }
  }

  return false;
}


bool
rigidbody_get_rigidbody(const Rigidbody_data *data, const uint32_t key, uintptr_t *return_value)
{
  assert(data);
  assert(key != 0);
  assert(data->field_rigidbody);
  assert(return_value);

  // Search for its index.
  // If we find it we can return the value.

  size_t index = 0;

  if(rigidbody_exists(data, key, &index))
  {
    assert(index < data->size);

    if(index < data->size)
    {
      *return_value = data->field_rigidbody[index];

      return true;
    }
  }

  return false;
}


bool
rigidbody_set_rigidbody(const Rigidbody_data *data, const uint32_t key, const uintptr_t *set_value)
{
  assert(data);
  assert(key != 0);
  assert(data->field_rigidbody);
  assert(set_value);

  // Search for its index.
  // If we find it we can set the value.

  size_t index = 0;

  if(rigidbody_exists(data, key, &index))

  index = index * 1;

  {
    assert(index < data->size);
    if(index < data->size)
    {
      data->field_rigidbody[index] = *set_value;

      return true;
    }
  }

  return false;
}


} // Data ns
