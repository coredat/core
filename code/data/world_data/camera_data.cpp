

/*
  WARNING
  -------
  This file is auto generated any changes here may be overwritten.
  See code_gen.rake in scripts folder.

  This file was last generated on: Tue 20 Sep 2016
*/


#include <data/world_data/camera_data.hpp>
#include <data/global_data/memory_data.hpp>
#include <common/error_strings.hpp>
#include <utilities/logging.hpp>
#include <utilities/memory.hpp>
#include <utilities/string_helpers.hpp>
#include <assert.h>
#include <cstring>


namespace World_data {


void
camera_data_init(Camera_data *data, const size_t size_hint)
{
  // Argument validation.
  assert(data && size_hint);

  // 16 byte alignment buffer, apply to all for safety.
  constexpr size_t simd_buffer = 16;

  // Calculate the various sizes of things.
  const size_t bytes_camera_id = sizeof(*data->camera_id) * size_hint + simd_buffer;
  const size_t bytes_property_entity_id = sizeof(*data->property_entity_id) * size_hint + simd_buffer;
  const size_t bytes_property_priority = sizeof(*data->property_priority) * size_hint + simd_buffer;
  const size_t bytes_property_texture_id = sizeof(*data->property_texture_id) * size_hint + simd_buffer;
  const size_t bytes_property_post_process_id = sizeof(*data->property_post_process_id) * size_hint + simd_buffer;
  const size_t bytes_property_camera = sizeof(*data->property_camera) * size_hint + simd_buffer;

  const size_t bytes_to_alloc = bytes_camera_id + bytes_property_entity_id + bytes_property_priority + bytes_property_texture_id + bytes_property_post_process_id + bytes_property_camera;

  // Allocate some memory.
  util::memory_chunk *data_memory = const_cast<util::memory_chunk*>(&data->memory);
  *data_memory = Memory::request_memory_chunk(bytes_to_alloc, "camera_data");

  assert(data_memory->bytes_in_chunk == bytes_to_alloc);

  data_lock(data);

  // Init memory
  {
    size_t byte_counter = 0;
    const void *alloc_start = data->memory.chunk_start;

    // Assign camera_id memory
    {
      void *offset = util::mem_offset(alloc_start, byte_counter);
      void *aligned = util::mem_next_16byte_boundry(offset);

      data->camera_id = reinterpret_cast<util::generic_id*>(aligned);
      #ifndef NDEBUG
      memset(offset, 0, bytes_camera_id);
      #endif

      byte_counter += bytes_camera_id;
      assert(byte_counter <= bytes_to_alloc);
    }
    // Assign property_entity_id memory
    {
      void *offset = util::mem_offset(alloc_start, byte_counter);
      void *aligned = util::mem_next_16byte_boundry(offset);

      data->property_entity_id = reinterpret_cast<util::generic_id*>(aligned);
      #ifndef NDEBUG
      memset(offset, 0, bytes_property_entity_id);
      #endif

      byte_counter += bytes_property_entity_id;
      assert(byte_counter <= bytes_to_alloc);
    }
    // Assign property_priority memory
    {
      void *offset = util::mem_offset(alloc_start, byte_counter);
      void *aligned = util::mem_next_16byte_boundry(offset);

      data->property_priority = reinterpret_cast<uint32_t*>(aligned);
      #ifndef NDEBUG
      memset(offset, 0, bytes_property_priority);
      #endif

      byte_counter += bytes_property_priority;
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
    // Assign property_post_process_id memory
    {
      void *offset = util::mem_offset(alloc_start, byte_counter);
      void *aligned = util::mem_next_16byte_boundry(offset);

      data->property_post_process_id = reinterpret_cast<util::generic_id*>(aligned);
      #ifndef NDEBUG
      memset(offset, 0, bytes_property_post_process_id);
      #endif

      byte_counter += bytes_property_post_process_id;
      assert(byte_counter <= bytes_to_alloc);
    }
    // Assign property_camera memory
    {
      void *offset = util::mem_offset(alloc_start, byte_counter);
      void *aligned = util::mem_next_16byte_boundry(offset);

      data->property_camera = reinterpret_cast<Camera::Camera_properties*>(aligned);
      #ifndef NDEBUG
      memset(offset, 0, bytes_property_camera);
      #endif

      byte_counter += bytes_property_camera;
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
camera_data_free(Camera_data *data)
{
  assert(data);
}


size_t
camera_data_get_size(const Camera_data *data)
{
  assert(data);
  return data->size;
}


size_t
camera_data_get_capacity(const Camera_data *data)
{
  assert(data);
  return data->capacity;
}


void
data_lock(const Camera_data *data)
{
  assert(data);
}


void
data_unlock(const Camera_data *data)
{
  assert(data);
}


bool
camera_data_push_back(Camera_data *data, const util::generic_id key, size_t *out_index)
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

  data->camera_id[index] = key;

  // Memset the properties
  {
    memset(&data->property_entity_id[index], 0, sizeof(*data->property_entity_id));
    memset(&data->property_priority[index], 0, sizeof(*data->property_priority));
    memset(&data->property_texture_id[index], 0, sizeof(*data->property_texture_id));
    memset(&data->property_post_process_id[index], 0, sizeof(*data->property_post_process_id));
    memset(&data->property_camera[index], 0, sizeof(*data->property_camera));
  }

  return true;
}


bool
camera_data_erase(Camera_data *data, const util::generic_id key)
{
  // Param check
  assert(data && key);

  size_t index_to_erase;

  if(camera_data_exists(data, key, &index_to_erase))
  {
    assert(index_to_erase < data->size);

    const size_t start_index = index_to_erase + 1;
    const size_t size_to_end = data->size - index_to_erase - 1;

    --(data->size);

    // Shuffle the memory down.
    memmove(&data->camera_id[index_to_erase], &data->camera_id[start_index], size_to_end * sizeof(*data->camera_id));
    memmove(&data->property_entity_id[index_to_erase], &data->property_entity_id[start_index], size_to_end * sizeof(*data->property_entity_id));
    memmove(&data->property_priority[index_to_erase], &data->property_priority[start_index], size_to_end * sizeof(*data->property_priority));
    memmove(&data->property_texture_id[index_to_erase], &data->property_texture_id[start_index], size_to_end * sizeof(*data->property_texture_id));
    memmove(&data->property_post_process_id[index_to_erase], &data->property_post_process_id[start_index], size_to_end * sizeof(*data->property_post_process_id));
    memmove(&data->property_camera[index_to_erase], &data->property_camera[start_index], size_to_end * sizeof(*data->property_camera));
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
camera_data_insert(Camera_data *data, const util::generic_id key, const size_t insert_index)
{
  assert(data && key);

  // If we are past the end of the size
  // Use push back.
  if(insert_index > data->size)
  {
    return camera_data_push_back(data, key);
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
    memmove(&data->camera_id[dest_index], &data->camera_id[insert_index], size_to_end * sizeof(*data->camera_id));
    memmove(&data->property_entity_id[dest_index], &data->property_entity_id[insert_index], size_to_end * sizeof(*data->property_entity_id));
    memmove(&data->property_priority[dest_index], &data->property_priority[insert_index], size_to_end * sizeof(*data->property_priority));
    memmove(&data->property_texture_id[dest_index], &data->property_texture_id[insert_index], size_to_end * sizeof(*data->property_texture_id));
    memmove(&data->property_post_process_id[dest_index], &data->property_post_process_id[insert_index], size_to_end * sizeof(*data->property_post_process_id));
    memmove(&data->property_camera[dest_index], &data->property_camera[insert_index], size_to_end * sizeof(*data->property_camera));

    // Add key to new entry.
    data->camera_id[insert_index] = key;

    return true;
  }

  return false;
}


bool
camera_data_exists(const Camera_data *data, const util::generic_id key, size_t *out_index)
{
  assert(data && key);

  if(data->size == 0)
  {
    return false;
  }

  bool found = false;

  size_t no_index;
  if(!out_index) { out_index = &no_index; }

  found = util::generic_id_search_linear(out_index, key, data->camera_id, data->size);

  return found;
}


bool
camera_data_get_property_entity_id(const Camera_data *data, const util::generic_id key, util::generic_id *out_value)
{
  size_t index;

  if(camera_data_exists(data, key, &index))
  {
    *out_value = data->property_entity_id[index];
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
camera_data_set_property_entity_id(Camera_data *data,  const util::generic_id key, const util::generic_id value)
{
  assert(data && key);

  size_t index;

  if(camera_data_exists(data, key, &index))
  {
    data->property_entity_id[index] = value;
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
camera_data_get_property_priority(const Camera_data *data, const util::generic_id key, uint32_t *out_value)
{
  size_t index;

  if(camera_data_exists(data, key, &index))
  {
    *out_value = data->property_priority[index];
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
camera_data_set_property_priority(Camera_data *data,  const util::generic_id key, const uint32_t value)
{
  assert(data && key);

  size_t index;

  if(camera_data_exists(data, key, &index))
  {
    data->property_priority[index] = value;
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
camera_data_get_property_texture_id(const Camera_data *data, const util::generic_id key, util::generic_id *out_value)
{
  size_t index;

  if(camera_data_exists(data, key, &index))
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
camera_data_set_property_texture_id(Camera_data *data,  const util::generic_id key, const util::generic_id value)
{
  assert(data && key);

  size_t index;

  if(camera_data_exists(data, key, &index))
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


bool
camera_data_get_property_post_process_id(const Camera_data *data, const util::generic_id key, util::generic_id *out_value)
{
  size_t index;

  if(camera_data_exists(data, key, &index))
  {
    *out_value = data->property_post_process_id[index];
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
camera_data_set_property_post_process_id(Camera_data *data,  const util::generic_id key, const util::generic_id value)
{
  assert(data && key);

  size_t index;

  if(camera_data_exists(data, key, &index))
  {
    data->property_post_process_id[index] = value;
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
camera_data_get_property_camera(const Camera_data *data, const util::generic_id key, Camera::Camera_properties *out_value)
{
  size_t index;

  if(camera_data_exists(data, key, &index))
  {
    *out_value = data->property_camera[index];
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
camera_data_set_property_camera(Camera_data *data,  const util::generic_id key, const Camera::Camera_properties value)
{
  assert(data && key);

  size_t index;

  if(camera_data_exists(data, key, &index))
  {
    data->property_camera[index] = value;
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
