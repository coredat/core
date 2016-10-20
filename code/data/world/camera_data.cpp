
/*
  Warning
  --
  This file is auto generated changes here may be overwritten.
*/


#include "camera_data.hpp"
#include <assert.h>
#include <cstring>


namespace Data {


// ====================== //
// COMMON MODEL FUNCTIONS //
// ====================== //



void
data_lock(const Camera_data *data)
{
  // Not yet impl.
}


void
data_unlock(const Camera_data *data)
{
  // Not yet impl.
}


bool
camera_create(Camera_data *data, const size_t size_hint)
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

    // Alloc space for priority
    if(all_alloc)
    {
      data->field_priority = new uint32_t[size_hint * 1];
      assert(data->field_priority);
      if(!data->field_priority) { all_alloc = false; }
      else { memset(data->field_priority, 0, sizeof(uint32_t) * size_hint * 1); }
    }

    // Alloc space for properties
    if(all_alloc)
    {
      data->field_properties = new Camera::Camera_properties[size_hint * 1];
      assert(data->field_properties);
      if(!data->field_properties) { all_alloc = false; }
      else { memset(data->field_properties, 0, sizeof(Camera::Camera_properties) * size_hint * 1); }
    }

    // Alloc space for texture_id
    if(all_alloc)
    {
      data->field_texture_id = new util::generic_id[size_hint * 1];
      assert(data->field_texture_id);
      if(!data->field_texture_id) { all_alloc = false; }
      else { memset(data->field_texture_id, 0, sizeof(util::generic_id) * size_hint * 1); }
    }

    // Alloc space for post_process_id
    if(all_alloc)
    {
      data->field_post_process_id = new util::generic_id[size_hint * 1];
      assert(data->field_post_process_id);
      if(!data->field_post_process_id) { all_alloc = false; }
      else { memset(data->field_post_process_id, 0, sizeof(util::generic_id) * size_hint * 1); }
    }
  }

  // Failed so clean up.
  if(!all_alloc)
  {
    camera_destroy(data);
  }

  return all_alloc;
}


void
camera_destroy(Camera_data *data)
{
  // Free up the memory.
  {
    // Remove keys
    if(data->keys) { delete[] data->keys; }
    data->keys = nullptr;

    // Remove priority
    if(data->field_priority) { delete[] data->field_priority; }
    data->field_priority = nullptr;

    // Remove properties
    if(data->field_properties) { delete[] data->field_properties; }
    data->field_properties = nullptr;

    // Remove texture_id
    if(data->field_texture_id) { delete[] data->field_texture_id; }
    data->field_texture_id = nullptr;

    // Remove post_process_id
    if(data->field_post_process_id) { delete[] data->field_post_process_id; }
    data->field_post_process_id = nullptr;
  }

  // Zero capacity and size
  {
    data->size = 0;

    size_t *capacity = const_cast<size_t*>(&data->capacity);
    *capacity = 0;
  }
}


uint32_t
camera_push(Camera_data *data, const uint32_t key)
{
  assert(data);
  assert(data->keys);

  // Do we need to resize?
  if(data->size >= data->capacity)
  {
    camera_resize_capacity(data, data->capacity << 1);
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
camera_insert(Camera_data *data, const uint32_t key, const size_t insert_point)
{
  assert(data);
  assert(data->keys);
  assert(key);

  // Do we need to resize?
  if(data->size >= data->capacity)
  {
    camera_resize_capacity(data, data->capacity << 1);
  }

  // Shuffle memory up
  {
    const size_t insert_index = insert_point + 1;
    const size_t start_index = insert_point;
    const size_t size_to_end = data->size - insert_point;

    // Shuffle the data down
    memmove(&data->keys[insert_point], &data->keys[start_index], size_to_end * sizeof(*data->keys));
    memmove(&data->field_priority[insert_point * 1], &data->field_priority[start_index * 1], size_to_end * sizeof(*data->field_priority) * 1);
    memmove(&data->field_properties[insert_point * 1], &data->field_properties[start_index * 1], size_to_end * sizeof(*data->field_properties) * 1);
    memmove(&data->field_texture_id[insert_point * 1], &data->field_texture_id[start_index * 1], size_to_end * sizeof(*data->field_texture_id) * 1);
    memmove(&data->field_post_process_id[insert_point * 1], &data->field_post_process_id[start_index * 1], size_to_end * sizeof(*data->field_post_process_id) * 1);
  }

  // Insert new data
  {
    data->keys[data->size++] = key;
  }

  return false;
}


bool
camera_remove(Camera_data *data, const uint32_t key)
{
  size_t index_to_erase = 0;

  if(camera_exists(data, key, &index_to_erase))
  {
    assert(index_to_erase < data->size);

    const size_t start_index = index_to_erase + 1;
    const size_t size_to_end = data->size - index_to_erase - 1;

    --(data->size);

    // Shuffle the data down
    memmove(&data->keys[index_to_erase], &data->keys[start_index], size_to_end * sizeof(*data->keys));
    memmove(&data->field_priority[index_to_erase * 1], &data->field_priority[start_index * 1], size_to_end * sizeof(*data->field_priority) * 1);
    memmove(&data->field_properties[index_to_erase * 1], &data->field_properties[start_index * 1], size_to_end * sizeof(*data->field_properties) * 1);
    memmove(&data->field_texture_id[index_to_erase * 1], &data->field_texture_id[start_index * 1], size_to_end * sizeof(*data->field_texture_id) * 1);
    memmove(&data->field_post_process_id[index_to_erase * 1], &data->field_post_process_id[start_index * 1], size_to_end * sizeof(*data->field_post_process_id) * 1);

    return true;
  }

  return false;
}


bool
camera_exists(const Camera_data *data, const uint32_t key, size_t *out_index)
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
camera_clear(Camera_data *data)
{
  assert(data);

  data->size = 0;
}


bool
camera_is_empty(const Camera_data *data, const size_t size_hint)
{
  assert(data);

  if(!data) { return false; }

  return !!data->size;
}


size_t
camera_get_size(const Camera_data *data)
{
  assert(data);

  if(!data) { return 0; }

  return data->size;
}


size_t
camera_get_capacity(const Camera_data *data)
{
  assert(data);

  if(!data) { return 0; }

  return data->capacity;
}


bool
camera_resize_capacity(Camera_data *data, const size_t size_hint)
{
  assert(data);
  assert(size_hint > data->size); // Will slice data

  // Create new data.
  Camera_data new_data;
  const bool created_new = camera_create(&new_data, size_hint);

  // Failed to resize.
  if(!created_new)
  {
    camera_destroy(&new_data);
    return false;
  }

  // Copy over data
  {
    memcpy(new_data.keys, data->keys, sizeof(uint32_t) * data->size);
    memcpy(new_data.field_priority, data->field_priority, sizeof(uint32_t) * data->size * 1);
    memcpy(new_data.field_properties, data->field_properties, sizeof(Camera::Camera_properties) * data->size * 1);
    memcpy(new_data.field_texture_id, data->field_texture_id, sizeof(util::generic_id) * data->size * 1);
    memcpy(new_data.field_post_process_id, data->field_post_process_id, sizeof(util::generic_id) * data->size * 1);
  }

  // Swap ptrs
  {
    uint32_t *old_keys = data->keys;
    data->keys = new_data.keys;
    new_data.keys = old_keys;

    uint32_t *old_priority = data->field_priority;
    data->field_priority = new_data.field_priority;
    new_data.field_priority = old_priority;

    Camera::Camera_properties *old_properties = data->field_properties;
    data->field_properties = new_data.field_properties;
    new_data.field_properties = old_properties;

    util::generic_id *old_texture_id = data->field_texture_id;
    data->field_texture_id = new_data.field_texture_id;
    new_data.field_texture_id = old_texture_id;

    util::generic_id *old_post_process_id = data->field_post_process_id;
    data->field_post_process_id = new_data.field_post_process_id;
    new_data.field_post_process_id = old_post_process_id;
  }

  // Set the Capacity
  {
    size_t *capacity = const_cast<size_t*>(&data->capacity);
    *capacity = new_data.capacity;
  }

  // Destroy new data
  camera_destroy(&new_data);

  return true;
}



// ===================== //
// DATA GETTER FUNCTIONS //
// ===================== //


const uint32_t*
camera_get_const_priority_data(const Camera_data *data)
{
  assert(data);
  assert(data->field_priority);

  return data->field_priority;
}


uint32_t*
camera_get_priority_data(Camera_data *data)
{
  assert(data);
  assert(data->field_priority);

  return data->field_priority;
}


const Camera::Camera_properties*
camera_get_const_properties_data(const Camera_data *data)
{
  assert(data);
  assert(data->field_properties);

  return data->field_properties;
}


Camera::Camera_properties*
camera_get_properties_data(Camera_data *data)
{
  assert(data);
  assert(data->field_properties);

  return data->field_properties;
}


const util::generic_id*
camera_get_const_texture_id_data(const Camera_data *data)
{
  assert(data);
  assert(data->field_texture_id);

  return data->field_texture_id;
}


util::generic_id*
camera_get_texture_id_data(Camera_data *data)
{
  assert(data);
  assert(data->field_texture_id);

  return data->field_texture_id;
}


const util::generic_id*
camera_get_const_post_process_id_data(const Camera_data *data)
{
  assert(data);
  assert(data->field_post_process_id);

  return data->field_post_process_id;
}


util::generic_id*
camera_get_post_process_id_data(Camera_data *data)
{
  assert(data);
  assert(data->field_post_process_id);

  return data->field_post_process_id;
}


// =============== //
// FIELD FUNCTIONS //
// =============== //


bool
camera_get_priority(const Camera_data *data, const uint32_t key, uint32_t *return_value)
{
  assert(data);
  assert(key != 0);
  assert(data->field_priority);
  assert(return_value);

  // Search for its index.
  // If we find it we can return the value.

  size_t index = 0;

  if(camera_exists(data, key, &index))
  {
    assert(index < data->size);

    if(index < data->size)
    {
      *return_value = data->field_priority[index];

      return true;
    }
  }

  return false;
}


bool
camera_set_priority(const Camera_data *data, const uint32_t key, const uint32_t *set_value)
{
  assert(data);
  assert(key != 0);
  assert(data->field_priority);
  assert(set_value);

  // Search for its index.
  // If we find it we can set the value.

  size_t index = 0;

  if(camera_exists(data, key, &index))
  {
    assert(index < data->size);
    if(index < data->size)
    {
      data->field_priority[index] = *set_value;

      return true;
    }
  }

  return false;
}


bool
camera_get_properties(const Camera_data *data, const uint32_t key, Camera::Camera_properties *return_value)
{
  assert(data);
  assert(key != 0);
  assert(data->field_properties);
  assert(return_value);

  // Search for its index.
  // If we find it we can return the value.

  size_t index = 0;

  if(camera_exists(data, key, &index))
  {
    assert(index < data->size);

    if(index < data->size)
    {
      *return_value = data->field_properties[index];

      return true;
    }
  }

  return false;
}


bool
camera_set_properties(const Camera_data *data, const uint32_t key, const Camera::Camera_properties *set_value)
{
  assert(data);
  assert(key != 0);
  assert(data->field_properties);
  assert(set_value);

  // Search for its index.
  // If we find it we can set the value.

  size_t index = 0;

  if(camera_exists(data, key, &index))
  {
    assert(index < data->size);
    if(index < data->size)
    {
      data->field_properties[index] = *set_value;

      return true;
    }
  }

  return false;
}


bool
camera_get_texture_id(const Camera_data *data, const uint32_t key, util::generic_id *return_value)
{
  assert(data);
  assert(key != 0);
  assert(data->field_texture_id);
  assert(return_value);

  // Search for its index.
  // If we find it we can return the value.

  size_t index = 0;

  if(camera_exists(data, key, &index))
  {
    assert(index < data->size);

    if(index < data->size)
    {
      *return_value = data->field_texture_id[index];

      return true;
    }
  }

  return false;
}


bool
camera_set_texture_id(const Camera_data *data, const uint32_t key, const util::generic_id *set_value)
{
  assert(data);
  assert(key != 0);
  assert(data->field_texture_id);
  assert(set_value);

  // Search for its index.
  // If we find it we can set the value.

  size_t index = 0;

  if(camera_exists(data, key, &index))
  {
    assert(index < data->size);
    if(index < data->size)
    {
      data->field_texture_id[index] = *set_value;

      return true;
    }
  }

  return false;
}


bool
camera_get_post_process_id(const Camera_data *data, const uint32_t key, util::generic_id *return_value)
{
  assert(data);
  assert(key != 0);
  assert(data->field_post_process_id);
  assert(return_value);

  // Search for its index.
  // If we find it we can return the value.

  size_t index = 0;

  if(camera_exists(data, key, &index))
  {
    assert(index < data->size);

    if(index < data->size)
    {
      *return_value = data->field_post_process_id[index];

      return true;
    }
  }

  return false;
}


bool
camera_set_post_process_id(const Camera_data *data, const uint32_t key, const util::generic_id *set_value)
{
  assert(data);
  assert(key != 0);
  assert(data->field_post_process_id);
  assert(set_value);

  // Search for its index.
  // If we find it we can set the value.

  size_t index = 0;

  if(camera_exists(data, key, &index))
  {
    assert(index < data->size);
    if(index < data->size)
    {
      data->field_post_process_id[index] = *set_value;

      return true;
    }
  }

  return false;
}


} // Data ns
