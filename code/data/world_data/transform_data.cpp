
/*
  Warning:
  This file is auto_generated any changes here may be overwritten.
*/

#include <data/world_data/transform_data.hpp>
#include <data/global_data/memory_data.hpp>
#include <common/error_strings.hpp>
#include <utilities/logging.hpp>
#include <utilities/memory.hpp>
#include <assert.h>


namespace World_data {


void
transform_data_init(Transform_data *data, const size_t size_hint)
{
  // Argument validation.
  assert(data && size_hint);

  // 16 byte alignment buffer, apply to all for safety.
  constexpr size_t simd_buffer = 16;

  // Calculate the various sizes of things.
  const size_t bytes_data_key = sizeof(*data->data_key) * size_hint + simd_buffer;
  const size_t bytes_property_transform = sizeof(*data->property_transform) * size_hint + simd_buffer;
  const size_t bytes_property_aabb = sizeof(*data->property_aabb) * size_hint + simd_buffer;

  const size_t bytes_to_alloc = bytes_data_key + bytes_property_transform + bytes_property_aabb;

  // Allocate some memory.
  util::memory_chunk *data_memory = const_cast<util::memory_chunk*>(&data->memory);
  *data_memory = Memory::request_memory_chunk(bytes_to_alloc, "transform_data");

  assert(data_memory->bytes_in_chunk == bytes_to_alloc);

  data_lock(data);

  // Init memory
  {
    size_t byte_counter = 0;
    const void *alloc_start = data->memory.chunk_start;

    // Assign data_key memory
    {
      void *offset = util::mem_offset(alloc_start, byte_counter);
      void *aligned = util::mem_next_16byte_boundry(offset);

      data->data_key = reinterpret_cast<util::generic_id*>(aligned);
      #ifndef NDEBUG
      memset(offset, 0, bytes_data_key);
      #endif

      byte_counter += bytes_data_key;
      assert(byte_counter <= bytes_to_alloc);
    }
    // Assign property_transform memory
    {
      void *offset = util::mem_offset(alloc_start, byte_counter);
      void *aligned = util::mem_next_16byte_boundry(offset);

      data->property_transform = reinterpret_cast<math::transform*>(aligned);
      #ifndef NDEBUG
      memset(offset, 0, bytes_property_transform);
      #endif

      byte_counter += bytes_property_transform;
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
transform_data_free(Transform_data *data)
{
  assert(data);
}


size_t
transform_data_get_size(const Transform_data *data)
{
  assert(data);
  return data->size;
}


size_t
transform_data_get_capacity(const Transform_data *data)
{
  assert(data);
  return data->capacity;
}


void
data_lock(Transform_data *data)
{
  assert(data);
}


void
data_unlock(Transform_data *data)
{
  assert(data);
}


bool
transform_data_push_back(Transform_data *data, const util::generic_id key, size_t *out_index)
{
  assert(data && key);
  assert(data->size < data->capacity);

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

  data->data_key[index] = key;

  // Memset the properties
  {
    memset(&data->property_transform[index], 0, sizeof(*data->property_transform));
    memset(&data->property_aabb[index], 0, sizeof(*data->property_aabb));
  }

  return true;
}


bool
transform_data_erase(Transform_data *data, const util::generic_id key)
{
  // Param check
  assert(data && key);

  size_t index_to_erase;

  if(transform_data_exists(data, key, &index_to_erase))
  {
    assert(index_to_erase < data->size);

    const size_t start_index = index_to_erase + 1;
    const size_t size_to_end = data->size - index_to_erase - 1;

    --(data->size);

    // Shuffle the memory down.
    memmove(&data->data_key[index_to_erase], &data->data_key[start_index], size_to_end * sizeof(*data->data_key));
    memmove(&data->property_transform[index_to_erase], &data->property_transform[start_index], size_to_end * sizeof(*data->property_transform));
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
transform_data_exists(const Transform_data *data, const util::generic_id key, size_t *out_index)
{
  assert(data && key);

  if(data->size == 0)
  {
    return false;
  }

  bool found = false;

  size_t no_index;
  if(!out_index) { out_index = &no_index; }

  found = util::generic_id_search_linearly(out_index, key, data->data_key, data->size);

  return found;
}


bool
transform_data_get_property_transform(const Transform_data *data, const util::generic_id key, math::transform *out_value)
{
  size_t index;

  if(transform_data_exists(data, key, &index))
  {
    *out_value = data->property_transform[index];
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
transform_data_set_property_transform(Transform_data *data,  const util::generic_id key, const math::transform value)
{
  assert(data && key);

  size_t index;

  if(transform_data_exists(data, key, &index))
  {
    data->property_transform[index] = value;
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
transform_data_get_property_aabb(const Transform_data *data, const util::generic_id key, math::aabb *out_value)
{
  size_t index;

  if(transform_data_exists(data, key, &index))
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
transform_data_set_property_aabb(Transform_data *data,  const util::generic_id key, const math::aabb value)
{
  assert(data && key);

  size_t index;

  if(transform_data_exists(data, key, &index))
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
