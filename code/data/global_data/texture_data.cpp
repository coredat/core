
/*
  Warning:
  This file is auto_generated any changes here may be overwritten.
*/

#include <data/global_data/texture_data.hpp>
#include <data/global_data/memory_data.hpp>
#include <common/error_strings.hpp>
#include <utilities/logging.hpp>
#include <utilities/memory.hpp>
#include <assert.h>


namespace Resource_data {


void
texture_data_init(Texture_data *data, const size_t size_hint)
{
  // Argument validation.
  assert(data && size_hint);

  // 16 byte alignment buffer, apply to all for safety.
  constexpr size_t simd_buffer = 16;

  // Calculate the various sizes of things.
  const size_t bytes_texture_id = sizeof(*data->texture_id) * size_hint + simd_buffer;
  const size_t bytes_property_name = sizeof(*data->property_name) * 32 * size_hint + simd_buffer;
  const size_t bytes_property_texture = sizeof(*data->property_texture) * size_hint + simd_buffer;

  const size_t bytes_to_alloc = bytes_texture_id + bytes_property_name + bytes_property_texture;

  // Allocate some memory.
  util::memory_chunk *data_memory = const_cast<util::memory_chunk*>(&data->memory);
  *data_memory = Memory::request_memory_chunk(bytes_to_alloc, "texture_data");

  assert(data_memory->bytes_in_chunk == bytes_to_alloc);

  data_lock(data);

  // Init memory
  {
    size_t byte_counter = 0;
    const void *alloc_start = data->memory.chunk_start;

    // Assign texture_id memory
    {
      void *offset = util::mem_offset(alloc_start, byte_counter);
      void *aligned = util::mem_next_16byte_boundry(offset);

      data->texture_id = reinterpret_cast<util::generic_id*>(aligned);
      #ifndef NDEBUG
      memset(offset, 0, bytes_texture_id);
      #endif

      byte_counter += bytes_texture_id;
      assert(byte_counter <= bytes_to_alloc);
    }
    // Assign property_name memory
    {
      void *offset = util::mem_offset(alloc_start, byte_counter);
      void *aligned = util::mem_next_16byte_boundry(offset);

      data->property_name = reinterpret_cast<char*>(aligned);
      #ifndef NDEBUG
      memset(offset, 0, bytes_property_name);
      #endif

      byte_counter += bytes_property_name;
      assert(byte_counter <= bytes_to_alloc);
    }
    // Assign property_texture memory
    {
      void *offset = util::mem_offset(alloc_start, byte_counter);
      void *aligned = util::mem_next_16byte_boundry(offset);

      data->property_texture = reinterpret_cast<Ogl::Texture*>(aligned);
      #ifndef NDEBUG
      memset(offset, 0, bytes_property_texture);
      #endif

      byte_counter += bytes_property_texture;
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
texture_data_free(Texture_data *data)
{
  assert(data);
}


size_t
texture_data_get_size(const Texture_data *data)
{
  assert(data);
  return data->size;
}


size_t
texture_data_get_capacity(const Texture_data *data)
{
  assert(data);
  return data->capacity;
}


void
data_lock(Texture_data *data)
{
  assert(data);
}


void
data_unlock(Texture_data *data)
{
  assert(data);
}


bool
texture_data_push_back(Texture_data *data, const util::generic_id key, size_t *out_index)
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

  data->texture_id[index] = key;

  // Memset the properties
  {
    memset(&data->property_name[index * 32], 0, sizeof(*data->property_name));
    memset(&data->property_texture[index], 0, sizeof(*data->property_texture));
  }

  return true;
}


bool
texture_data_erase(Texture_data *data, const util::generic_id key)
{
  // Param check
  assert(data && key);

  size_t index_to_erase;

  if(texture_data_exists(data, key, &index_to_erase))
  {
    assert(index_to_erase < data->size);

    const size_t start_index = index_to_erase + 1;
    const size_t size_to_end = data->size - index_to_erase - 1;

    --(data->size);

    // Shuffle the memory down.
    memmove(&data->texture_id[index_to_erase], &data->texture_id[start_index], size_to_end * sizeof(*data->texture_id));
    memmove(&data->property_name[index_to_erase * 32], &data->property_name[start_index * 32], (size_to_end * 32) * sizeof(*data->property_name));
    memmove(&data->property_texture[index_to_erase], &data->property_texture[start_index], size_to_end * sizeof(*data->property_texture));
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
texture_data_exists(const Texture_data *data, const util::generic_id key, size_t *out_index)
{
  assert(data && key);

  if(data->size == 0)
  {
    return false;
  }

  bool found = false;

  size_t no_index;
  if(!out_index) { out_index = &no_index; }

  found = util::generic_id_search_binary(out_index, key, data->texture_id, data->size);

  return found;
}


bool
texture_data_get_property_name(const Texture_data *data, const util::generic_id key, char const **out_value)
{
  size_t index;

  if(texture_data_exists(data, key, &index))
  {
    *out_value = &data->property_name[index * 32];
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
texture_data_set_property_name(Texture_data *data,  const util::generic_id key, const char *value)
{
  assert(data && key);

  size_t index;

  if(texture_data_exists(data, key, &index))
  {
    strlcpy(&data->property_name[index * 32], value, 32);
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
texture_data_search_property_name(const Texture_data *data, const char *value, util::generic_id *out_key)
{
  bool found = false;

  for(size_t i = 0; i < data->size; ++i)
  {
    if(!strcmp(value, &data->property_name[i * 32]))
    {
      found = true;

      if(out_key)
      {
        *out_key = data->texture_id[i];
      }

      break;
    }
  }

  return found;
}


bool
texture_data_get_property_texture(const Texture_data *data, const util::generic_id key, Ogl::Texture *out_value)
{
  size_t index;

  if(texture_data_exists(data, key, &index))
  {
    *out_value = data->property_texture[index];
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
texture_data_set_property_texture(Texture_data *data,  const util::generic_id key, const Ogl::Texture value)
{
  assert(data && key);

  size_t index;

  if(texture_data_exists(data, key, &index))
  {
    data->property_texture[index] = value;
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
