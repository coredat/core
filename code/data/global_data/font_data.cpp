

/*
  WARNING
  -------
  This file is auto generated any changes here may be overwritten.
  See code_gen.rake in scripts folder.

  This file was last generated on: Mon 08 Aug 2016
*/


#include <data/global_data/font_data.hpp>
#include <data/global_data/memory_data.hpp>
#include <common/error_strings.hpp>
#include <utilities/logging.hpp>
#include <utilities/memory.hpp>
#include <utilities/string_helpers.hpp>
#include <assert.h>
#include <cstring>


namespace Resource_data {


void
font_data_init(Font_data *data, const size_t size_hint)
{
  // Argument validation.
  assert(data && size_hint);

  // 16 byte alignment buffer, apply to all for safety.
  constexpr size_t simd_buffer = 16;

  // Calculate the various sizes of things.
  const size_t bytes_font_id = sizeof(*data->font_id) * size_hint + simd_buffer;
  const size_t bytes_property_font_name = sizeof(*data->property_font_name) * 32 * size_hint + simd_buffer;
  const size_t bytes_property_font_face = sizeof(*data->property_font_face) * size_hint + simd_buffer;
  const size_t bytes_property_font_bitmap = sizeof(*data->property_font_bitmap) * size_hint + simd_buffer;
  const size_t bytes_property_texture_id = sizeof(*data->property_texture_id) * size_hint + simd_buffer;

  const size_t bytes_to_alloc = bytes_font_id + bytes_property_font_name + bytes_property_font_face + bytes_property_font_bitmap + bytes_property_texture_id;

  // Allocate some memory.
  util::memory_chunk *data_memory = const_cast<util::memory_chunk*>(&data->memory);
  *data_memory = Memory::request_memory_chunk(bytes_to_alloc, "font_data");

  assert(data_memory->bytes_in_chunk == bytes_to_alloc);

  data_lock(data);

  // Init memory
  {
    size_t byte_counter = 0;
    const void *alloc_start = data->memory.chunk_start;

    // Assign font_id memory
    {
      void *offset = util::mem_offset(alloc_start, byte_counter);
      void *aligned = util::mem_next_16byte_boundry(offset);

      data->font_id = reinterpret_cast<util::generic_id*>(aligned);
      #ifndef NDEBUG
      memset(offset, 0, bytes_font_id);
      #endif

      byte_counter += bytes_font_id;
      assert(byte_counter <= bytes_to_alloc);
    }
    // Assign property_font_name memory
    {
      void *offset = util::mem_offset(alloc_start, byte_counter);
      void *aligned = util::mem_next_16byte_boundry(offset);

      data->property_font_name = reinterpret_cast<char*>(aligned);
      #ifndef NDEBUG
      memset(offset, 0, bytes_property_font_name);
      #endif

      byte_counter += bytes_property_font_name;
      assert(byte_counter <= bytes_to_alloc);
    }
    // Assign property_font_face memory
    {
      void *offset = util::mem_offset(alloc_start, byte_counter);
      void *aligned = util::mem_next_16byte_boundry(offset);

      data->property_font_face = reinterpret_cast<stbtt_fontinfo*>(aligned);
      #ifndef NDEBUG
      memset(offset, 0, bytes_property_font_face);
      #endif

      byte_counter += bytes_property_font_face;
      assert(byte_counter <= bytes_to_alloc);
    }
    // Assign property_font_bitmap memory
    {
      void *offset = util::mem_offset(alloc_start, byte_counter);
      void *aligned = util::mem_next_16byte_boundry(offset);

      data->property_font_bitmap = reinterpret_cast<Text::Font_bitmap*>(aligned);
      #ifndef NDEBUG
      memset(offset, 0, bytes_property_font_bitmap);
      #endif

      byte_counter += bytes_property_font_bitmap;
      assert(byte_counter <= bytes_to_alloc);
    }
    // Assign property_texture_id memory
    {
      void *offset = util::mem_offset(alloc_start, byte_counter);
      void *aligned = util::mem_next_16byte_boundry(offset);

      data->property_texture_id = reinterpret_cast<util::generic_id*>(aligned);
      #ifndef NDEBUG
      memset(offset, 0, bytes_property_texture_id);
      #endif

      byte_counter += bytes_property_texture_id;
      assert(byte_counter <= bytes_to_alloc);
    }
  }

  // Set the size and capacity
  {
    data->size = 0;

    size_t *capacity = const_cast<size_t*>(&data->capacity);
    *capacity = size_hint;
  }

  data_unlock(data);
}


void
font_data_free(Font_data *data)
{
  assert(data);
}


size_t
font_data_get_size(const Font_data *data)
{
  assert(data);
  return data->size;
}


size_t
font_data_get_capacity(const Font_data *data)
{
  assert(data);
  return data->capacity;
}


void
data_lock(const Font_data *data)
{
  assert(data);
}


void
data_unlock(const Font_data *data)
{
  assert(data);
}


bool
font_data_push_back(Font_data *data, const util::generic_id key, size_t *out_index)
{
  assert(data && key);
  assert(data->size < data->capacity);

  // TODO: Duplicate key check

  if(data->size >= data->capacity)
  {
    LOG_ERROR(Error_string::no_free_space());

    return false;
  }

  const uint32_t index = data->size;

  if(out_index)
  {
    *out_index = index;
  }

  ++(data->size);

  data->font_id[index] = key;

  // Memset the properties
  {
    memset(&data->property_font_name[index * 32], 0, sizeof(*data->property_font_name));
    memset(&data->property_font_face[index], 0, sizeof(*data->property_font_face));
    memset(&data->property_font_bitmap[index], 0, sizeof(*data->property_font_bitmap));
    memset(&data->property_texture_id[index], 0, sizeof(*data->property_texture_id));
  }

  return true;
}


bool
font_data_erase(Font_data *data, const util::generic_id key)
{
  // Param check
  assert(data && key);

  size_t index_to_erase;

  if(font_data_exists(data, key, &index_to_erase))
  {
    assert(index_to_erase < data->size);

    const size_t start_index = index_to_erase + 1;
    const size_t size_to_end = data->size - index_to_erase - 1;

    --(data->size);

    // Shuffle the memory down.
    memmove(&data->font_id[index_to_erase], &data->font_id[start_index], size_to_end * sizeof(*data->font_id));
    memmove(&data->property_font_name[index_to_erase * 32], &data->property_font_name[start_index * 32], (size_to_end * 32) * sizeof(*data->property_font_name));
    memmove(&data->property_font_face[index_to_erase], &data->property_font_face[start_index], size_to_end * sizeof(*data->property_font_face));
    memmove(&data->property_font_bitmap[index_to_erase], &data->property_font_bitmap[start_index], size_to_end * sizeof(*data->property_font_bitmap));
    memmove(&data->property_texture_id[index_to_erase], &data->property_texture_id[start_index], size_to_end * sizeof(*data->property_texture_id));
  }
  else
  {
    LOG_ERROR(Error_string::entity_not_found());
    assert(false);

    return false;
  }

  return true;
}


bool
font_data_exists(const Font_data *data, const util::generic_id key, size_t *out_index)
{
  assert(data && key);

  if(data->size == 0)
  {
    return false;
  }

  bool found = false;

  size_t no_index;
  if(!out_index) { out_index = &no_index; }

  found = util::generic_id_search_binary(out_index, key, data->font_id, data->size);

  return found;
}


bool
font_data_get_property_font_name(const Font_data *data, const util::generic_id key, char const **out_value)
{
  size_t index;

  if(font_data_exists(data, key, &index))
  {
    *out_value = &data->property_font_name[index * 32];
  }
  else
  {
    LOG_ERROR(Error_string::entity_not_found());
    assert(false);

    return false;
  }

  return true;
}


bool
font_data_set_property_font_name(Font_data *data,  const util::generic_id key, const char *value)
{
  assert(data && key);

  size_t index;

  if(font_data_exists(data, key, &index))
  {
    strlcpy(&data->property_font_name[index * 32], value, 32);
  }
  else
  {
    LOG_ERROR(Error_string::entity_not_found());
    assert(false);

    return false;
  }

  return true;
}


bool
font_data_search_property_font_name(const Font_data *data, const char *value, util::generic_id *out_key)
{
  bool found = false;

  for(size_t i = 0; i < data->size; ++i)
  {
    if(!strcmp(value, &data->property_font_name[i * 32]))
    {
      found = true;

      if(out_key)
      {
        *out_key = data->font_id[i];
      }

      break;
    }
  }

  return found;
}


bool
font_data_get_property_font_face(const Font_data *data, const util::generic_id key, stbtt_fontinfo *out_value)
{
  size_t index;

  if(font_data_exists(data, key, &index))
  {
    *out_value = data->property_font_face[index];
  }
  else
  {
    LOG_ERROR(Error_string::entity_not_found());
    assert(false);

    return false;
  }

  return true;
}


bool
font_data_set_property_font_face(Font_data *data,  const util::generic_id key, const stbtt_fontinfo value)
{
  assert(data && key);

  size_t index;

  if(font_data_exists(data, key, &index))
  {
    data->property_font_face[index] = value;
  }
  else
  {
    LOG_ERROR(Error_string::entity_not_found());
    assert(false);

    return false;
  }

  return true;
}


bool
font_data_get_property_font_bitmap(const Font_data *data, const util::generic_id key, Text::Font_bitmap *out_value)
{
  size_t index;

  if(font_data_exists(data, key, &index))
  {
    *out_value = data->property_font_bitmap[index];
  }
  else
  {
    LOG_ERROR(Error_string::entity_not_found());
    assert(false);

    return false;
  }

  return true;
}


bool
font_data_set_property_font_bitmap(Font_data *data,  const util::generic_id key, const Text::Font_bitmap value)
{
  assert(data && key);

  size_t index;

  if(font_data_exists(data, key, &index))
  {
    data->property_font_bitmap[index] = value;
  }
  else
  {
    LOG_ERROR(Error_string::entity_not_found());
    assert(false);

    return false;
  }

  return true;
}


bool
font_data_get_property_texture_id(const Font_data *data, const util::generic_id key, util::generic_id *out_value)
{
  size_t index;

  if(font_data_exists(data, key, &index))
  {
    *out_value = data->property_texture_id[index];
  }
  else
  {
    LOG_ERROR(Error_string::entity_not_found());
    assert(false);

    return false;
  }

  return true;
}


bool
font_data_set_property_texture_id(Font_data *data,  const util::generic_id key, const util::generic_id value)
{
  assert(data && key);

  size_t index;

  if(font_data_exists(data, key, &index))
  {
    data->property_texture_id[index] = value;
  }
  else
  {
    LOG_ERROR(Error_string::entity_not_found());
    assert(false);

    return false;
  }

  return true;
}


} // ns
