

/*
  WARNING
  -------
  This file is auto_generated any changes here may be overwritten.
  See code_gen.rake in scripts folder.

  This file was last generated on: Fri 29 Jul 2016
*/


#include <data/global_data/post_process_data.hpp>
#include <data/global_data/memory_data.hpp>
#include <common/error_strings.hpp>
#include <utilities/logging.hpp>
#include <utilities/memory.hpp>
#include <assert.h>
#include <cstring>


namespace Resource_data {


void
post_process_data_init(Post_process_data *data, const size_t size_hint)
{
  // Argument validation.
  assert(data && size_hint);

  // 16 byte alignment buffer, apply to all for safety.
  constexpr size_t simd_buffer = 16;

  // Calculate the various sizes of things.
  const size_t bytes_post_process_id = sizeof(*data->post_process_id) * size_hint + simd_buffer;
  const size_t bytes_property_name = sizeof(*data->property_name) * 32 * size_hint + simd_buffer;
  const size_t bytes_property_post_shader = sizeof(*data->property_post_shader) * size_hint + simd_buffer;

  const size_t bytes_to_alloc = bytes_post_process_id + bytes_property_name + bytes_property_post_shader;

  // Allocate some memory.
  util::memory_chunk *data_memory = const_cast<util::memory_chunk*>(&data->memory);
  *data_memory = Memory::request_memory_chunk(bytes_to_alloc, "post_process_data");

  assert(data_memory->bytes_in_chunk == bytes_to_alloc);

  data_lock(data);

  // Init memory
  {
    size_t byte_counter = 0;
    const void *alloc_start = data->memory.chunk_start;

    // Assign post_process_id memory
    {
      void *offset = util::mem_offset(alloc_start, byte_counter);
      void *aligned = util::mem_next_16byte_boundry(offset);

      data->post_process_id = reinterpret_cast<util::generic_id*>(aligned);
      #ifndef NDEBUG
      memset(offset, 0, bytes_post_process_id);
      #endif

      byte_counter += bytes_post_process_id;
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
    // Assign property_post_shader memory
    {
      void *offset = util::mem_offset(alloc_start, byte_counter);
      void *aligned = util::mem_next_16byte_boundry(offset);

      data->property_post_shader = reinterpret_cast<Post_renderer::Post_shader*>(aligned);
      #ifndef NDEBUG
      memset(offset, 0, bytes_property_post_shader);
      #endif

      byte_counter += bytes_property_post_shader;
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
post_process_data_free(Post_process_data *data)
{
  assert(data);
}


size_t
post_process_data_get_size(const Post_process_data *data)
{
  assert(data);
  return data->size;
}


size_t
post_process_data_get_capacity(const Post_process_data *data)
{
  assert(data);
  return data->capacity;
}


void
data_lock(const Post_process_data *data)
{
  assert(data);
}


void
data_unlock(const Post_process_data *data)
{
  assert(data);
}


bool
post_process_data_push_back(Post_process_data *data, const util::generic_id key, size_t *out_index)
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

  data->post_process_id[index] = key;

  // Memset the properties
  {
    memset(&data->property_name[index * 32], 0, sizeof(*data->property_name));
    memset(&data->property_post_shader[index], 0, sizeof(*data->property_post_shader));
  }

  return true;
}


bool
post_process_data_erase(Post_process_data *data, const util::generic_id key)
{
  // Param check
  assert(data && key);

  size_t index_to_erase;

  if(post_process_data_exists(data, key, &index_to_erase))
  {
    assert(index_to_erase < data->size);

    const size_t start_index = index_to_erase + 1;
    const size_t size_to_end = data->size - index_to_erase - 1;

    --(data->size);

    // Shuffle the memory down.
    memmove(&data->post_process_id[index_to_erase], &data->post_process_id[start_index], size_to_end * sizeof(*data->post_process_id));
    memmove(&data->property_name[index_to_erase * 32], &data->property_name[start_index * 32], (size_to_end * 32) * sizeof(*data->property_name));
    memmove(&data->property_post_shader[index_to_erase], &data->property_post_shader[start_index], size_to_end * sizeof(*data->property_post_shader));
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
post_process_data_exists(const Post_process_data *data, const util::generic_id key, size_t *out_index)
{
  assert(data && key);

  if(data->size == 0)
  {
    return false;
  }

  bool found = false;

  size_t no_index;
  if(!out_index) { out_index = &no_index; }

  found = util::generic_id_search_binary(out_index, key, data->post_process_id, data->size);

  return found;
}


bool
post_process_data_get_property_name(const Post_process_data *data, const util::generic_id key, char const **out_value)
{
  size_t index;

  if(post_process_data_exists(data, key, &index))
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
post_process_data_set_property_name(Post_process_data *data,  const util::generic_id key, const char *value)
{
  assert(data && key);

  size_t index;

  if(post_process_data_exists(data, key, &index))
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
post_process_data_search_property_name(const Post_process_data *data, const char *value, util::generic_id *out_key)
{
  bool found = false;

  for(size_t i = 0; i < data->size; ++i)
  {
    if(!strcmp(value, &data->property_name[i * 32]))
    {
      found = true;

      if(out_key)
      {
        *out_key = data->post_process_id[i];
      }

      break;
    }
  }

  return found;
}


bool
post_process_data_get_property_post_shader(const Post_process_data *data, const util::generic_id key, Post_renderer::Post_shader **out_value)
{
  size_t index;

  if(post_process_data_exists(data, key, &index))
  {
    *out_value = &data->property_post_shader[index];
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
post_process_data_set_property_post_shader(Post_process_data *data,  const util::generic_id key, const Post_renderer::Post_shader *value)
{
  assert(data && key);

  size_t index;

  if(post_process_data_exists(data, key, &index))
  {
    data->property_post_shader[index] = *value;
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
