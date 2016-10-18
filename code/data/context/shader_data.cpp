
/*
  Warning
  --
  This file is auto generated changes here may be overwritten.
*/


#include "shader_data.hpp"
#include <assert.h>
#include <cstring>


namespace Data {


// ====================== //
// COMMON MODEL FUNCTIONS //
// ====================== //

namespace {

// Key our keys unique.
uint32_t shader_model_key_instance_number{0};

} // anon ns


void
data_lock(const Shader_data *data)
{
  // Not yet impl.
}


void
data_unlock(const Shader_data *data)
{
  // Not yet impl.
}


bool
shader_create(Shader_data *data, const size_t size_hint)
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

    // Alloc space for shader
    if(all_alloc)
    {
      data->field_shader = new Ogl::Shader[size_hint * 1];
      assert(data->field_shader);
      if(!data->field_shader) { all_alloc = false; }
      else { memset(data->field_shader, 0, sizeof(Ogl::Shader) * size_hint * 1); }
    }
  }

  // Failed so clean up.
  if(!all_alloc)
  {
    shader_destroy(data);
  }

  return all_alloc;
}


void
shader_destroy(Shader_data *data)
{
  // Free up the memory.
  {
    // Remove keys
    if(data->keys) { delete[] data->keys; }
    data->keys = nullptr;

    // Remove name
    if(data->field_name) { delete[] data->field_name; }
    data->field_name = nullptr;

    // Remove shader
    if(data->field_shader) { delete[] data->field_shader; }
    data->field_shader = nullptr;
  }

  // Zero capacity and size
  {
    data->size = 0;

    size_t *capacity = const_cast<size_t*>(&data->capacity);
    *capacity = 0;
  }
}


uint32_t
shader_push(Shader_data *data)
{
  assert(data);
  assert(data->keys);

  // Do we need to resize?
  if(data->size >= data->capacity)
  {
    shader_resize_capacity(data, data->capacity << 1);
  }

  // Push key at the back
  {
    const uint32_t new_key = ++shader_model_key_instance_number;
    data->keys[data->size++] = new_key;

    return new_key;
  }


  return 0;
}



bool
shader_remove(Shader_data *data, const uint32_t key)
{
  size_t index_to_erase = 0;

  if(shader_exists(data, key, &index_to_erase))
  {
    assert(index_to_erase < data->size);

    const size_t start_index = index_to_erase + 1;
    const size_t size_to_end = data->size - index_to_erase - 1;

    --(data->size);

    // Shuffle the data down
    memmove(&data->keys[index_to_erase], &data->keys[start_index], size_to_end * sizeof(*data->keys));
    memmove(&data->field_name[index_to_erase * 32], &data->field_name[start_index * 32], size_to_end * sizeof(*data->field_name) * 32);
    memmove(&data->field_shader[index_to_erase * 1], &data->field_shader[start_index * 1], size_to_end * sizeof(*data->field_shader) * 1);

    return true;
  }

  return false;
}


bool
shader_exists(const Shader_data *data, const uint32_t key, size_t *out_index)
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
shader_clear(Shader_data *data)
{
  assert(data);

  data->size = 0;
}


bool
shader_is_empty(const Shader_data *data, const size_t size_hint)
{
  assert(data);

  if(!data) { return false; }

  return !!data->size;
}


size_t
shader_get_size(const Shader_data *data)
{
  assert(data);

  if(!data) { return 0; }

  return data->size;
}


size_t
shader_get_capacity(const Shader_data *data)
{
  assert(data);

  if(!data) { return 0; }

  return data->capacity;
}


bool
shader_resize_capacity(Shader_data *data, const size_t size_hint)
{
  assert(data);
  assert(size_hint > data->size); // Will slice data

  // Create new data.
  Shader_data new_data;
  const bool created_new = shader_create(&new_data, size_hint);

  // Failed to resize.
  if(!created_new)
  {
    shader_destroy(&new_data);
    return false;
  }

  // Copy over data
  {
    memcpy(new_data.keys, data->keys, sizeof(uint32_t) * data->size);
    memcpy(new_data.field_name, data->field_name, sizeof(char) * data->size * 32);
    memcpy(new_data.field_shader, data->field_shader, sizeof(Ogl::Shader) * data->size * 1);
  }

  // Swap ptrs
  {
    uint32_t *old_keys = data->keys;
    data->keys = new_data.keys;
    new_data.keys = old_keys;

    char *old_name = data->field_name;
    data->field_name = new_data.field_name;
    new_data.field_name = old_name;

    Ogl::Shader *old_shader = data->field_shader;
    data->field_shader = new_data.field_shader;
    new_data.field_shader = old_shader;
  }

  // Set the Capacity
  {
    size_t *capacity = const_cast<size_t*>(&data->capacity);
    *capacity = new_data.capacity;
  }

  // Destroy new data
  shader_destroy(&new_data);

  return true;
}



// ===================== //
// DATA GETTER FUNCTIONS //
// ===================== //


const char*
shader_get_const_name_data(const Shader_data *data)
{
  assert(data);
  assert(data->field_name);

  return data->field_name;
}


char*
shader_get_name_data(Shader_data *data)
{
  assert(data);
  assert(data->field_name);

  return data->field_name;
}


const Ogl::Shader*
shader_get_const_shader_data(const Shader_data *data)
{
  assert(data);
  assert(data->field_shader);

  return data->field_shader;
}


Ogl::Shader*
shader_get_shader_data(Shader_data *data)
{
  assert(data);
  assert(data->field_shader);

  return data->field_shader;
}


// =============== //
// FIELD FUNCTIONS //
// =============== //


bool
shader_get_name(const Shader_data *data, const uint32_t key, const char **return_value)
{
  assert(data);
  assert(key != 0);
  assert(data->field_name);
  assert(return_value);

  // Search for its index.
  // If we find it we can return the value.

  size_t index = 0;

  if(shader_exists(data, key, &index))
  {
    assert(index < data->size);

    if(index < data->size)
    {
      *return_value = &data->field_name[index * 32];

      return true;
    }
  }

  return false;
}


bool
shader_set_name(const Shader_data *data, const uint32_t key, const char *set_value, const size_t size)
{
  assert(data);
  assert(key != 0);
  assert(data->field_name);
  assert(set_value);

  // Search for its index.
  // If we find it we can set the value.

  size_t index = 0;

  if(shader_exists(data, key, &index))
  {
    assert(index < data->size);
    if(index < data->size)
    {
      memcpy(&data->field_name[index * 32], set_value, sizeof(char) * size);

      return true;
    }
  }

  return false;
}


bool
shader_get_shader(const Shader_data *data, const uint32_t key, Ogl::Shader *return_value)
{
  assert(data);
  assert(key != 0);
  assert(data->field_shader);
  assert(return_value);

  // Search for its index.
  // If we find it we can return the value.

  size_t index = 0;

  if(shader_exists(data, key, &index))
  {
    assert(index < data->size);

    if(index < data->size)
    {
      *return_value = data->field_shader[index];

      return true;
    }
  }

  return false;
}


bool
shader_set_shader(const Shader_data *data, const uint32_t key, const Ogl::Shader *set_value)
{
  assert(data);
  assert(key != 0);
  assert(data->field_shader);
  assert(set_value);

  // Search for its index.
  // If we find it we can set the value.

  size_t index = 0;

  if(shader_exists(data, key, &index))
  {
    assert(index < data->size);
    if(index < data->size)
    {
      data->field_shader[index] = *set_value;

      return true;
    }
  }

  return false;
}


} // Data ns
