
/*
  Warning
  --
  This file is auto generated changes here may be overwritten.
*/


#include "font_data.hpp"
#include <assert.h>
#include <cstring>


namespace Data {


// ====================== //
// COMMON MODEL FUNCTIONS //
// ====================== //

namespace {

// Key our keys unique.
uint32_t font_model_key_instance_number{0};

} // anon ns


void
data_lock(const Font_data *data)
{
  // Not yet impl.
}


void
data_unlock(const Font_data *data)
{
  // Not yet impl.
}


bool
font_create(Font_data *data, const size_t size_hint)
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

    // Alloc space for font_face
    if(all_alloc)
    {
      data->field_font_face = new stbtt_fontinfo[size_hint * 1];
      assert(data->field_font_face);
      if(!data->field_font_face) { all_alloc = false; }
      else { memset(data->field_font_face, 0, sizeof(stbtt_fontinfo) * size_hint * 1); }
    }

    // Alloc space for font_bitmap
    if(all_alloc)
    {
      data->field_font_bitmap = new Text::Font_bitmap[size_hint * 1];
      assert(data->field_font_bitmap);
      if(!data->field_font_bitmap) { all_alloc = false; }
      else { memset(data->field_font_bitmap, 0, sizeof(Text::Font_bitmap) * size_hint * 1); }
    }

    // Alloc space for texture_id
    if(all_alloc)
    {
      data->field_texture_id = new util::generic_id[size_hint * 1];
      assert(data->field_texture_id);
      if(!data->field_texture_id) { all_alloc = false; }
      else { memset(data->field_texture_id, 0, sizeof(util::generic_id) * size_hint * 1); }
    }
  }

  // Failed so clean up.
  if(!all_alloc)
  {
    font_destroy(data);
  }

  return all_alloc;
}


void
font_destroy(Font_data *data)
{
  // Free up the memory.
  {
    // Remove keys
    if(data->keys) { delete[] data->keys; }
    data->keys = nullptr;

    // Remove name
    if(data->field_name) { delete[] data->field_name; }
    data->field_name = nullptr;

    // Remove font_face
    if(data->field_font_face) { delete[] data->field_font_face; }
    data->field_font_face = nullptr;

    // Remove font_bitmap
    if(data->field_font_bitmap) { delete[] data->field_font_bitmap; }
    data->field_font_bitmap = nullptr;

    // Remove texture_id
    if(data->field_texture_id) { delete[] data->field_texture_id; }
    data->field_texture_id = nullptr;
  }

  // Zero capacity and size
  {
    data->size = 0;

    size_t *capacity = const_cast<size_t*>(&data->capacity);
    *capacity = 0;
  }
}


uint32_t
font_push(Font_data *data)
{
  assert(data);
  assert(data->keys);

  // Do we need to resize?
  if(data->size >= data->capacity)
  {
    font_resize_capacity(data, data->capacity << 1);
  }

  // Push key at the back
  {
    const uint32_t new_key = ++font_model_key_instance_number;
    data->keys[data->size++] = new_key;

    return new_key;
  }


  return 0;
}



bool
font_remove(Font_data *data, const uint32_t key)
{
  size_t index_to_erase = 0;

  if(font_exists(data, key, &index_to_erase))
  {
    assert(index_to_erase < data->size);

    const size_t start_index = index_to_erase + 1;
    const size_t size_to_end = data->size - index_to_erase - 1;

    --(data->size);

    // Shuffle the data down
    memmove(&data->keys[index_to_erase], &data->keys[start_index], size_to_end * sizeof(*data->keys));
    memmove(&data->field_name[index_to_erase * 32], &data->field_name[start_index * 32], size_to_end * sizeof(*data->field_name) * 32);
    memmove(&data->field_font_face[index_to_erase * 1], &data->field_font_face[start_index * 1], size_to_end * sizeof(*data->field_font_face) * 1);
    memmove(&data->field_font_bitmap[index_to_erase * 1], &data->field_font_bitmap[start_index * 1], size_to_end * sizeof(*data->field_font_bitmap) * 1);
    memmove(&data->field_texture_id[index_to_erase * 1], &data->field_texture_id[start_index * 1], size_to_end * sizeof(*data->field_texture_id) * 1);

    return true;
  }

  return false;
}


bool
font_exists(const Font_data *data, const uint32_t key, size_t *out_index)
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
font_clear(Font_data *data)
{
  assert(data);

  data->size = 0;
}


bool
font_is_empty(const Font_data *data, const size_t size_hint)
{
  assert(data);

  if(!data) { return false; }

  return !!data->size;
}


size_t
font_get_size(const Font_data *data)
{
  assert(data);

  if(!data) { return 0; }

  return data->size;
}


size_t
font_get_capacity(const Font_data *data)
{
  assert(data);

  if(!data) { return 0; }

  return data->capacity;
}


bool
font_resize_capacity(Font_data *data, const size_t size_hint)
{
  assert(data);
  assert(size_hint > data->size); // Will slice data

  // Create new data.
  Font_data new_data;
  const bool created_new = font_create(&new_data, size_hint);

  // Failed to resize.
  if(!created_new)
  {
    font_destroy(&new_data);
    return false;
  }

  // Copy over data
  {
    memcpy(new_data.keys, data->keys, sizeof(uint32_t) * data->size);
    memcpy(new_data.field_name, data->field_name, sizeof(char) * data->size * 32);
    memcpy(new_data.field_font_face, data->field_font_face, sizeof(stbtt_fontinfo) * data->size * 1);
    memcpy(new_data.field_font_bitmap, data->field_font_bitmap, sizeof(Text::Font_bitmap) * data->size * 1);
    memcpy(new_data.field_texture_id, data->field_texture_id, sizeof(util::generic_id) * data->size * 1);
  }

  // Swap ptrs
  {
    uint32_t *old_keys = data->keys;
    data->keys = new_data.keys;
    new_data.keys = old_keys;

    char *old_name = data->field_name;
    data->field_name = new_data.field_name;
    new_data.field_name = old_name;

    stbtt_fontinfo *old_font_face = data->field_font_face;
    data->field_font_face = new_data.field_font_face;
    new_data.field_font_face = old_font_face;

    Text::Font_bitmap *old_font_bitmap = data->field_font_bitmap;
    data->field_font_bitmap = new_data.field_font_bitmap;
    new_data.field_font_bitmap = old_font_bitmap;

    util::generic_id *old_texture_id = data->field_texture_id;
    data->field_texture_id = new_data.field_texture_id;
    new_data.field_texture_id = old_texture_id;
  }

  // Set the Capacity
  {
    size_t *capacity = const_cast<size_t*>(&data->capacity);
    *capacity = new_data.capacity;
  }

  // Destroy new data
  font_destroy(&new_data);

  return true;
}



// ===================== //
// DATA GETTER FUNCTIONS //
// ===================== //


const char*
font_get_const_name_data(const Font_data *data)
{
  assert(data);
  assert(data->field_name);

  return data->field_name;
}


char*
font_get_name_data(Font_data *data)
{
  assert(data);
  assert(data->field_name);

  return data->field_name;
}


const stbtt_fontinfo*
font_get_const_font_face_data(const Font_data *data)
{
  assert(data);
  assert(data->field_font_face);

  return data->field_font_face;
}


stbtt_fontinfo*
font_get_font_face_data(Font_data *data)
{
  assert(data);
  assert(data->field_font_face);

  return data->field_font_face;
}


const Text::Font_bitmap*
font_get_const_font_bitmap_data(const Font_data *data)
{
  assert(data);
  assert(data->field_font_bitmap);

  return data->field_font_bitmap;
}


Text::Font_bitmap*
font_get_font_bitmap_data(Font_data *data)
{
  assert(data);
  assert(data->field_font_bitmap);

  return data->field_font_bitmap;
}


const util::generic_id*
font_get_const_texture_id_data(const Font_data *data)
{
  assert(data);
  assert(data->field_texture_id);

  return data->field_texture_id;
}


util::generic_id*
font_get_texture_id_data(Font_data *data)
{
  assert(data);
  assert(data->field_texture_id);

  return data->field_texture_id;
}


// =============== //
// FIELD FUNCTIONS //
// =============== //


bool
font_get_name(const Font_data *data, const uint32_t key, const char **return_value)
{
  assert(data);
  assert(key != 0);
  assert(data->field_name);
  assert(return_value);

  // Search for its index.
  // If we find it we can return the value.

  size_t index = 0;

  if(font_exists(data, key, &index))
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
font_set_name(const Font_data *data, const uint32_t key, const char *set_value, const size_t size)
{
  assert(data);
  assert(key != 0);
  assert(data->field_name);
  assert(set_value);

  // Search for its index.
  // If we find it we can set the value.

  size_t index = 0;

  if(font_exists(data, key, &index))
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
font_get_font_face(const Font_data *data, const uint32_t key, stbtt_fontinfo *return_value)
{
  assert(data);
  assert(key != 0);
  assert(data->field_font_face);
  assert(return_value);

  // Search for its index.
  // If we find it we can return the value.

  size_t index = 0;

  if(font_exists(data, key, &index))
  {
    assert(index < data->size);

    if(index < data->size)
    {
      *return_value = data->field_font_face[index];

      return true;
    }
  }

  return false;
}


bool
font_set_font_face(const Font_data *data, const uint32_t key, const stbtt_fontinfo *set_value)
{
  assert(data);
  assert(key != 0);
  assert(data->field_font_face);
  assert(set_value);

  // Search for its index.
  // If we find it we can set the value.

  size_t index = 0;

  if(font_exists(data, key, &index))
  {
    assert(index < data->size);
    if(index < data->size)
    {
      data->field_font_face[index] = *set_value;

      return true;
    }
  }

  return false;
}


bool
font_get_font_bitmap(const Font_data *data, const uint32_t key, Text::Font_bitmap *return_value)
{
  assert(data);
  assert(key != 0);
  assert(data->field_font_bitmap);
  assert(return_value);

  // Search for its index.
  // If we find it we can return the value.

  size_t index = 0;

  if(font_exists(data, key, &index))
  {
    assert(index < data->size);

    if(index < data->size)
    {
      *return_value = data->field_font_bitmap[index];

      return true;
    }
  }

  return false;
}


bool
font_set_font_bitmap(const Font_data *data, const uint32_t key, const Text::Font_bitmap *set_value)
{
  assert(data);
  assert(key != 0);
  assert(data->field_font_bitmap);
  assert(set_value);

  // Search for its index.
  // If we find it we can set the value.

  size_t index = 0;

  if(font_exists(data, key, &index))
  {
    assert(index < data->size);
    if(index < data->size)
    {
      data->field_font_bitmap[index] = *set_value;

      return true;
    }
  }

  return false;
}


bool
font_get_texture_id(const Font_data *data, const uint32_t key, util::generic_id *return_value)
{
  assert(data);
  assert(key != 0);
  assert(data->field_texture_id);
  assert(return_value);

  // Search for its index.
  // If we find it we can return the value.

  size_t index = 0;

  if(font_exists(data, key, &index))
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
font_set_texture_id(const Font_data *data, const uint32_t key, const util::generic_id *set_value)
{
  assert(data);
  assert(key != 0);
  assert(data->field_texture_id);
  assert(set_value);

  // Search for its index.
  // If we find it we can set the value.

  size_t index = 0;

  if(font_exists(data, key, &index))
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


} // Data ns
