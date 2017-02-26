
/*
  Warning
  --
  This file is auto generated changes here may be overwritten.
*/


#include "text_mesh_data.hpp"
#include <assert.h>
#include <cstring>


namespace Data {


// ====================== //
// COMMON MODEL FUNCTIONS //
// ====================== //

namespace {

// Key our keys unique.
uint32_t text_mesh_model_key_instance_number{0};

} // anon ns


void
data_lock(const Text_mesh_data *data)
{
  // Not yet impl.
}


void
data_unlock(const Text_mesh_data *data)
{
  // Not yet impl.
}


bool
text_mesh_create(Text_mesh_data *data, const size_t size_hint)
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

    // Alloc space for text
    if(all_alloc)
    {
      data->field_text = new char[size_hint * 32];
      assert(data->field_text);
      if(!data->field_text) { all_alloc = false; }
      else { memset(data->field_text, 0, sizeof(char) * size_hint * 32); }
    }

    // Alloc space for text_size
    if(all_alloc)
    {
      data->field_text_size = new uint32_t[size_hint * 1];
      assert(data->field_text_size);
      if(!data->field_text_size) { all_alloc = false; }
      else { memset(data->field_text_size, 0, sizeof(uint32_t) * size_hint * 1); }
    }

    // Alloc space for font_id
    if(all_alloc)
    {
      data->field_font_id = new uint32_t[size_hint * 1];
      assert(data->field_font_id);
      if(!data->field_font_id) { all_alloc = false; }
      else { memset(data->field_font_id, 0, sizeof(uint32_t) * size_hint * 1); }
    }

    // Alloc space for mesh
    if(all_alloc)
    {
      data->field_mesh = new Graphics_api::Mesh[size_hint * 1];
      assert(data->field_mesh);
      if(!data->field_mesh) { all_alloc = false; }
      else { memset(data->field_mesh, 0, sizeof(Graphics_api::Mesh) * size_hint * 1); }
    }
  }

  // Failed so clean up.
  if(!all_alloc)
  {
    text_mesh_destroy(data);
  }

  return all_alloc;
}


void
text_mesh_destroy(Text_mesh_data *data)
{
  // Free up the memory.
  {
    // Remove keys
    if(data->keys) { delete[] data->keys; }
    data->keys = nullptr;

    // Remove text
    if(data->field_text) { delete[] data->field_text; }
    data->field_text = nullptr;

    // Remove text_size
    if(data->field_text_size) { delete[] data->field_text_size; }
    data->field_text_size = nullptr;

    // Remove font_id
    if(data->field_font_id) { delete[] data->field_font_id; }
    data->field_font_id = nullptr;

    // Remove mesh
    if(data->field_mesh) { delete[] data->field_mesh; }
    data->field_mesh = nullptr;
  }

  // Zero capacity and size
  {
    data->size = 0;

    size_t *capacity = const_cast<size_t*>(&data->capacity);
    *capacity = 0;
  }
}


uint32_t
text_mesh_push(Text_mesh_data *data)
{
  assert(data);
  assert(data->keys);

  // Do we need to resize?
  if(data->size >= data->capacity)
  {
    text_mesh_resize_capacity(data, data->capacity << 1);
  }

  // Push key at the back
  {
    const uint32_t new_key = ++text_mesh_model_key_instance_number;
    data->keys[data->size++] = new_key;

    return new_key;
  }


  return 0;
}



bool
text_mesh_remove(Text_mesh_data *data, const uint32_t key)
{
  size_t index_to_erase = 0;

  if(text_mesh_exists(data, key, &index_to_erase))
  {
    assert(index_to_erase < data->size);

    const size_t start_index = index_to_erase + 1;
    const size_t size_to_end = data->size - index_to_erase - 1;

    --(data->size);

    // Shuffle the data down
    memmove(&data->keys[index_to_erase], &data->keys[start_index], size_to_end * sizeof(*data->keys));
    memmove(&data->field_text[index_to_erase * 32], &data->field_text[start_index * 32], size_to_end * sizeof(*data->field_text) * 32);
    memmove(&data->field_text_size[index_to_erase * 1], &data->field_text_size[start_index * 1], size_to_end * sizeof(*data->field_text_size) * 1);
    memmove(&data->field_font_id[index_to_erase * 1], &data->field_font_id[start_index * 1], size_to_end * sizeof(*data->field_font_id) * 1);
    memmove(&data->field_mesh[index_to_erase * 1], &data->field_mesh[start_index * 1], size_to_end * sizeof(*data->field_mesh) * 1);

    return true;
  }

  return false;
}


bool
text_mesh_exists(const Text_mesh_data *data, const uint32_t key, size_t *out_index)
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
text_mesh_clear(Text_mesh_data *data)
{
  assert(data);

  data->size = 0;
}


bool
text_mesh_is_empty(const Text_mesh_data *data, const size_t size_hint)
{
  assert(data);

  if(!data) { return false; }

  return !!data->size;
}


size_t
text_mesh_get_size(const Text_mesh_data *data)
{
  assert(data);

  if(!data) { return 0; }

  return data->size;
}


size_t
text_mesh_get_capacity(const Text_mesh_data *data)
{
  assert(data);

  if(!data) { return 0; }

  return data->capacity;
}


bool
text_mesh_resize_capacity(Text_mesh_data *data, const size_t size_hint)
{
  assert(data);
  assert(size_hint > data->size); // Will slice data

  // Create new data.
  Text_mesh_data new_data;
  const bool created_new = text_mesh_create(&new_data, size_hint);

  // Failed to resize.
  if(!created_new)
  {
    text_mesh_destroy(&new_data);
    return false;
  }

  // Copy over data
  {
    memcpy(new_data.keys, data->keys, sizeof(uint32_t) * data->size);
    memcpy(new_data.field_text, data->field_text, sizeof(char) * data->size * 32);
    memcpy(new_data.field_text_size, data->field_text_size, sizeof(uint32_t) * data->size * 1);
    memcpy(new_data.field_font_id, data->field_font_id, sizeof(uint32_t) * data->size * 1);
    memcpy(new_data.field_mesh, data->field_mesh, sizeof(Graphics_api::Mesh) * data->size * 1);
  }

  // Swap ptrs
  {
    uint32_t *old_keys = data->keys;
    data->keys = new_data.keys;
    new_data.keys = old_keys;

    char *old_text = data->field_text;
    data->field_text = new_data.field_text;
    new_data.field_text = old_text;

    uint32_t *old_text_size = data->field_text_size;
    data->field_text_size = new_data.field_text_size;
    new_data.field_text_size = old_text_size;

    uint32_t *old_font_id = data->field_font_id;
    data->field_font_id = new_data.field_font_id;
    new_data.field_font_id = old_font_id;

    Graphics_api::Mesh *old_mesh = data->field_mesh;
    data->field_mesh = new_data.field_mesh;
    new_data.field_mesh = old_mesh;
  }

  // Set the Capacity
  {
    size_t *capacity = const_cast<size_t*>(&data->capacity);
    *capacity = new_data.capacity;
  }

  // Destroy new data
  text_mesh_destroy(&new_data);

  return true;
}



// ===================== //
// DATA GETTER FUNCTIONS //
// ===================== //


const char*
text_mesh_get_const_text_data(const Text_mesh_data *data)
{
  assert(data);
  assert(data->field_text);

  return data->field_text;
}


char*
text_mesh_get_text_data(Text_mesh_data *data)
{
  assert(data);
  assert(data->field_text);

  return data->field_text;
}


const uint32_t*
text_mesh_get_const_text_size_data(const Text_mesh_data *data)
{
  assert(data);
  assert(data->field_text_size);

  return data->field_text_size;
}


uint32_t*
text_mesh_get_text_size_data(Text_mesh_data *data)
{
  assert(data);
  assert(data->field_text_size);

  return data->field_text_size;
}


const uint32_t*
text_mesh_get_const_font_id_data(const Text_mesh_data *data)
{
  assert(data);
  assert(data->field_font_id);

  return data->field_font_id;
}


uint32_t*
text_mesh_get_font_id_data(Text_mesh_data *data)
{
  assert(data);
  assert(data->field_font_id);

  return data->field_font_id;
}


const Graphics_api::Mesh*
text_mesh_get_const_mesh_data(const Text_mesh_data *data)
{
  assert(data);
  assert(data->field_mesh);

  return data->field_mesh;
}


Graphics_api::Mesh*
text_mesh_get_mesh_data(Text_mesh_data *data)
{
  assert(data);
  assert(data->field_mesh);

  return data->field_mesh;
}


// =============== //
// FIELD FUNCTIONS //
// =============== //


bool
text_mesh_get_text(const Text_mesh_data *data, const uint32_t key, const char **return_value)
{
  assert(data);
  assert(key != 0);
  assert(data->field_text);
  assert(return_value);

  // Search for its index.
  // If we find it we can return the value.

  size_t index = 0;

  if(text_mesh_exists(data, key, &index))
  {
    assert(index < data->size);

    if(index < data->size)
    {
      *return_value = &data->field_text[index * 32];

      return true;
    }
  }

  return false;
}


bool
text_mesh_set_text(const Text_mesh_data *data, const uint32_t key, const char *set_value, const size_t size)
{
  assert(data);
  assert(key != 0);
  assert(data->field_text);
  assert(set_value);

  // Search for its index.
  // If we find it we can set the value.

  size_t index = 0;

  if(text_mesh_exists(data, key, &index))
  {
    assert(index < data->size);
    if(index < data->size)
    {
      memcpy(&data->field_text[index * 32], set_value, sizeof(char) * size);

      return true;
    }
  }

  return false;
}


bool
text_mesh_get_text_size(const Text_mesh_data *data, const uint32_t key, uint32_t *return_value)
{
  assert(data);
  assert(key != 0);
  assert(data->field_text_size);
  assert(return_value);

  // Search for its index.
  // If we find it we can return the value.

  size_t index = 0;

  if(text_mesh_exists(data, key, &index))
  {
    assert(index < data->size);

    if(index < data->size)
    {
      *return_value = data->field_text_size[index];

      return true;
    }
  }

  return false;
}


bool
text_mesh_set_text_size(const Text_mesh_data *data, const uint32_t key, const uint32_t *set_value)
{
  assert(data);
  assert(key != 0);
  assert(data->field_text_size);
  assert(set_value);

  // Search for its index.
  // If we find it we can set the value.

  size_t index = 0;

  if(text_mesh_exists(data, key, &index))
  {
    assert(index < data->size);
    if(index < data->size)
    {
      data->field_text_size[index] = *set_value;

      return true;
    }
  }

  return false;
}


bool
text_mesh_get_font_id(const Text_mesh_data *data, const uint32_t key, uint32_t *return_value)
{
  assert(data);
  assert(key != 0);
  assert(data->field_font_id);
  assert(return_value);

  // Search for its index.
  // If we find it we can return the value.

  size_t index = 0;

  if(text_mesh_exists(data, key, &index))
  {
    assert(index < data->size);

    if(index < data->size)
    {
      *return_value = data->field_font_id[index];

      return true;
    }
  }

  return false;
}


bool
text_mesh_set_font_id(const Text_mesh_data *data, const uint32_t key, const uint32_t *set_value)
{
  assert(data);
  assert(key != 0);
  assert(data->field_font_id);
  assert(set_value);

  // Search for its index.
  // If we find it we can set the value.

  size_t index = 0;

  if(text_mesh_exists(data, key, &index))
  {
    assert(index < data->size);
    if(index < data->size)
    {
      data->field_font_id[index] = *set_value;

      return true;
    }
  }

  return false;
}


bool
text_mesh_get_mesh(const Text_mesh_data *data, const uint32_t key, Graphics_api::Mesh *return_value)
{
  assert(data);
  assert(key != 0);
  assert(data->field_mesh);
  assert(return_value);

  // Search for its index.
  // If we find it we can return the value.

  size_t index = 0;

  if(text_mesh_exists(data, key, &index))
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
text_mesh_set_mesh(const Text_mesh_data *data, const uint32_t key, const Graphics_api::Mesh *set_value)
{
  assert(data);
  assert(key != 0);
  assert(data->field_mesh);
  assert(set_value);

  // Search for its index.
  // If we find it we can set the value.

  size_t index = 0;

  if(text_mesh_exists(data, key, &index))
  {
    assert(index < data->size);
    if(index < data->size)
    {
      data->field_mesh[index] = *set_value;

      return true;
    }
  }

  return false;
}


} // Data ns
