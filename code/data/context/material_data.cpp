
/*
  Warning
  --
  This file is auto generated changes here may be overwritten.
*/


#include "material_data.hpp"
#include <assert.h>
#include <cstring>


namespace Data {


// ====================== //
// COMMON MODEL FUNCTIONS //
// ====================== //

namespace {

// Key our keys unique.
uint32_t material_model_key_instance_number{0};

} // anon ns


void
data_lock(const Material_data *data)
{
  // Not yet impl.
}


void
data_unlock(const Material_data *data)
{
  // Not yet impl.
}


bool
material_create(Material_data *data, const size_t size_hint)
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

    // Alloc space for material_hash
    if(all_alloc)
    {
      data->field_material_hash = new Material_renderer::Material_id[size_hint * 1];
      assert(data->field_material_hash);
      if(!data->field_material_hash) { all_alloc = false; }
      else { memset(data->field_material_hash, 0, sizeof(Material_renderer::Material_id) * size_hint * 1); }
    }

    // Alloc space for material
    if(all_alloc)
    {
      data->field_material = new Material_renderer::Material[size_hint * 1];
      assert(data->field_material);
      if(!data->field_material) { all_alloc = false; }
      else { memset(data->field_material, 0, sizeof(Material_renderer::Material) * size_hint * 1); }
    }
  }

  // Failed so clean up.
  if(!all_alloc)
  {
    material_destroy(data);
  }

  return all_alloc;
}


void
material_destroy(Material_data *data)
{
  // Free up the memory.
  {
    // Remove keys
    if(data->keys) { delete[] data->keys; }
    data->keys = nullptr;

    // Remove name
    if(data->field_name) { delete[] data->field_name; }
    data->field_name = nullptr;

    // Remove material_hash
    if(data->field_material_hash) { delete[] data->field_material_hash; }
    data->field_material_hash = nullptr;

    // Remove material
    if(data->field_material) { delete[] data->field_material; }
    data->field_material = nullptr;
  }

  // Zero capacity and size
  {
    data->size = 0;

    size_t *capacity = const_cast<size_t*>(&data->capacity);
    *capacity = 0;
  }
}


uint32_t
material_push(Material_data *data)
{
  assert(data);
  assert(data->keys);

  // Do we need to resize?
  if(data->size >= data->capacity)
  {
    material_resize_capacity(data, data->capacity << 1);
  }

  // Push key at the back
  {
    const uint32_t new_key = ++material_model_key_instance_number;
    data->keys[data->size++] = new_key;

    return new_key;
  }


  return 0;
}



bool
material_remove(Material_data *data, const uint32_t key)
{
  size_t index_to_erase = 0;

  if(material_exists(data, key, &index_to_erase))
  {
    assert(index_to_erase < data->size);

    const size_t start_index = index_to_erase + 1;
    const size_t size_to_end = data->size - index_to_erase - 1;

    --(data->size);

    // Shuffle the data down
    memmove(&data->keys[index_to_erase], &data->keys[start_index], size_to_end * sizeof(*data->keys));
    memmove(&data->field_name[index_to_erase * 32], &data->field_name[start_index * 32], size_to_end * sizeof(*data->field_name) * 32);
    memmove(&data->field_material_hash[index_to_erase * 1], &data->field_material_hash[start_index * 1], size_to_end * sizeof(*data->field_material_hash) * 1);
    memmove(&data->field_material[index_to_erase * 1], &data->field_material[start_index * 1], size_to_end * sizeof(*data->field_material) * 1);

    return true;
  }

  return false;
}


bool
material_exists(const Material_data *data, const uint32_t key, size_t *out_index)
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
material_clear(Material_data *data)
{
  assert(data);

  data->size = 0;
}


bool
material_is_empty(const Material_data *data, const size_t size_hint)
{
  assert(data);

  if(!data) { return false; }

  return !!data->size;
}


size_t
material_get_size(const Material_data *data)
{
  assert(data);

  if(!data) { return 0; }

  return data->size;
}


size_t
material_get_capacity(const Material_data *data)
{
  assert(data);

  if(!data) { return 0; }

  return data->capacity;
}


bool
material_resize_capacity(Material_data *data, const size_t size_hint)
{
  assert(data);
  assert(size_hint > data->size); // Will slice data

  // Create new data.
  Material_data new_data;
  const bool created_new = material_create(&new_data, size_hint);

  // Failed to resize.
  if(!created_new)
  {
    material_destroy(&new_data);
    return false;
  }

  // Copy over data
  {
    memcpy(new_data.keys, data->keys, sizeof(uint32_t) * data->size);
    memcpy(new_data.field_name, data->field_name, sizeof(char) * data->size * 32);
    memcpy(new_data.field_material_hash, data->field_material_hash, sizeof(Material_renderer::Material_id) * data->size * 1);
    memcpy(new_data.field_material, data->field_material, sizeof(Material_renderer::Material) * data->size * 1);
  }

  // Swap ptrs
  {
    uint32_t *old_keys = data->keys;
    data->keys = new_data.keys;
    new_data.keys = old_keys;

    char *old_name = data->field_name;
    data->field_name = new_data.field_name;
    new_data.field_name = old_name;

    Material_renderer::Material_id *old_material_hash = data->field_material_hash;
    data->field_material_hash = new_data.field_material_hash;
    new_data.field_material_hash = old_material_hash;

    Material_renderer::Material *old_material = data->field_material;
    data->field_material = new_data.field_material;
    new_data.field_material = old_material;
  }

  // Set the Capacity
  {
    size_t *capacity = const_cast<size_t*>(&data->capacity);
    *capacity = new_data.capacity;
  }

  // Destroy new data
  material_destroy(&new_data);

  return true;
}



// ===================== //
// DATA GETTER FUNCTIONS //
// ===================== //


const char*
material_get_const_name_data(const Material_data *data)
{
  assert(data);
  assert(data->field_name);

  return data->field_name;
}


char*
material_get_name_data(Material_data *data)
{
  assert(data);
  assert(data->field_name);

  return data->field_name;
}


const Material_renderer::Material_id*
material_get_const_material_hash_data(const Material_data *data)
{
  assert(data);
  assert(data->field_material_hash);

  return data->field_material_hash;
}


Material_renderer::Material_id*
material_get_material_hash_data(Material_data *data)
{
  assert(data);
  assert(data->field_material_hash);

  return data->field_material_hash;
}


const Material_renderer::Material*
material_get_const_material_data(const Material_data *data)
{
  assert(data);
  assert(data->field_material);

  return data->field_material;
}


Material_renderer::Material*
material_get_material_data(Material_data *data)
{
  assert(data);
  assert(data->field_material);

  return data->field_material;
}


// =============== //
// FIELD FUNCTIONS //
// =============== //


bool
material_get_name(const Material_data *data, const uint32_t key, const char **return_value)
{
  assert(data);
  assert(key != 0);
  assert(data->field_name);
  assert(return_value);

  // Search for its index.
  // If we find it we can return the value.

  size_t index = 0;

  if(material_exists(data, key, &index))
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
material_set_name(const Material_data *data, const uint32_t key, const char *set_value, const size_t size)
{
  assert(data);
  assert(key != 0);
  assert(data->field_name);
  assert(set_value);

  // Search for its index.
  // If we find it we can set the value.

  size_t index = 0;

  if(material_exists(data, key, &index))
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
material_get_material_hash(const Material_data *data, const uint32_t key, Material_renderer::Material_id *return_value)
{
  assert(data);
  assert(key != 0);
  assert(data->field_material_hash);
  assert(return_value);

  // Search for its index.
  // If we find it we can return the value.

  size_t index = 0;

  if(material_exists(data, key, &index))
  {
    assert(index < data->size);

    if(index < data->size)
    {
      *return_value = data->field_material_hash[index];

      return true;
    }
  }

  return false;
}


bool
material_set_material_hash(const Material_data *data, const uint32_t key, const Material_renderer::Material_id *set_value)
{
  assert(data);
  assert(key != 0);
  assert(data->field_material_hash);
  assert(set_value);

  // Search for its index.
  // If we find it we can set the value.

  size_t index = 0;

  if(material_exists(data, key, &index))
  {
    assert(index < data->size);
    if(index < data->size)
    {
      data->field_material_hash[index] = *set_value;

      return true;
    }
  }

  return false;
}


bool
material_get_material(const Material_data *data, const uint32_t key, Material_renderer::Material *return_value)
{
  assert(data);
  assert(key != 0);
  assert(data->field_material);
  assert(return_value);

  // Search for its index.
  // If we find it we can return the value.

  size_t index = 0;

  if(material_exists(data, key, &index))
  {
    assert(index < data->size);

    if(index < data->size)
    {
      *return_value = data->field_material[index];

      return true;
    }
  }

  return false;
}


bool
material_set_material(const Material_data *data, const uint32_t key, const Material_renderer::Material *set_value)
{
  assert(data);
  assert(key != 0);
  assert(data->field_material);
  assert(set_value);

  // Search for its index.
  // If we find it we can set the value.

  size_t index = 0;

  if(material_exists(data, key, &index))
  {
    assert(index < data->size);
    if(index < data->size)
    {
      data->field_material[index] = *set_value;

      return true;
    }
  }

  return false;
}


} // Data ns
