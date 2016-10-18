
/*
  Warning
  --
  This file is auto generated changes here may be overwritten.
*/


#include "font_glyph_data.hpp"
#include <assert.h>
#include <cstring>


namespace Data {


// ====================== //
// COMMON MODEL FUNCTIONS //
// ====================== //



void
data_lock(const Font_glyph_data *data)
{
  // Not yet impl.
}


void
data_unlock(const Font_glyph_data *data)
{
  // Not yet impl.
}


bool
font_glyph_create(Font_glyph_data *data, const size_t size_hint)
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

    // Alloc space for character
    if(all_alloc)
    {
      data->field_character = new Text::Character[size_hint * 1];
      assert(data->field_character);
      if(!data->field_character) { all_alloc = false; }
      else { memset(data->field_character, 0, sizeof(Text::Character) * size_hint * 1); }
    }
  }

  // Failed so clean up.
  if(!all_alloc)
  {
    font_glyph_destroy(data);
  }

  return all_alloc;
}


void
font_glyph_destroy(Font_glyph_data *data)
{
  // Free up the memory.
  {
    // Remove keys
    if(data->keys) { delete[] data->keys; }
    data->keys = nullptr;

    // Remove character
    if(data->field_character) { delete[] data->field_character; }
    data->field_character = nullptr;
  }

  // Zero capacity and size
  {
    data->size = 0;

    size_t *capacity = const_cast<size_t*>(&data->capacity);
    *capacity = 0;
  }
}


uint32_t
font_glyph_push(Font_glyph_data *data, const uint32_t key)
{
  assert(data);
  assert(data->keys);

  // Do we need to resize?
  if(data->size >= data->capacity)
  {
    font_glyph_resize_capacity(data, data->capacity << 1);
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
font_glyph_insert(Font_glyph_data *data, const uint32_t key, const size_t insert_point)
{
  assert(data);
  assert(data->keys);
  assert(key);

  // Do we need to resize?
  if(data->size >= data->capacity)
  {
    font_glyph_resize_capacity(data, data->capacity << 1);
  }

  // Shuffle memory up
  {
    const size_t insert_index = insert_point + 1;
    const size_t start_index = insert_point;
    const size_t size_to_end = data->size - insert_point;

    // Shuffle the data down
    memmove(&data->keys[insert_point], &data->keys[start_index], size_to_end * sizeof(*data->keys));
    memmove(&data->field_character[insert_point * 1], &data->field_character[start_index * 1], size_to_end * sizeof(*data->field_character) * 1);
  }

  // Insert new data
  {
    data->keys[data->size++] = key;
  }

  return false;
}


bool
font_glyph_remove(Font_glyph_data *data, const uint32_t key)
{
  size_t index_to_erase = 0;

  if(font_glyph_exists(data, key, &index_to_erase))
  {
    assert(index_to_erase < data->size);

    const size_t start_index = index_to_erase + 1;
    const size_t size_to_end = data->size - index_to_erase - 1;

    --(data->size);

    // Shuffle the data down
    memmove(&data->keys[index_to_erase], &data->keys[start_index], size_to_end * sizeof(*data->keys));
    memmove(&data->field_character[index_to_erase * 1], &data->field_character[start_index * 1], size_to_end * sizeof(*data->field_character) * 1);

    return true;
  }

  return false;
}


bool
font_glyph_exists(const Font_glyph_data *data, const uint32_t key, size_t *out_index)
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
font_glyph_clear(Font_glyph_data *data)
{
  assert(data);

  data->size = 0;
}


bool
font_glyph_is_empty(const Font_glyph_data *data, const size_t size_hint)
{
  assert(data);

  if(!data) { return false; }

  return !!data->size;
}


size_t
font_glyph_get_size(const Font_glyph_data *data)
{
  assert(data);

  if(!data) { return 0; }

  return data->size;
}


size_t
font_glyph_get_capacity(const Font_glyph_data *data)
{
  assert(data);

  if(!data) { return 0; }

  return data->capacity;
}


bool
font_glyph_resize_capacity(Font_glyph_data *data, const size_t size_hint)
{
  assert(data);
  assert(size_hint > data->size); // Will slice data

  // Create new data.
  Font_glyph_data new_data;
  const bool created_new = font_glyph_create(&new_data, size_hint);

  // Failed to resize.
  if(!created_new)
  {
    font_glyph_destroy(&new_data);
    return false;
  }

  // Copy over data
  {
    memcpy(new_data.keys, data->keys, sizeof(uint32_t) * data->size);
    memcpy(new_data.field_character, data->field_character, sizeof(Text::Character) * data->size * 1);
  }

  // Swap ptrs
  {
    uint32_t *old_keys = data->keys;
    data->keys = new_data.keys;
    new_data.keys = old_keys;

    Text::Character *old_character = data->field_character;
    data->field_character = new_data.field_character;
    new_data.field_character = old_character;
  }

  // Set the Capacity
  {
    size_t *capacity = const_cast<size_t*>(&data->capacity);
    *capacity = new_data.capacity;
  }

  // Destroy new data
  font_glyph_destroy(&new_data);

  return true;
}



// ===================== //
// DATA GETTER FUNCTIONS //
// ===================== //


const Text::Character*
font_glyph_get_const_character_data(const Font_glyph_data *data)
{
  assert(data);
  assert(data->field_character);

  return data->field_character;
}


Text::Character*
font_glyph_get_character_data(Font_glyph_data *data)
{
  assert(data);
  assert(data->field_character);

  return data->field_character;
}


// =============== //
// FIELD FUNCTIONS //
// =============== //


bool
font_glyph_get_character(const Font_glyph_data *data, const uint32_t key, Text::Character *return_value)
{
  assert(data);
  assert(key != 0);
  assert(data->field_character);
  assert(return_value);

  // Search for its index.
  // If we find it we can return the value.

  size_t index = 0;

  if(font_glyph_exists(data, key, &index))
  {
    assert(index < data->size);

    if(index < data->size)
    {
      *return_value = data->field_character[index];

      return true;
    }
  }

  return false;
}


bool
font_glyph_set_character(const Font_glyph_data *data, const uint32_t key, const Text::Character *set_value)
{
  assert(data);
  assert(key != 0);
  assert(data->field_character);
  assert(set_value);

  // Search for its index.
  // If we find it we can set the value.

  size_t index = 0;

  if(font_glyph_exists(data, key, &index))

  index = index * 1;

  {
    assert(index < data->size);
    if(index < data->size * 1)
    {
      data->field_character[index] = *set_value;

      return true;
    }
  }

  return false;
}


} // Data ns
