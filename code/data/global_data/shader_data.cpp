

/*
  WARNING
  -------
  This file is auto generated any changes here may be overwritten.
  See code_gen.rake in scripts folder.

  This file was last generated on: Sun 09 Oct 2016
*/


#include <data/global_data/shader_data.hpp>
#include <data/global_data/memory_data.hpp>
#include <common/error_strings.hpp>
#include <utilities/logging.hpp>
#include <utilities/memory.hpp>
#include <utilities/string_helpers.hpp>
#include <assert.h>
#include <cstring>


namespace Resource_data {


void
shader_data_init(Shader_data *data, const size_t size_hint)
{
  // Argument validation.
  assert(data && size_hint);

  // 16 byte alignment buffer, apply to all for safety.
  constexpr size_t simd_buffer = 16;

  // Calculate the various sizes of things.
  const size_t bytes_shader_id = sizeof(*data->shader_id) * size_hint + simd_buffer;
  const size_t bytes_property_name = sizeof(*data->property_name) * 32 * size_hint + simd_buffer;
  const size_t bytes_property_shader = sizeof(*data->property_shader) * size_hint + simd_buffer;

  const size_t bytes_to_alloc = bytes_shader_id + bytes_property_name + bytes_property_shader;

  // Allocate some memory.
  util::memory_chunk *data_memory = const_cast<util::memory_chunk*>(&data->memory);
  *data_memory = Memory::request_memory_chunk(bytes_to_alloc, "shader_data");

  assert(data_memory->bytes_in_chunk == bytes_to_alloc);

  data_lock(data);

  // Init memory
  {
    size_t byte_counter = 0;
    const void *alloc_start = data->memory.chunk_start;

    // Assign shader_id memory
    {
      void *offset = util::mem_offset(alloc_start, byte_counter);
      void *aligned = util::mem_next_16byte_boundry(offset);

      data->shader_id = reinterpret_cast<util::generic_id*>(aligned);
      #ifndef NDEBUG
      memset(offset, 0, bytes_shader_id);
      #endif

      byte_counter += bytes_shader_id;
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
    // Assign property_shader memory
    {
      void *offset = util::mem_offset(alloc_start, byte_counter);
      void *aligned = util::mem_next_16byte_boundry(offset);

      data->property_shader = reinterpret_cast<Ogl::Shader*>(aligned);
      #ifndef NDEBUG
      memset(offset, 0, bytes_property_shader);
      #endif

      byte_counter += bytes_property_shader;
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
shader_data_free(Shader_data *data)
{
  assert(data);
}


size_t
shader_data_get_size(const Shader_data *data)
{
  assert(data);
  return data->size;
}


size_t
shader_data_get_capacity(const Shader_data *data)
{
  assert(data);
  return data->capacity;
}


void
data_lock(const Shader_data *data)
{
  assert(data);
}


void
data_unlock(const Shader_data *data)
{
  assert(data);
}


util::generic_id
shader_data_push_back(Shader_data *data, size_t *out_index)
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

  data->shader_id[index] = key;

  // Memset the properties
  {
    memset(&data->property_name[index * 32], 0, sizeof(*data->property_name));
    memset(&data->property_shader[index], 0, sizeof(*data->property_shader));
  }

  return key;
}


bool
shader_data_erase(Shader_data *data, const util::generic_id key)
{
  // Param check
  assert(data && key);

  size_t index_to_erase;

  if(shader_data_exists(data, key, &index_to_erase))
  {
    assert(index_to_erase < data->size);

    const size_t start_index = index_to_erase + 1;
    const size_t size_to_end = data->size - index_to_erase - 1;

    --(data->size);

    // Shuffle the memory down.
    memmove(&data->shader_id[index_to_erase], &data->shader_id[start_index], size_to_end * sizeof(*data->shader_id));
    memmove(&data->property_name[index_to_erase * 32], &data->property_name[start_index * 32], (size_to_end * 32) * sizeof(*data->property_name));
    memmove(&data->property_shader[index_to_erase], &data->property_shader[start_index], size_to_end * sizeof(*data->property_shader));
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
shader_data_exists(const Shader_data *data, const util::generic_id key, size_t *out_index)
{
  assert(data && key);

  if(data->size == 0)
  {
    return false;
  }

  bool found = false;

  size_t no_index;
  if(!out_index) { out_index = &no_index; }

  found = util::generic_id_search_binary(out_index, key, data->shader_id, data->size);

  return found;
}


bool
shader_data_get_property_name(const Shader_data *data, const util::generic_id key, char const **out_value)
{
  size_t index;

  if(shader_data_exists(data, key, &index))
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
shader_data_set_property_name(Shader_data *data,  const util::generic_id key, const char *value)
{
  assert(data && key);

  size_t index;

  if(shader_data_exists(data, key, &index))
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
shader_data_search_property_name(const Shader_data *data, const char *value, util::generic_id *out_key)
{
  bool found = false;

  for(size_t i = 0; i < data->size; ++i)
  {
    if(!strcmp(value, &data->property_name[i * 32]))
    {
      found = true;

      if(out_key)
      {
        *out_key = data->shader_id[i];
      }

      break;
    }
  }

  return found;
}


bool
shader_data_get_property_shader(const Shader_data *data, const util::generic_id key, Ogl::Shader *out_value)
{
  size_t index;

  if(shader_data_exists(data, key, &index))
  {
    *out_value = data->property_shader[index];
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
shader_data_set_property_shader(Shader_data *data,  const util::generic_id key, const Ogl::Shader value)
{
  assert(data && key);

  size_t index;

  if(shader_data_exists(data, key, &index))
  {
    data->property_shader[index] = value;
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
