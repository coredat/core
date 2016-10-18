
/*
  Warning
  --
  This file is auto generated changes here may be overwritten.
*/


#include "mesh_data.hpp"
#include <assert.h>
#include <cstring>


namespace Data {


// ====================== //
// COMMON MODEL FUNCTIONS //
// ====================== //

namespace {

// Key our keys unique.
uint32_t mesh_model_key_instance_number{0};

} // anon ns


void
data_lock(const Mesh_data *data)
{
  // Not yet impl.
}


void
data_unlock(const Mesh_data *data)
{
  // Not yet impl.
}


bool
mesh_create(Mesh_data *data, const size_t size_hint)
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

    // Alloc space for mesh
    if(all_alloc)
    {
      data->field_mesh = new Graphics_api::Mesh[size_hint * 1];
      assert(data->field_mesh);
      if(!data->field_mesh) { all_alloc = false; }
      else { memset(data->field_mesh, 0, sizeof(Graphics_api::Mesh) * size_hint * 1); }
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
    mesh_destroy(data);
  }

  return all_alloc;
}


void
mesh_destroy(Mesh_data *data)
{
  // Free up the memory.
  {
    // Remove keys
    if(data->keys) { delete[] data->keys; }
    data->keys = nullptr;

    // Remove name
    if(data->field_name) { delete[] data->field_name; }
    data->field_name = nullptr;

    // Remove mesh
    if(data->field_mesh) { delete[] data->field_mesh; }
    data->field_mesh = nullptr;

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
mesh_push(Mesh_data *data)
{
  assert(data);
  assert(data->keys);

  // Do we need to resize?
  if(data->size >= data->capacity)
  {
    mesh_resize_capacity(data, data->capacity << 1);
  }

  // Push key at the back
  {
    const uint32_t new_key = ++mesh_model_key_instance_number;
    data->keys[data->size++] = new_key;

    return new_key;
  }


  return 0;
}



bool
mesh_remove(Mesh_data *data, const uint32_t key)
{
  size_t index_to_erase = 0;

  if(mesh_exists(data, key, &index_to_erase))
  {
    assert(index_to_erase < data->size);

    const size_t start_index = index_to_erase + 1;
    const size_t size_to_end = data->size - index_to_erase - 1;

    --(data->size);

    // Shuffle the data down
    memmove(&data->keys[index_to_erase], &data->keys[start_index], size_to_end * sizeof(*data->keys));
    memmove(&data->field_name[index_to_erase * 32], &data->field_name[start_index * 32], size_to_end * sizeof(*data->field_name) * 32);
    memmove(&data->field_mesh[index_to_erase * 1], &data->field_mesh[start_index * 1], size_to_end * sizeof(*data->field_mesh) * 1);
    memmove(&data->field_aabb[index_to_erase * 1], &data->field_aabb[start_index * 1], size_to_end * sizeof(*data->field_aabb) * 1);

    return true;
  }

  return false;
}


bool
mesh_exists(const Mesh_data *data, const uint32_t key, size_t *out_index)
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
mesh_clear(Mesh_data *data)
{
  assert(data);

  data->size = 0;
}


bool
mesh_is_empty(const Mesh_data *data, const size_t size_hint)
{
  assert(data);

  if(!data) { return false; }

  return !!data->size;
}


size_t
mesh_get_size(const Mesh_data *data)
{
  assert(data);

  if(!data) { return 0; }

  return data->size;
}


size_t
mesh_get_capacity(const Mesh_data *data)
{
  assert(data);

  if(!data) { return 0; }

  return data->capacity;
}


bool
mesh_resize_capacity(Mesh_data *data, const size_t size_hint)
{
  assert(data);
  assert(size_hint > data->size); // Will slice data

  // Create new data.
  Mesh_data new_data;
  const bool created_new = mesh_create(&new_data, size_hint);

  // Failed to resize.
  if(!created_new)
  {
    mesh_destroy(&new_data);
    return false;
  }

  // Copy over data
  {
    memcpy(new_data.keys, data->keys, sizeof(uint32_t) * data->size);
    memcpy(new_data.field_name, data->field_name, sizeof(char) * data->size * 32);
    memcpy(new_data.field_mesh, data->field_mesh, sizeof(Graphics_api::Mesh) * data->size * 1);
    memcpy(new_data.field_aabb, data->field_aabb, sizeof(math::aabb) * data->size * 1);
  }

  // Swap ptrs
  {
    uint32_t *old_keys = data->keys;
    data->keys = new_data.keys;
    new_data.keys = old_keys;

    char *old_name = data->field_name;
    data->field_name = new_data.field_name;
    new_data.field_name = old_name;

    Graphics_api::Mesh *old_mesh = data->field_mesh;
    data->field_mesh = new_data.field_mesh;
    new_data.field_mesh = old_mesh;

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
  mesh_destroy(&new_data);

  return true;
}



// ===================== //
// DATA GETTER FUNCTIONS //
// ===================== //


const char*
mesh_get_const_name_data(const Mesh_data *data)
{
  assert(data);
  assert(data->field_name);

  return data->field_name;
}


char*
mesh_get_name_data(Mesh_data *data)
{
  assert(data);
  assert(data->field_name);

  return data->field_name;
}


const Graphics_api::Mesh*
mesh_get_const_mesh_data(const Mesh_data *data)
{
  assert(data);
  assert(data->field_mesh);

  return data->field_mesh;
}


Graphics_api::Mesh*
mesh_get_mesh_data(Mesh_data *data)
{
  assert(data);
  assert(data->field_mesh);

  return data->field_mesh;
}


const math::aabb*
mesh_get_const_aabb_data(const Mesh_data *data)
{
  assert(data);
  assert(data->field_aabb);

  return data->field_aabb;
}


math::aabb*
mesh_get_aabb_data(Mesh_data *data)
{
  assert(data);
  assert(data->field_aabb);

  return data->field_aabb;
}


// =============== //
// FIELD FUNCTIONS //
// =============== //


bool
mesh_get_name(const Mesh_data *data, const uint32_t key, const char **return_value)
{
  assert(data);
  assert(key != 0);
  assert(data->field_name);
  assert(return_value);

  // Search for its index.
  // If we find it we can return the value.

  size_t index = 0;

  if(mesh_exists(data, key, &index))
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
mesh_set_name(const Mesh_data *data, const uint32_t key, const char *set_value, const size_t size)
{
  assert(data);
  assert(key != 0);
  assert(data->field_name);
  assert(set_value);

  // Search for its index.
  // If we find it we can set the value.

  size_t index = 0;

  if(mesh_exists(data, key, &index))

  index = index * 32;

  {
    assert(index < data->size);
    if(index < data->size * 32)
    {
      memcpy(&data->field_name[index], set_value, sizeof(char) * size);

      return true;
    }
  }

  return false;
}


bool
mesh_get_mesh(const Mesh_data *data, const uint32_t key, Graphics_api::Mesh *return_value)
{
  assert(data);
  assert(key != 0);
  assert(data->field_mesh);
  assert(return_value);

  // Search for its index.
  // If we find it we can return the value.

  size_t index = 0;

  if(mesh_exists(data, key, &index))
  {
    assert(index < data->size);

    if(index < data->size)
    {
      *return_value = data->field_mesh[index];

      return true;
    }
  }

  return false;
}


bool
mesh_set_mesh(const Mesh_data *data, const uint32_t key, const Graphics_api::Mesh *set_value)
{
  assert(data);
  assert(key != 0);
  assert(data->field_mesh);
  assert(set_value);

  // Search for its index.
  // If we find it we can set the value.

  size_t index = 0;

  if(mesh_exists(data, key, &index))

  index = index * 1;

  {
    assert(index < data->size);
    if(index < data->size * 1)
    {
      data->field_mesh[index] = *set_value;

      return true;
    }
  }

  return false;
}


bool
mesh_get_aabb(const Mesh_data *data, const uint32_t key, math::aabb *return_value)
{
  assert(data);
  assert(key != 0);
  assert(data->field_aabb);
  assert(return_value);

  // Search for its index.
  // If we find it we can return the value.

  size_t index = 0;

  if(mesh_exists(data, key, &index))
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
mesh_set_aabb(const Mesh_data *data, const uint32_t key, const math::aabb *set_value)
{
  assert(data);
  assert(key != 0);
  assert(data->field_aabb);
  assert(set_value);

  // Search for its index.
  // If we find it we can set the value.

  size_t index = 0;

  if(mesh_exists(data, key, &index))

  index = index * 1;

  {
    assert(index < data->size);
    if(index < data->size * 1)
    {
      data->field_aabb[index] = *set_value;

      return true;
    }
  }

  return false;
}


} // Data ns
