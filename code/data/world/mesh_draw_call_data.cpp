
/*
  Warning
  --
  This file is auto generated changes here may be overwritten.
*/


#include "mesh_draw_call_data.hpp"
#include <assert.h>
#include <cstring>


namespace Data {


// ====================== //
// COMMON MODEL FUNCTIONS //
// ====================== //



void
data_lock(const Mesh_draw_call_data *data)
{
  // Not yet impl.
}


void
data_unlock(const Mesh_draw_call_data *data)
{
  // Not yet impl.
}


bool
mesh_draw_call_create(Mesh_draw_call_data *data, const size_t size_hint)
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

    // Alloc space for draw_call
    if(all_alloc)
    {
      data->field_draw_call = new Mesh_renderer_draw_call[size_hint * 1];
      assert(data->field_draw_call);
      if(!data->field_draw_call) { all_alloc = false; }
      else { memset(data->field_draw_call, 0, sizeof(Mesh_renderer_draw_call) * size_hint * 1); }
    }

    // Alloc space for material_id
    if(all_alloc)
    {
      data->field_material_id = new uint32_t[size_hint * 1];
      assert(data->field_material_id);
      if(!data->field_material_id) { all_alloc = false; }
      else { memset(data->field_material_id, 0, sizeof(uint32_t) * size_hint * 1); }
    }
  }

  // Failed so clean up.
  if(!all_alloc)
  {
    mesh_draw_call_destroy(data);
  }

  return all_alloc;
}


void
mesh_draw_call_destroy(Mesh_draw_call_data *data)
{
  // Free up the memory.
  {
    // Remove keys
    if(data->keys) { delete[] data->keys; }
    data->keys = nullptr;

    // Remove draw_call
    if(data->field_draw_call) { delete[] data->field_draw_call; }
    data->field_draw_call = nullptr;

    // Remove material_id
    if(data->field_material_id) { delete[] data->field_material_id; }
    data->field_material_id = nullptr;
  }

  // Zero capacity and size
  {
    data->size = 0;

    size_t *capacity = const_cast<size_t*>(&data->capacity);
    *capacity = 0;
  }
}


uint32_t
mesh_draw_call_push(Mesh_draw_call_data *data, const uint32_t key)
{
  assert(data);
  assert(data->keys);

  // Do we need to resize?
  if(data->size >= data->capacity)
  {
    mesh_draw_call_resize_capacity(data, data->capacity << 1);
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
mesh_draw_call_insert(Mesh_draw_call_data *data, const uint32_t key, const size_t insert_point)
{
  assert(data);
  assert(data->keys);
  assert(key);

  // Do we need to resize?
  if(data->size >= data->capacity)
  {
    mesh_draw_call_resize_capacity(data, data->capacity << 1);
  }

  // Shuffle memory up
  {
    const size_t insert_index = insert_point + 1;
    const size_t size_to_end = data->size - insert_point;

    // Shuffle the data down
    memmove(&data->keys[insert_index], &data->keys[insert_point], size_to_end * sizeof(*data->keys));
    memmove(&data->field_draw_call[insert_index * 1], &data->field_draw_call[insert_point * 1], size_to_end * sizeof(*data->field_draw_call) * 1);
    memmove(&data->field_material_id[insert_index * 1], &data->field_material_id[insert_point * 1], size_to_end * sizeof(*data->field_material_id) * 1);

    data->keys[insert_point] = key;
  }

  // Insert new data
  {
    ++data->size;
  }

  return true;
}


bool
mesh_draw_call_remove(Mesh_draw_call_data *data, const uint32_t key)
{
  size_t index_to_erase = 0;

  if(mesh_draw_call_exists(data, key, &index_to_erase))
  {
    assert(index_to_erase < data->size);

    const size_t start_index = index_to_erase + 1;
    const size_t size_to_end = data->size - index_to_erase - 1;

    --(data->size);

    // Shuffle the data down
    memmove(&data->keys[index_to_erase], &data->keys[start_index], size_to_end * sizeof(*data->keys));
    memmove(&data->field_draw_call[index_to_erase * 1], &data->field_draw_call[start_index * 1], size_to_end * sizeof(*data->field_draw_call) * 1);
    memmove(&data->field_material_id[index_to_erase * 1], &data->field_material_id[start_index * 1], size_to_end * sizeof(*data->field_material_id) * 1);

    return true;
  }

  return false;
}


bool
mesh_draw_call_exists(const Mesh_draw_call_data *data, const uint32_t key, size_t *out_index)
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
mesh_draw_call_clear(Mesh_draw_call_data *data)
{
  assert(data);

  data->size = 0;
}


bool
mesh_draw_call_is_empty(const Mesh_draw_call_data *data, const size_t size_hint)
{
  assert(data);

  if(!data) { return false; }

  return !!data->size;
}


size_t
mesh_draw_call_get_size(const Mesh_draw_call_data *data)
{
  assert(data);

  if(!data) { return 0; }

  return data->size;
}


size_t
mesh_draw_call_get_capacity(const Mesh_draw_call_data *data)
{
  assert(data);

  if(!data) { return 0; }

  return data->capacity;
}


bool
mesh_draw_call_resize_capacity(Mesh_draw_call_data *data, const size_t size_hint)
{
  assert(data);
  assert(size_hint > data->size); // Will slice data

  // Create new data.
  Mesh_draw_call_data new_data;
  const bool created_new = mesh_draw_call_create(&new_data, size_hint);

  // Failed to resize.
  if(!created_new)
  {
    mesh_draw_call_destroy(&new_data);
    return false;
  }

  // Copy over data
  {
    memcpy(new_data.keys, data->keys, sizeof(uint32_t) * data->size);
    memcpy(new_data.field_draw_call, data->field_draw_call, sizeof(Mesh_renderer_draw_call) * data->size * 1);
    memcpy(new_data.field_material_id, data->field_material_id, sizeof(uint32_t) * data->size * 1);
  }

  // Swap ptrs
  {
    uint32_t *old_keys = data->keys;
    data->keys = new_data.keys;
    new_data.keys = old_keys;

    Mesh_renderer_draw_call *old_draw_call = data->field_draw_call;
    data->field_draw_call = new_data.field_draw_call;
    new_data.field_draw_call = old_draw_call;

    uint32_t *old_material_id = data->field_material_id;
    data->field_material_id = new_data.field_material_id;
    new_data.field_material_id = old_material_id;
  }

  // Set the Capacity
  {
    size_t *capacity = const_cast<size_t*>(&data->capacity);
    *capacity = new_data.capacity;
  }

  // Destroy new data
  mesh_draw_call_destroy(&new_data);

  return true;
}



// ===================== //
// DATA GETTER FUNCTIONS //
// ===================== //


const Mesh_renderer_draw_call*
mesh_draw_call_get_const_draw_call_data(const Mesh_draw_call_data *data)
{
  assert(data);
  assert(data->field_draw_call);

  return data->field_draw_call;
}


Mesh_renderer_draw_call*
mesh_draw_call_get_draw_call_data(Mesh_draw_call_data *data)
{
  assert(data);
  assert(data->field_draw_call);

  return data->field_draw_call;
}


const uint32_t*
mesh_draw_call_get_const_material_id_data(const Mesh_draw_call_data *data)
{
  assert(data);
  assert(data->field_material_id);

  return data->field_material_id;
}


uint32_t*
mesh_draw_call_get_material_id_data(Mesh_draw_call_data *data)
{
  assert(data);
  assert(data->field_material_id);

  return data->field_material_id;
}


// =============== //
// FIELD FUNCTIONS //
// =============== //


bool
mesh_draw_call_get_draw_call(const Mesh_draw_call_data *data, const uint32_t key, Mesh_renderer_draw_call *return_value)
{
  assert(data);
  assert(key != 0);
  assert(data->field_draw_call);
  assert(return_value);

  // Search for its index.
  // If we find it we can return the value.

  size_t index = 0;

  if(mesh_draw_call_exists(data, key, &index))
  {
    assert(index < data->size);

    if(index < data->size)
    {
      *return_value = data->field_draw_call[index];

      return true;
    }
  }

  return false;
}


bool
mesh_draw_call_set_draw_call(const Mesh_draw_call_data *data, const uint32_t key, const Mesh_renderer_draw_call *set_value)
{
  assert(data);
  assert(key != 0);
  assert(data->field_draw_call);
  assert(set_value);

  // Search for its index.
  // If we find it we can set the value.

  size_t index = 0;

  if(mesh_draw_call_exists(data, key, &index))
  {
    assert(index < data->size);
    if(index < data->size)
    {
      data->field_draw_call[index] = *set_value;

      return true;
    }
  }

  return false;
}


bool
mesh_draw_call_get_material_id(const Mesh_draw_call_data *data, const uint32_t key, uint32_t *return_value)
{
  assert(data);
  assert(key != 0);
  assert(data->field_material_id);
  assert(return_value);

  // Search for its index.
  // If we find it we can return the value.

  size_t index = 0;

  if(mesh_draw_call_exists(data, key, &index))
  {
    assert(index < data->size);

    if(index < data->size)
    {
      *return_value = data->field_material_id[index];

      return true;
    }
  }

  return false;
}


bool
mesh_draw_call_set_material_id(const Mesh_draw_call_data *data, const uint32_t key, const uint32_t *set_value)
{
  assert(data);
  assert(key != 0);
  assert(data->field_material_id);
  assert(set_value);

  // Search for its index.
  // If we find it we can set the value.

  size_t index = 0;

  if(mesh_draw_call_exists(data, key, &index))
  {
    assert(index < data->size);
    if(index < data->size)
    {
      data->field_material_id[index] = *set_value;

      return true;
    }
  }

  return false;
}


} // Data ns
