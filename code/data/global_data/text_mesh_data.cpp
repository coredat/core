

/*
  WARNING
  -------
  This file is auto generated any changes here may be overwritten.
  See code_gen.rake in scripts folder.

  This file was last generated on: Sun 21 Aug 2016
*/


#include <data/global_data/text_mesh_data.hpp>
#include <data/global_data/memory_data.hpp>
#include <common/error_strings.hpp>
#include <utilities/logging.hpp>
#include <utilities/memory.hpp>
#include <utilities/string_helpers.hpp>
#include <assert.h>
#include <cstring>


namespace Resource_data {


void
text_mesh_data_init(Text_mesh_data *data, const size_t size_hint)
{
  // Argument validation.
  assert(data && size_hint);

  // 16 byte alignment buffer, apply to all for safety.
  constexpr size_t simd_buffer = 16;

  // Calculate the various sizes of things.
  const size_t bytes_text_mesh_id = sizeof(*data->text_mesh_id) * size_hint + simd_buffer;
  const size_t bytes_property_text = sizeof(*data->property_text) * 32 * size_hint + simd_buffer;
  const size_t bytes_property_text_size = sizeof(*data->property_text_size) * size_hint + simd_buffer;
  const size_t bytes_property_font_id = sizeof(*data->property_font_id) * size_hint + simd_buffer;
  const size_t bytes_property_mesh = sizeof(*data->property_mesh) * size_hint + simd_buffer;

  const size_t bytes_to_alloc = bytes_text_mesh_id + bytes_property_text + bytes_property_text_size + bytes_property_font_id + bytes_property_mesh;

  // Allocate some memory.
  util::memory_chunk *data_memory = const_cast<util::memory_chunk*>(&data->memory);
  *data_memory = Memory::request_memory_chunk(bytes_to_alloc, "text_mesh_data");

  assert(data_memory->bytes_in_chunk == bytes_to_alloc);

  data_lock(data);

  // Init memory
  {
    size_t byte_counter = 0;
    const void *alloc_start = data->memory.chunk_start;

    // Assign text_mesh_id memory
    {
      void *offset = util::mem_offset(alloc_start, byte_counter);
      void *aligned = util::mem_next_16byte_boundry(offset);

      data->text_mesh_id = reinterpret_cast<util::generic_id*>(aligned);
      #ifndef NDEBUG
      memset(offset, 0, bytes_text_mesh_id);
      #endif

      byte_counter += bytes_text_mesh_id;
      assert(byte_counter <= bytes_to_alloc);
    }
    // Assign property_text memory
    {
      void *offset = util::mem_offset(alloc_start, byte_counter);
      void *aligned = util::mem_next_16byte_boundry(offset);

      data->property_text = reinterpret_cast<char*>(aligned);
      #ifndef NDEBUG
      memset(offset, 0, bytes_property_text);
      #endif

      byte_counter += bytes_property_text;
      assert(byte_counter <= bytes_to_alloc);
    }
    // Assign property_text_size memory
    {
      void *offset = util::mem_offset(alloc_start, byte_counter);
      void *aligned = util::mem_next_16byte_boundry(offset);

      data->property_text_size = reinterpret_cast<uint32_t*>(aligned);
      #ifndef NDEBUG
      memset(offset, 0, bytes_property_text_size);
      #endif

      byte_counter += bytes_property_text_size;
      assert(byte_counter <= bytes_to_alloc);
    }
    // Assign property_font_id memory
    {
      void *offset = util::mem_offset(alloc_start, byte_counter);
      void *aligned = util::mem_next_16byte_boundry(offset);

      data->property_font_id = reinterpret_cast<util::generic_id*>(aligned);
      #ifndef NDEBUG
      memset(offset, 0, bytes_property_font_id);
      #endif

      byte_counter += bytes_property_font_id;
      assert(byte_counter <= bytes_to_alloc);
    }
    // Assign property_mesh memory
    {
      void *offset = util::mem_offset(alloc_start, byte_counter);
      void *aligned = util::mem_next_16byte_boundry(offset);

      data->property_mesh = reinterpret_cast<Graphics_api::Mesh*>(aligned);
      #ifndef NDEBUG
      memset(offset, 0, bytes_property_mesh);
      #endif

      byte_counter += bytes_property_mesh;
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
text_mesh_data_free(Text_mesh_data *data)
{
  assert(data);
}


size_t
text_mesh_data_get_size(const Text_mesh_data *data)
{
  assert(data);
  return data->size;
}


size_t
text_mesh_data_get_capacity(const Text_mesh_data *data)
{
  assert(data);
  return data->capacity;
}


void
data_lock(const Text_mesh_data *data)
{
  assert(data);
}


void
data_unlock(const Text_mesh_data *data)
{
  assert(data);
}


util::generic_id
text_mesh_data_push_back(Text_mesh_data *data, size_t *out_index)
{
  assert(data);
  assert(data->size < data->capacity);

  // TODO: Duplicate key check

  if(data->size >= data->capacity)
  {
    LOG_ERROR(Error_string::no_free_space());

    return 0;
  }

  const uint32_t index = data->size;
  const util::generic_id key = ++data->key_count;

  if(out_index)
  {
    *out_index = index;
  }

  ++(data->size);

  data->text_mesh_id[index] = key;

  // Memset the properties
  {
    memset(&data->property_text[index * 32], 0, sizeof(*data->property_text));
    memset(&data->property_text_size[index], 0, sizeof(*data->property_text_size));
    memset(&data->property_font_id[index], 0, sizeof(*data->property_font_id));
    memset(&data->property_mesh[index], 0, sizeof(*data->property_mesh));
  }

  return key;
}


bool
text_mesh_data_erase(Text_mesh_data *data, const util::generic_id key)
{
  // Param check
  assert(data && key);

  size_t index_to_erase;

  if(text_mesh_data_exists(data, key, &index_to_erase))
  {
    assert(index_to_erase < data->size);

    const size_t start_index = index_to_erase + 1;
    const size_t size_to_end = data->size - index_to_erase - 1;

    --(data->size);

    // Shuffle the memory down.
    memmove(&data->text_mesh_id[index_to_erase], &data->text_mesh_id[start_index], size_to_end * sizeof(*data->text_mesh_id));
    memmove(&data->property_text[index_to_erase * 32], &data->property_text[start_index * 32], (size_to_end * 32) * sizeof(*data->property_text));
    memmove(&data->property_text_size[index_to_erase], &data->property_text_size[start_index], size_to_end * sizeof(*data->property_text_size));
    memmove(&data->property_font_id[index_to_erase], &data->property_font_id[start_index], size_to_end * sizeof(*data->property_font_id));
    memmove(&data->property_mesh[index_to_erase], &data->property_mesh[start_index], size_to_end * sizeof(*data->property_mesh));
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
text_mesh_data_exists(const Text_mesh_data *data, const util::generic_id key, size_t *out_index)
{
  assert(data && key);

  if(data->size == 0)
  {
    return false;
  }

  bool found = false;

  size_t no_index;
  if(!out_index) { out_index = &no_index; }

  found = util::generic_id_search_binary(out_index, key, data->text_mesh_id, data->size);

  return found;
}


bool
text_mesh_data_get_property_text(const Text_mesh_data *data, const util::generic_id key, char const **out_value)
{
  size_t index;

  if(text_mesh_data_exists(data, key, &index))
  {
    *out_value = &data->property_text[index * 32];
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
text_mesh_data_set_property_text(Text_mesh_data *data,  const util::generic_id key, const char *value)
{
  assert(data && key);

  size_t index;

  if(text_mesh_data_exists(data, key, &index))
  {
    strlcpy(&data->property_text[index * 32], value, 32);
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
text_mesh_data_search_property_text(const Text_mesh_data *data, const char *value, util::generic_id *out_key)
{
  bool found = false;

  for(size_t i = 0; i < data->size; ++i)
  {
    if(!strcmp(value, &data->property_text[i * 32]))
    {
      found = true;

      if(out_key)
      {
        *out_key = data->text_mesh_id[i];
      }

      break;
    }
  }

  return found;
}


bool
text_mesh_data_get_property_text_size(const Text_mesh_data *data, const util::generic_id key, uint32_t *out_value)
{
  size_t index;

  if(text_mesh_data_exists(data, key, &index))
  {
    *out_value = data->property_text_size[index];
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
text_mesh_data_set_property_text_size(Text_mesh_data *data,  const util::generic_id key, const uint32_t value)
{
  assert(data && key);

  size_t index;

  if(text_mesh_data_exists(data, key, &index))
  {
    data->property_text_size[index] = value;
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
text_mesh_data_get_property_font_id(const Text_mesh_data *data, const util::generic_id key, util::generic_id *out_value)
{
  size_t index;

  if(text_mesh_data_exists(data, key, &index))
  {
    *out_value = data->property_font_id[index];
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
text_mesh_data_set_property_font_id(Text_mesh_data *data,  const util::generic_id key, const util::generic_id value)
{
  assert(data && key);

  size_t index;

  if(text_mesh_data_exists(data, key, &index))
  {
    data->property_font_id[index] = value;
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
text_mesh_data_get_property_mesh(const Text_mesh_data *data, const util::generic_id key, Graphics_api::Mesh *out_value)
{
  size_t index;

  if(text_mesh_data_exists(data, key, &index))
  {
    *out_value = data->property_mesh[index];
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
text_mesh_data_set_property_mesh(Text_mesh_data *data,  const util::generic_id key, const Graphics_api::Mesh value)
{
  assert(data && key);

  size_t index;

  if(text_mesh_data_exists(data, key, &index))
  {
    data->property_mesh[index] = value;
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
