
/*
  Warning
  --
  This file is auto generated changes here may be overwritten.
*/


#include "light_data.hpp"
#include <assert.h>
#include <cstring>


namespace Data {


// ====================== //
// COMMON MODEL FUNCTIONS //
// ====================== //



void
data_lock(const Light_data *data)
{
  // Not yet impl.
}


void
data_unlock(const Light_data *data)
{
  // Not yet impl.
}


bool
light_create(Light_data *data, const size_t size_hint)
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

    // Alloc space for light
    if(all_alloc)
    {
      data->field_light = new Lighting::Light[size_hint * 1];
      assert(data->field_light);
      if(!data->field_light) { all_alloc = false; }
      else { memset(data->field_light, 0, sizeof(Lighting::Light) * size_hint * 1); }
    }

    // Alloc space for bounding_box
    if(all_alloc)
    {
      data->field_bounding_box = new math::aabb[size_hint * 1];
      assert(data->field_bounding_box);
      if(!data->field_bounding_box) { all_alloc = false; }
      else { memset(data->field_bounding_box, 0, sizeof(math::aabb) * size_hint * 1); }
    }
  }

  // Failed so clean up.
  if(!all_alloc)
  {
    light_destroy(data);
  }

  return all_alloc;
}


void
light_destroy(Light_data *data)
{
  // Free up the memory.
  {
    // Remove keys
    if(data->keys) { delete[] data->keys; }
    data->keys = nullptr;

    // Remove light
    if(data->field_light) { delete[] data->field_light; }
    data->field_light = nullptr;

    // Remove bounding_box
    if(data->field_bounding_box) { delete[] data->field_bounding_box; }
    data->field_bounding_box = nullptr;
  }

  // Zero capacity and size
  {
    data->size = 0;

    size_t *capacity = const_cast<size_t*>(&data->capacity);
    *capacity = 0;
  }
}


uint32_t
light_push(Light_data *data, const uint32_t key)
{
  assert(data);
  assert(data->keys);

  // Do we need to resize?
  if(data->size >= data->capacity)
  {
    light_resize_capacity(data, data->capacity << 1);
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
light_insert(Light_data *data, const uint32_t key, const size_t insert_point)
{
  assert(data);
  assert(data->keys);
  assert(key);

  // Do we need to resize?
  if(data->size >= data->capacity)
  {
    light_resize_capacity(data, data->capacity << 1);
  }

  // Shuffle memory up
  {
    const size_t insert_index = insert_point + 1;
    const size_t size_to_end = data->size - insert_point;

    // Shuffle the data down
    memmove(&data->keys[insert_index], &data->keys[insert_point], size_to_end * sizeof(*data->keys));
    memmove(&data->field_light[insert_index * 1], &data->field_light[insert_point * 1], size_to_end * sizeof(*data->field_light) * 1);
    memmove(&data->field_bounding_box[insert_index * 1], &data->field_bounding_box[insert_point * 1], size_to_end * sizeof(*data->field_bounding_box) * 1);

    data->keys[insert_point] = key;
  }

  // Insert new data
  {
    ++data->size;
  }

  return true;
}


bool
light_remove(Light_data *data, const uint32_t key)
{
  size_t index_to_erase = 0;

  if(light_exists(data, key, &index_to_erase))
  {
    assert(index_to_erase < data->size);

    const size_t start_index = index_to_erase + 1;
    const size_t size_to_end = data->size - index_to_erase - 1;

    --(data->size);

    // Shuffle the data down
    memmove(&data->keys[index_to_erase], &data->keys[start_index], size_to_end * sizeof(*data->keys));
    memmove(&data->field_light[index_to_erase * 1], &data->field_light[start_index * 1], size_to_end * sizeof(*data->field_light) * 1);
    memmove(&data->field_bounding_box[index_to_erase * 1], &data->field_bounding_box[start_index * 1], size_to_end * sizeof(*data->field_bounding_box) * 1);

    return true;
  }

  return false;
}


bool
light_exists(const Light_data *data, const uint32_t key, size_t *out_index)
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
light_clear(Light_data *data)
{
  assert(data);

  data->size = 0;
}


bool
light_is_empty(const Light_data *data, const size_t size_hint)
{
  assert(data);

  if(!data) { return false; }

  return !!data->size;
}


size_t
light_get_size(const Light_data *data)
{
  assert(data);

  if(!data) { return 0; }

  return data->size;
}


size_t
light_get_capacity(const Light_data *data)
{
  assert(data);

  if(!data) { return 0; }

  return data->capacity;
}


bool
light_resize_capacity(Light_data *data, const size_t size_hint)
{
  assert(data);
  assert(size_hint > data->size); // Will slice data

  // Create new data.
  Light_data new_data;
  const bool created_new = light_create(&new_data, size_hint);

  // Failed to resize.
  if(!created_new)
  {
    light_destroy(&new_data);
    return false;
  }

  // Copy over data
  {
    memcpy(new_data.keys, data->keys, sizeof(uint32_t) * data->size);
    memcpy(new_data.field_light, data->field_light, sizeof(Lighting::Light) * data->size * 1);
    memcpy(new_data.field_bounding_box, data->field_bounding_box, sizeof(math::aabb) * data->size * 1);
  }

  // Swap ptrs
  {
    uint32_t *old_keys = data->keys;
    data->keys = new_data.keys;
    new_data.keys = old_keys;

    Lighting::Light *old_light = data->field_light;
    data->field_light = new_data.field_light;
    new_data.field_light = old_light;

    math::aabb *old_bounding_box = data->field_bounding_box;
    data->field_bounding_box = new_data.field_bounding_box;
    new_data.field_bounding_box = old_bounding_box;
  }

  // Set the Capacity
  {
    size_t *capacity = const_cast<size_t*>(&data->capacity);
    *capacity = new_data.capacity;
  }

  // Destroy new data
  light_destroy(&new_data);

  return true;
}



// ===================== //
// DATA GETTER FUNCTIONS //
// ===================== //


const Lighting::Light*
light_get_const_light_data(const Light_data *data)
{
  assert(data);
  assert(data->field_light);

  return data->field_light;
}


Lighting::Light*
light_get_light_data(Light_data *data)
{
  assert(data);
  assert(data->field_light);

  return data->field_light;
}


const math::aabb*
light_get_const_bounding_box_data(const Light_data *data)
{
  assert(data);
  assert(data->field_bounding_box);

  return data->field_bounding_box;
}


math::aabb*
light_get_bounding_box_data(Light_data *data)
{
  assert(data);
  assert(data->field_bounding_box);

  return data->field_bounding_box;
}


// =============== //
// FIELD FUNCTIONS //
// =============== //


bool
light_get_light(const Light_data *data, const uint32_t key, Lighting::Light *return_value)
{
  assert(data);
  assert(key != 0);
  assert(data->field_light);
  assert(return_value);

  // Search for its index.
  // If we find it we can return the value.

  size_t index = 0;

  if(light_exists(data, key, &index))
  {
    assert(index < data->size);

    if(index < data->size)
    {
      *return_value = data->field_light[index];

      return true;
    }
  }

  return false;
}


bool
light_set_light(const Light_data *data, const uint32_t key, const Lighting::Light *set_value)
{
  assert(data);
  assert(key != 0);
  assert(data->field_light);
  assert(set_value);

  // Search for its index.
  // If we find it we can set the value.

  size_t index = 0;

  if(light_exists(data, key, &index))
  {
    assert(index < data->size);
    if(index < data->size)
    {
      data->field_light[index] = *set_value;

      return true;
    }
  }

  return false;
}


bool
light_get_bounding_box(const Light_data *data, const uint32_t key, math::aabb *return_value)
{
  assert(data);
  assert(key != 0);
  assert(data->field_bounding_box);
  assert(return_value);

  // Search for its index.
  // If we find it we can return the value.

  size_t index = 0;

  if(light_exists(data, key, &index))
  {
    assert(index < data->size);

    if(index < data->size)
    {
      *return_value = data->field_bounding_box[index];

      return true;
    }
  }

  return false;
}


bool
light_set_bounding_box(const Light_data *data, const uint32_t key, const math::aabb *set_value)
{
  assert(data);
  assert(key != 0);
  assert(data->field_bounding_box);
  assert(set_value);

  // Search for its index.
  // If we find it we can set the value.

  size_t index = 0;

  if(light_exists(data, key, &index))
  {
    assert(index < data->size);
    if(index < data->size)
    {
      data->field_bounding_box[index] = *set_value;

      return true;
    }
  }

  return false;
}


} // Data ns
