
/*
  Warning:
  This file is auto_generated any changes here may be overwritten.
*/

#include <data/world_data/renderer_text_draw_calls_data.hpp>
#include <data/global_data/memory_data.hpp>
#include <common/error_strings.hpp>
#include <utilities/logging.hpp>
#include <utilities/memory.hpp>
#include <utilities/string_helpers.hpp>
#include <assert.h>
#include <cstring>


namespace World_data {


void
renderer_text_draw_calls_data_init(Renderer_text_draw_calls_data *data, const size_t size_hint)
{
  // Argument validation.
  assert(data && size_hint);

  // 16 byte alignment buffer, apply to all for safety.
  constexpr size_t simd_buffer = 16;

  // Calculate the various sizes of things.
  const size_t bytes_renderer_text_draw_calls_id = sizeof(*data->renderer_text_draw_calls_id) * size_hint + simd_buffer;
  const size_t bytes_property_text = sizeof(*data->property_text) * 32 * size_hint + simd_buffer;
  const size_t bytes_property_model_id = sizeof(*data->property_model_id) * size_hint + simd_buffer;

  const size_t bytes_to_alloc = bytes_renderer_text_draw_calls_id + bytes_property_text + bytes_property_model_id;

  // Allocate some memory.
  util::memory_chunk *data_memory = const_cast<util::memory_chunk*>(&data->memory);
  *data_memory = Memory::request_memory_chunk(bytes_to_alloc, "renderer_text_draw_calls_data");

  assert(data_memory->bytes_in_chunk == bytes_to_alloc);

  data_lock(data);

  // Init memory
  {
    size_t byte_counter = 0;
    const void *alloc_start = data->memory.chunk_start;

    // Assign renderer_text_draw_calls_id memory
    {
      void *offset = util::mem_offset(alloc_start, byte_counter);
      void *aligned = util::mem_next_16byte_boundry(offset);

      data->renderer_text_draw_calls_id = reinterpret_cast<util::generic_id*>(aligned);
      #ifndef NDEBUG
      memset(offset, 0, bytes_renderer_text_draw_calls_id);
      #endif

      byte_counter += bytes_renderer_text_draw_calls_id;
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
    // Assign property_model_id memory
    {
      void *offset = util::mem_offset(alloc_start, byte_counter);
      void *aligned = util::mem_next_16byte_boundry(offset);

      data->property_model_id = reinterpret_cast<util::generic_id*>(aligned);
      #ifndef NDEBUG
      memset(offset, 0, bytes_property_model_id);
      #endif

      byte_counter += bytes_property_model_id;
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
renderer_text_draw_calls_data_free(Renderer_text_draw_calls_data *data)
{
  assert(data);
}


size_t
renderer_text_draw_calls_data_get_size(const Renderer_text_draw_calls_data *data)
{
  assert(data);
  return data->size;
}


size_t
renderer_text_draw_calls_data_get_capacity(const Renderer_text_draw_calls_data *data)
{
  assert(data);
  return data->capacity;
}


void
data_lock(const Renderer_text_draw_calls_data *data)
{
  assert(data);
}


void
data_unlock(const Renderer_text_draw_calls_data *data)
{
  assert(data);
}


bool
renderer_text_draw_calls_data_push_back(Renderer_text_draw_calls_data *data, const util::generic_id key, size_t *out_index)
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

  data->renderer_text_draw_calls_id[index] = key;

  // Memset the properties
  {
    memset(&data->property_text[index * 32], 0, sizeof(*data->property_text));
    memset(&data->property_model_id[index], 0, sizeof(*data->property_model_id));
  }

  return true;
}


bool
renderer_text_draw_calls_data_erase(Renderer_text_draw_calls_data *data, const util::generic_id key)
{
  // Param check
  assert(data && key);

  size_t index_to_erase;

  if(renderer_text_draw_calls_data_exists(data, key, &index_to_erase))
  {
    assert(index_to_erase < data->size);

    const size_t start_index = index_to_erase + 1;
    const size_t size_to_end = data->size - index_to_erase - 1;

    --(data->size);

    // Shuffle the memory down.
    memmove(&data->renderer_text_draw_calls_id[index_to_erase], &data->renderer_text_draw_calls_id[start_index], size_to_end * sizeof(*data->renderer_text_draw_calls_id));
    memmove(&data->property_text[index_to_erase * 32], &data->property_text[start_index * 32], (size_to_end * 32) * sizeof(*data->property_text));
    memmove(&data->property_model_id[index_to_erase], &data->property_model_id[start_index], size_to_end * sizeof(*data->property_model_id));
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
renderer_text_draw_calls_data_insert(Renderer_text_draw_calls_data *data, const util::generic_id key, const size_t insert_index)
{
  assert(data && key);

  // If we are past the end of the size
  // Use push back.
  if(insert_index > data->size)
  {
    return renderer_text_draw_calls_data_push_back(data, key);
  }

  // Check that we have capacity
  if(data->size >= data->capacity)
  {
    LOG_ERROR(Error_string::no_free_space())
    return false;
  }

  // Check we are inserting in bounds, then insert
  if(insert_index < data->capacity)
  {
    const size_t dest_index = insert_index + 1;
    const size_t size_to_end = data->size - insert_index;

    ++(data->size);

    // Shuffle the memory up.
    memmove(&data->renderer_text_draw_calls_id[dest_index], &data->renderer_text_draw_calls_id[insert_index], size_to_end * sizeof(*data->renderer_text_draw_calls_id));
    memmove(&data->property_text[dest_index * 32], &data->property_text[insert_index * 32], (size_to_end * 32) * sizeof(*data->property_text));
    memmove(&data->property_model_id[dest_index], &data->property_model_id[insert_index], size_to_end * sizeof(*data->property_model_id));

    // Add key to new entry.
    data->renderer_text_draw_calls_id[insert_index] = key;

    return true;
  }

  return false;
}


bool
renderer_text_draw_calls_data_exists(const Renderer_text_draw_calls_data *data, const util::generic_id key, size_t *out_index)
{
  assert(data && key);

  if(data->size == 0)
  {
    return false;
  }

  bool found = false;

  size_t no_index;
  if(!out_index) { out_index = &no_index; }

  found = util::generic_id_search_linear(out_index, key, data->renderer_text_draw_calls_id, data->size);

  return found;
}


bool
renderer_text_draw_calls_data_get_property_text(const Renderer_text_draw_calls_data *data, const util::generic_id key, char const **out_value)
{
  size_t index;

  if(renderer_text_draw_calls_data_exists(data, key, &index))
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
renderer_text_draw_calls_data_set_property_text(Renderer_text_draw_calls_data *data,  const util::generic_id key, const char *value)
{
  assert(data && key);

  size_t index;

  if(renderer_text_draw_calls_data_exists(data, key, &index))
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
renderer_text_draw_calls_data_get_property_model_id(const Renderer_text_draw_calls_data *data, const util::generic_id key, util::generic_id *out_value)
{
  size_t index;

  if(renderer_text_draw_calls_data_exists(data, key, &index))
  {
    *out_value = data->property_model_id[index];
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
renderer_text_draw_calls_data_set_property_model_id(Renderer_text_draw_calls_data *data,  const util::generic_id key, const util::generic_id value)
{
  assert(data && key);

  size_t index;

  if(renderer_text_draw_calls_data_exists(data, key, &index))
  {
    data->property_model_id[index] = value;
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
