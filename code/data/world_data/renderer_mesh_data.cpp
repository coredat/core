

/*
  WARNING
  -------
  This file is auto generated any changes here may be overwritten.
  See code_gen.rake in scripts folder.

  This file was last generated on: Sun 09 Oct 2016
*/


#include <data/world_data/renderer_mesh_data.hpp>
#include <data/global_data/memory_data.hpp>
#include <common/error_strings.hpp>
#include <utilities/logging.hpp>
#include <utilities/memory.hpp>
#include <utilities/string_helpers.hpp>
#include <assert.h>
#include <cstring>


namespace World_data {


void
renderer_mesh_data_init(Renderer_mesh_data *data, const size_t size_hint)
{
  // Argument validation.
  assert(data && size_hint);

  // 16 byte alignment buffer, apply to all for safety.
  constexpr size_t simd_buffer = 16;

  // Calculate the various sizes of things.
  const size_t bytes_renderer_mesh_id = sizeof(*data->renderer_mesh_id) * size_hint + simd_buffer;
  const size_t bytes_property_material_id = sizeof(*data->property_material_id) * size_hint + simd_buffer;
  const size_t bytes_property_draw_call = sizeof(*data->property_draw_call) * size_hint + simd_buffer;

  const size_t bytes_to_alloc = bytes_renderer_mesh_id + bytes_property_material_id + bytes_property_draw_call;

  // Allocate some memory.
  util::memory_chunk *data_memory = const_cast<util::memory_chunk*>(&data->memory);
  *data_memory = Memory::request_memory_chunk(bytes_to_alloc, "renderer_mesh_data");

  assert(data_memory->bytes_in_chunk == bytes_to_alloc);

  data_lock(data);

  // Init memory
  {
    size_t byte_counter = 0;
    const void *alloc_start = data->memory.chunk_start;

    // Assign renderer_mesh_id memory
    {
      void *offset = util::mem_offset(alloc_start, byte_counter);
      void *aligned = util::mem_next_16byte_boundry(offset);

      data->renderer_mesh_id = reinterpret_cast<util::generic_id*>(aligned);
      #ifndef NDEBUG
      memset(offset, 0, bytes_renderer_mesh_id);
      #endif

      byte_counter += bytes_renderer_mesh_id;
      assert(byte_counter <= bytes_to_alloc);
    }
    // Assign property_material_id memory
    {
      void *offset = util::mem_offset(alloc_start, byte_counter);
      void *aligned = util::mem_next_16byte_boundry(offset);

      data->property_material_id = reinterpret_cast<util::generic_id*>(aligned);
      #ifndef NDEBUG
      memset(offset, 0, bytes_property_material_id);
      #endif

      byte_counter += bytes_property_material_id;
      assert(byte_counter <= bytes_to_alloc);
    }
    // Assign property_draw_call memory
    {
      void *offset = util::mem_offset(alloc_start, byte_counter);
      void *aligned = util::mem_next_16byte_boundry(offset);

      data->property_draw_call = reinterpret_cast<Mesh_renderer_draw_call*>(aligned);
      #ifndef NDEBUG
      memset(offset, 0, bytes_property_draw_call);
      #endif

      byte_counter += bytes_property_draw_call;
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
renderer_mesh_data_free(Renderer_mesh_data *data)
{
  assert(data);
}


size_t
renderer_mesh_data_get_size(const Renderer_mesh_data *data)
{
  assert(data);
  return data->size;
}


size_t
renderer_mesh_data_get_capacity(const Renderer_mesh_data *data)
{
  assert(data);
  return data->capacity;
}


void
data_lock(const Renderer_mesh_data *data)
{
  assert(data);
}


void
data_unlock(const Renderer_mesh_data *data)
{
  assert(data);
}


bool
renderer_mesh_data_push_back(Renderer_mesh_data *data, const util::generic_id key, size_t *out_index)
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

  data->renderer_mesh_id[index] = key;

  // Memset the properties
  {
    memset(&data->property_material_id[index], 0, sizeof(*data->property_material_id));
    memset(&data->property_draw_call[index], 0, sizeof(*data->property_draw_call));
  }

  return true;
}


bool
renderer_mesh_data_erase(Renderer_mesh_data *data, const util::generic_id key)
{
  // Param check
  assert(data && key);

  size_t index_to_erase;

  if(renderer_mesh_data_exists(data, key, &index_to_erase))
  {
    assert(index_to_erase < data->size);

    const size_t start_index = index_to_erase + 1;
    const size_t size_to_end = data->size - index_to_erase - 1;

    --(data->size);

    // Shuffle the memory down.
    memmove(&data->renderer_mesh_id[index_to_erase], &data->renderer_mesh_id[start_index], size_to_end * sizeof(*data->renderer_mesh_id));
    memmove(&data->property_material_id[index_to_erase], &data->property_material_id[start_index], size_to_end * sizeof(*data->property_material_id));
    memmove(&data->property_draw_call[index_to_erase], &data->property_draw_call[start_index], size_to_end * sizeof(*data->property_draw_call));
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
renderer_mesh_data_insert(Renderer_mesh_data *data, const util::generic_id key, const size_t insert_index)
{
  assert(data && key);

  // If we are past the end of the size
  // Use push back.
  if(insert_index > data->size)
  {
    return renderer_mesh_data_push_back(data, key);
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
    memmove(&data->renderer_mesh_id[dest_index], &data->renderer_mesh_id[insert_index], size_to_end * sizeof(*data->renderer_mesh_id));
    memmove(&data->property_material_id[dest_index], &data->property_material_id[insert_index], size_to_end * sizeof(*data->property_material_id));
    memmove(&data->property_draw_call[dest_index], &data->property_draw_call[insert_index], size_to_end * sizeof(*data->property_draw_call));

    // Add key to new entry.
    data->renderer_mesh_id[insert_index] = key;

    return true;
  }

  return false;
}


bool
renderer_mesh_data_exists(const Renderer_mesh_data *data, const util::generic_id key, size_t *out_index)
{
  assert(data && key);

  if(data->size == 0)
  {
    return false;
  }

  bool found = false;

  size_t no_index;
  if(!out_index) { out_index = &no_index; }

  found = util::generic_id_search_linear(out_index, key, data->renderer_mesh_id, data->size);

  return found;
}


bool
renderer_mesh_data_get_property_material_id(const Renderer_mesh_data *data, const util::generic_id key, util::generic_id *out_value)
{
  size_t index;

  if(renderer_mesh_data_exists(data, key, &index))
  {
    *out_value = data->property_material_id[index];
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
renderer_mesh_data_set_property_material_id(Renderer_mesh_data *data,  const util::generic_id key, const util::generic_id value)
{
  assert(data && key);

  size_t index;

  if(renderer_mesh_data_exists(data, key, &index))
  {
    data->property_material_id[index] = value;
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
renderer_mesh_data_get_property_draw_call(const Renderer_mesh_data *data, const util::generic_id key, Mesh_renderer_draw_call **out_value)
{
  size_t index;

  if(renderer_mesh_data_exists(data, key, &index))
  {
    *out_value = &data->property_draw_call[index];
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
renderer_mesh_data_set_property_draw_call(Renderer_mesh_data *data,  const util::generic_id key, const Mesh_renderer_draw_call *value)
{
  assert(data && key);

  size_t index;

  if(renderer_mesh_data_exists(data, key, &index))
  {
    data->property_draw_call[index] = *value;
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
