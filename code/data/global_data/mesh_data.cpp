
/*
  Warning:
  This file is auto_generated any changes here may be overwritten.
*/

#include <data/global_data/mesh_data.hpp>
#include <data/global_data/memory_data.hpp>
#include <common/error_strings.hpp>
#include <utilities/logging.hpp>
#include <utilities/memory.hpp>
#include <utilities/string_helpers.hpp>
#include <assert.h>
#include <cstring>


namespace Resource_data {


void
mesh_data_init(Mesh_data *data, const size_t size_hint)
{
  // Argument validation.
  assert(data && size_hint);

  // 16 byte alignment buffer, apply to all for safety.
  constexpr size_t simd_buffer = 16;

  // Calculate the various sizes of things.
  const size_t bytes_mesh_id = sizeof(*data->mesh_id) * size_hint + simd_buffer;
  const size_t bytes_property_name = sizeof(*data->property_name) * 32 * size_hint + simd_buffer;
  const size_t bytes_property_mesh = sizeof(*data->property_mesh) * size_hint + simd_buffer;
  const size_t bytes_property_aabb = sizeof(*data->property_aabb) * size_hint + simd_buffer;

  const size_t bytes_to_alloc = bytes_mesh_id + bytes_property_name + bytes_property_mesh + bytes_property_aabb;

  // Allocate some memory.
  util::memory_chunk *data_memory = const_cast<util::memory_chunk*>(&data->memory);
  *data_memory = Memory::request_memory_chunk(bytes_to_alloc, "mesh_data");

  assert(data_memory->bytes_in_chunk == bytes_to_alloc);

  data_lock(data);

  // Init memory
  {
    size_t byte_counter = 0;
    const void *alloc_start = data->memory.chunk_start;

    // Assign mesh_id memory
    {
      void *offset = util::mem_offset(alloc_start, byte_counter);
      void *aligned = util::mem_next_16byte_boundry(offset);

      data->mesh_id = reinterpret_cast<util::generic_id*>(aligned);
      #ifndef NDEBUG
      memset(offset, 0, bytes_mesh_id);
      #endif

      byte_counter += bytes_mesh_id;
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
    // Assign property_aabb memory
    {
      void *offset = util::mem_offset(alloc_start, byte_counter);
      void *aligned = util::mem_next_16byte_boundry(offset);

      data->property_aabb = reinterpret_cast<math::aabb*>(aligned);
      #ifndef NDEBUG
      memset(offset, 0, bytes_property_aabb);
      #endif

      byte_counter += bytes_property_aabb;
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
mesh_data_free(Mesh_data *data)
{
  assert(data);
}


size_t
mesh_data_get_size(const Mesh_data *data)
{
  assert(data);
  return data->size;
}


size_t
mesh_data_get_capacity(const Mesh_data *data)
{
  assert(data);
  return data->capacity;
}


void
data_lock(const Mesh_data *data)
{
  assert(data);
}


void
data_unlock(const Mesh_data *data)
{
  assert(data);
}


util::generic_id
mesh_data_push_back(Mesh_data *data, size_t *out_index)
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

  data->mesh_id[index] = key;

  // Memset the properties
  {
    memset(&data->property_name[index * 32], 0, sizeof(*data->property_name));
    memset(&data->property_mesh[index], 0, sizeof(*data->property_mesh));
    memset(&data->property_aabb[index], 0, sizeof(*data->property_aabb));
  }

  return key;
}


bool
mesh_data_erase(Mesh_data *data, const util::generic_id key)
{
  // Param check
  assert(data && key);

  size_t index_to_erase;

  if(mesh_data_exists(data, key, &index_to_erase))
  {
    assert(index_to_erase < data->size);

    const size_t start_index = index_to_erase + 1;
    const size_t size_to_end = data->size - index_to_erase - 1;

    --(data->size);

    // Shuffle the memory down.
    memmove(&data->mesh_id[index_to_erase], &data->mesh_id[start_index], size_to_end * sizeof(*data->mesh_id));
    memmove(&data->property_name[index_to_erase * 32], &data->property_name[start_index * 32], (size_to_end * 32) * sizeof(*data->property_name));
    memmove(&data->property_mesh[index_to_erase], &data->property_mesh[start_index], size_to_end * sizeof(*data->property_mesh));
    memmove(&data->property_aabb[index_to_erase], &data->property_aabb[start_index], size_to_end * sizeof(*data->property_aabb));
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
mesh_data_exists(const Mesh_data *data, const util::generic_id key, size_t *out_index)
{
  assert(data && key);

  if(data->size == 0)
  {
    return false;
  }

  bool found = false;

  size_t no_index;
  if(!out_index) { out_index = &no_index; }

  found = util::generic_id_search_binary(out_index, key, data->mesh_id, data->size);

  return found;
}


bool
mesh_data_get_property_name(const Mesh_data *data, const util::generic_id key, char const **out_value)
{
  size_t index;

  if(mesh_data_exists(data, key, &index))
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
mesh_data_set_property_name(Mesh_data *data,  const util::generic_id key, const char *value)
{
  assert(data && key);

  size_t index;

  if(mesh_data_exists(data, key, &index))
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
mesh_data_search_property_name(const Mesh_data *data, const char *value, util::generic_id *out_key)
{
  bool found = false;

  for(size_t i = 0; i < data->size; ++i)
  {
    if(!strcmp(value, &data->property_name[i * 32]))
    {
      found = true;

      if(out_key)
      {
        *out_key = data->mesh_id[i];
      }

      break;
    }
  }

  return found;
}


bool
mesh_data_get_property_mesh(const Mesh_data *data, const util::generic_id key, Graphics_api::Mesh *out_value)
{
  size_t index;

  if(mesh_data_exists(data, key, &index))
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
mesh_data_set_property_mesh(Mesh_data *data,  const util::generic_id key, const Graphics_api::Mesh value)
{
  assert(data && key);

  size_t index;

  if(mesh_data_exists(data, key, &index))
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




bool
mesh_data_get_property_aabb(const Mesh_data *data, const util::generic_id key, math::aabb *out_value)
{
  size_t index;

  if(mesh_data_exists(data, key, &index))
  {
    *out_value = data->property_aabb[index];
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
mesh_data_set_property_aabb(Mesh_data *data,  const util::generic_id key, const math::aabb value)
{
  assert(data && key);

  size_t index;

  if(mesh_data_exists(data, key, &index))
  {
    data->property_aabb[index] = value;
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
