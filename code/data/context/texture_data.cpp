
/*
  Warning
  --
  This file is auto generated changes here may be overwritten.
*/


#include "texture_data.hpp"
#include <assert.h>
#include <cstring>


namespace Data {


// ====================== //
// COMMON MODEL FUNCTIONS //
// ====================== //

namespace {

// Key our keys unique.
uint32_t texture_model_key_instance_number{0};

} // anon ns


void
data_lock(const Texture_data *data)
{
  // Not yet impl.
}


void
data_unlock(const Texture_data *data)
{
  // Not yet impl.
}


bool
texture_create(Texture_data *data, const size_t size_hint)
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

    // Alloc space for name
    if(all_alloc)
    {
      data->field_name = new char[size_hint * 32];
      assert(data->field_name);
      if(!data->field_name) { all_alloc = false; }
      else { memset(data->field_name, 0, sizeof(char) * size_hint * 32); }
    }

    // Alloc space for texture
    if(all_alloc)
    {
      data->field_texture = new Ogl::Texture[size_hint * 1];
      assert(data->field_texture);
      if(!data->field_texture) { all_alloc = false; }
      else { memset(data->field_texture, 0, sizeof(Ogl::Texture) * size_hint * 1); }
    }

    // Alloc space for render_target
    if(all_alloc)
    {
      data->field_render_target = new Ogl::Frame_buffer[size_hint * 1];
      assert(data->field_render_target);
      if(!data->field_render_target) { all_alloc = false; }
      else { memset(data->field_render_target, 0, sizeof(Ogl::Frame_buffer) * size_hint * 1); }
    }
  }

  // Failed so clean up.
  if(!all_alloc)
  {
    texture_destroy(data);
  }

  return all_alloc;
}


void
texture_destroy(Texture_data *data)
{
  // Free up the memory.
  {
    // Remove keys
    if(data->keys) { delete[] data->keys; }
    data->keys = nullptr;

    // Remove name
    if(data->field_name) { delete[] data->field_name; }
    data->field_name = nullptr;

    // Remove texture
    if(data->field_texture) { delete[] data->field_texture; }
    data->field_texture = nullptr;

    // Remove render_target
    if(data->field_render_target) { delete[] data->field_render_target; }
    data->field_render_target = nullptr;
  }

  // Zero capacity and size
  {
    data->size = 0;

    size_t *capacity = const_cast<size_t*>(&data->capacity);
    *capacity = 0;
  }
}


uint32_t
texture_push(Texture_data *data)
{
  assert(data);
  assert(data->keys);

  // Do we need to resize?
  if(data->size >= data->capacity)
  {
    texture_resize_capacity(data, data->capacity << 1);
  }

  // Push key at the back
  {
    const uint32_t new_key = ++texture_model_key_instance_number;
    data->keys[data->size++] = new_key;

    return new_key;
  }


  return 0;
}



bool
texture_remove(Texture_data *data, const uint32_t key)
{
  size_t index_to_erase = 0;

  if(texture_exists(data, key, &index_to_erase))
  {
    assert(index_to_erase < data->size);

    const size_t start_index = index_to_erase + 1;
    const size_t size_to_end = data->size - index_to_erase - 1;

    --(data->size);

    // Shuffle the data down
    memmove(&data->keys[index_to_erase], &data->keys[start_index], size_to_end * sizeof(*data->keys));
    memmove(&data->field_name[index_to_erase * 32], &data->field_name[start_index * 32], size_to_end * sizeof(*data->field_name) * 32);
    memmove(&data->field_texture[index_to_erase * 1], &data->field_texture[start_index * 1], size_to_end * sizeof(*data->field_texture) * 1);
    memmove(&data->field_render_target[index_to_erase * 1], &data->field_render_target[start_index * 1], size_to_end * sizeof(*data->field_render_target) * 1);

    return true;
  }

  return false;
}


bool
texture_exists(const Texture_data *data, const uint32_t key, size_t *out_index)
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
texture_clear(Texture_data *data)
{
  assert(data);

  data->size = 0;
}


bool
texture_is_empty(const Texture_data *data, const size_t size_hint)
{
  assert(data);

  if(!data) { return false; }

  return !!data->size;
}


size_t
texture_get_size(const Texture_data *data)
{
  assert(data);

  if(!data) { return 0; }

  return data->size;
}


size_t
texture_get_capacity(const Texture_data *data)
{
  assert(data);

  if(!data) { return 0; }

  return data->capacity;
}


bool
texture_resize_capacity(Texture_data *data, const size_t size_hint)
{
  assert(data);
  assert(size_hint > data->size); // Will slice data

  // Create new data.
  Texture_data new_data;
  const bool created_new = texture_create(&new_data, size_hint);

  // Failed to resize.
  if(!created_new)
  {
    texture_destroy(&new_data);
    return false;
  }

  // Copy over data
  {
    memcpy(new_data.keys, data->keys, sizeof(uint32_t) * data->size);
    memcpy(new_data.field_name, data->field_name, sizeof(char) * data->size * 32);
    memcpy(new_data.field_texture, data->field_texture, sizeof(Ogl::Texture) * data->size * 1);
    memcpy(new_data.field_render_target, data->field_render_target, sizeof(Ogl::Frame_buffer) * data->size * 1);
  }

  // Swap ptrs
  {
    uint32_t *old_keys = data->keys;
    data->keys = new_data.keys;
    new_data.keys = old_keys;

    char *old_name = data->field_name;
    data->field_name = new_data.field_name;
    new_data.field_name = old_name;

    Ogl::Texture *old_texture = data->field_texture;
    data->field_texture = new_data.field_texture;
    new_data.field_texture = old_texture;

    Ogl::Frame_buffer *old_render_target = data->field_render_target;
    data->field_render_target = new_data.field_render_target;
    new_data.field_render_target = old_render_target;
  }

  // Set the Capacity
  {
    size_t *capacity = const_cast<size_t*>(&data->capacity);
    *capacity = new_data.capacity;
  }

  // Destroy new data
  texture_destroy(&new_data);

  return true;
}



// ===================== //
// DATA GETTER FUNCTIONS //
// ===================== //


const char*
texture_get_const_name_data(const Texture_data *data)
{
  assert(data);
  assert(data->field_name);

  return data->field_name;
}


char*
texture_get_name_data(Texture_data *data)
{
  assert(data);
  assert(data->field_name);

  return data->field_name;
}


const Ogl::Texture*
texture_get_const_texture_data(const Texture_data *data)
{
  assert(data);
  assert(data->field_texture);

  return data->field_texture;
}


Ogl::Texture*
texture_get_texture_data(Texture_data *data)
{
  assert(data);
  assert(data->field_texture);

  return data->field_texture;
}


const Ogl::Frame_buffer*
texture_get_const_render_target_data(const Texture_data *data)
{
  assert(data);
  assert(data->field_render_target);

  return data->field_render_target;
}


Ogl::Frame_buffer*
texture_get_render_target_data(Texture_data *data)
{
  assert(data);
  assert(data->field_render_target);

  return data->field_render_target;
}


// =============== //
// FIELD FUNCTIONS //
// =============== //


bool
texture_get_name(const Texture_data *data, const uint32_t key, const char **return_value)
{
  assert(data);
  assert(key != 0);
  assert(data->field_name);
  assert(return_value);

  // Search for its index.
  // If we find it we can return the value.

  size_t index = 0;

  if(texture_exists(data, key, &index))
  {
    assert(index < data->size);

    if(index < data->size)
    {
      *return_value = &data->field_name[index];

      return true;
    }
  }

  return false;
}


bool
texture_set_name(const Texture_data *data, const uint32_t key, const char *set_value, const size_t size)
{
  assert(data);
  assert(key != 0);
  assert(data->field_name);
  assert(set_value);

  // Search for its index.
  // If we find it we can set the value.

  size_t index = 0;

  if(texture_exists(data, key, &index))

  index = index * 32;

  {
    assert(index / 32 < data->size);
    if(index < data->size * 32)
    {
      memcpy(&data->field_name[index], set_value, sizeof(char) * size);

      return true;
    }
  }

  return false;
}


bool
texture_get_texture(const Texture_data *data, const uint32_t key, Ogl::Texture *return_value)
{
  assert(data);
  assert(key != 0);
  assert(data->field_texture);
  assert(return_value);

  // Search for its index.
  // If we find it we can return the value.

  size_t index = 0;

  if(texture_exists(data, key, &index))
  {
    assert(index < data->size);

    if(index < data->size)
    {
      *return_value = data->field_texture[index];

      return true;
    }
  }

  return false;
}


bool
texture_set_texture(const Texture_data *data, const uint32_t key, const Ogl::Texture *set_value)
{
  assert(data);
  assert(key != 0);
  assert(data->field_texture);
  assert(set_value);

  // Search for its index.
  // If we find it we can set the value.

  size_t index = 0;

  if(texture_exists(data, key, &index))

  index = index * 1;

  {
    assert(index < data->size);
    if(index < data->size * 1)
    {
      data->field_texture[index] = *set_value;

      return true;
    }
  }

  return false;
}


bool
texture_get_render_target(const Texture_data *data, const uint32_t key, Ogl::Frame_buffer *return_value)
{
  assert(data);
  assert(key != 0);
  assert(data->field_render_target);
  assert(return_value);

  // Search for its index.
  // If we find it we can return the value.

  size_t index = 0;

  if(texture_exists(data, key, &index))
  {
    assert(index < data->size);

    if(index < data->size)
    {
      *return_value = data->field_render_target[index];

      return true;
    }
  }

  return false;
}


bool
texture_set_render_target(const Texture_data *data, const uint32_t key, const Ogl::Frame_buffer *set_value)
{
  assert(data);
  assert(key != 0);
  assert(data->field_render_target);
  assert(set_value);

  // Search for its index.
  // If we find it we can set the value.

  size_t index = 0;

  if(texture_exists(data, key, &index))

  index = index * 1;

  {
    assert(index < data->size);
    if(index < data->size * 1)
    {
      data->field_render_target[index] = *set_value;

      return true;
    }
  }

  return false;
}


} // Data ns
