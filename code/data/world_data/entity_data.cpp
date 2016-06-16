
/*
  Warning:
  This file is auto_generated any changes here may be overwritten.
*/

#include <data/world_data/entity_data.hpp>
#include <data/global_data/memory_data.hpp>
#include <common/error_strings.hpp>
#include <utilities/logging.hpp>
#include <utilities/memory.hpp>
#include <assert.h>


namespace World_data {


void
entity_data_init(Entity_data *data, const size_t size_hint)
{
  // Argument validation.
  assert(data && size_hint);

  // 16 byte alignment buffer, apply to all for safety.
  constexpr size_t simd_buffer = 16;

  // Calculate the various sizes of things.
  const size_t bytes_entity_id = sizeof(*data->entity_id) * size_hint + simd_buffer;
  const size_t bytes_property_name = sizeof(*data->property_name) * 32 * size_hint + simd_buffer;
  const size_t bytes_property_tag = sizeof(*data->property_tag) * size_hint + simd_buffer;
  const size_t bytes_property_components = sizeof(*data->property_components) * size_hint + simd_buffer;
  const size_t bytes_property_user_data = sizeof(*data->property_user_data) * size_hint + simd_buffer;

  const size_t bytes_to_alloc = bytes_entity_id + bytes_property_name + bytes_property_tag + bytes_property_components + bytes_property_user_data;

  // Allocate some memory.
  util::memory_chunk *data_memory = const_cast<util::memory_chunk*>(&data->memory);
  *data_memory = Memory::request_memory_chunk(bytes_to_alloc, "entity_data");

  assert(data_memory->bytes_in_chunk == bytes_to_alloc);

  data_lock(data);

  // Init memory
  {
    size_t byte_counter = 0;
    const void *alloc_start = data->memory.chunk_start;

    // Assign entity_id memory
    {
      void *offset = util::mem_offset(alloc_start, byte_counter);
      void *aligned = util::mem_next_16byte_boundry(offset);

      data->entity_id = reinterpret_cast<util::generic_id*>(aligned);
      #ifndef NDEBUG
      memset(offset, 0, bytes_entity_id);
      #endif

      byte_counter += bytes_entity_id;
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
    // Assign property_tag memory
    {
      void *offset = util::mem_offset(alloc_start, byte_counter);
      void *aligned = util::mem_next_16byte_boundry(offset);

      data->property_tag = reinterpret_cast<uint32_t*>(aligned);
      #ifndef NDEBUG
      memset(offset, 0, bytes_property_tag);
      #endif

      byte_counter += bytes_property_tag;
      assert(byte_counter <= bytes_to_alloc);
    }
    // Assign property_components memory
    {
      void *offset = util::mem_offset(alloc_start, byte_counter);
      void *aligned = util::mem_next_16byte_boundry(offset);

      data->property_components = reinterpret_cast<uint32_t*>(aligned);
      #ifndef NDEBUG
      memset(offset, 0, bytes_property_components);
      #endif

      byte_counter += bytes_property_components;
      assert(byte_counter <= bytes_to_alloc);
    }
    // Assign property_user_data memory
    {
      void *offset = util::mem_offset(alloc_start, byte_counter);
      void *aligned = util::mem_next_16byte_boundry(offset);

      data->property_user_data = reinterpret_cast<uintptr_t*>(aligned);
      #ifndef NDEBUG
      memset(offset, 0, bytes_property_user_data);
      #endif

      byte_counter += bytes_property_user_data;
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
entity_data_free(Entity_data *data)
{
  assert(data);
}


size_t
entity_data_get_size(const Entity_data *data)
{
  assert(data);
  return data->size;
}


size_t
entity_data_get_capacity(const Entity_data *data)
{
  assert(data);
  return data->capacity;
}


void
data_lock(const Entity_data *data)
{
  assert(data);
}


void
data_unlock(const Entity_data *data)
{
  assert(data);
}


util::generic_id
entity_data_push_back(Entity_data *data, size_t *out_index)
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

  data->entity_id[index] = key;

  // Memset the properties
  {
    memset(&data->property_name[index * 32], 0, sizeof(*data->property_name));
    memset(&data->property_tag[index], 0, sizeof(*data->property_tag));
    memset(&data->property_components[index], 0, sizeof(*data->property_components));
    memset(&data->property_user_data[index], 0, sizeof(*data->property_user_data));
  }

  return key;
}


bool
entity_data_erase(Entity_data *data, const util::generic_id key)
{
  // Param check
  assert(data && key);

  size_t index_to_erase;

  if(entity_data_exists(data, key, &index_to_erase))
  {
    assert(index_to_erase < data->size);

    const size_t start_index = index_to_erase + 1;
    const size_t size_to_end = data->size - index_to_erase - 1;

    --(data->size);

    // Shuffle the memory down.
    memmove(&data->entity_id[index_to_erase], &data->entity_id[start_index], size_to_end * sizeof(*data->entity_id));
    memmove(&data->property_name[index_to_erase * 32], &data->property_name[start_index * 32], (size_to_end * 32) * sizeof(*data->property_name));
    memmove(&data->property_tag[index_to_erase], &data->property_tag[start_index], size_to_end * sizeof(*data->property_tag));
    memmove(&data->property_components[index_to_erase], &data->property_components[start_index], size_to_end * sizeof(*data->property_components));
    memmove(&data->property_user_data[index_to_erase], &data->property_user_data[start_index], size_to_end * sizeof(*data->property_user_data));
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
entity_data_exists(const Entity_data *data, const util::generic_id key, size_t *out_index)
{
  assert(data && key);

  if(data->size == 0)
  {
    return false;
  }

  bool found = false;

  size_t no_index;
  if(!out_index) { out_index = &no_index; }

  found = util::generic_id_search_binary(out_index, key, data->entity_id, data->size);

  return found;
}


bool
entity_data_get_property_name(const Entity_data *data, const util::generic_id key, char const **out_value)
{
  size_t index;

  if(entity_data_exists(data, key, &index))
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
entity_data_set_property_name(Entity_data *data,  const util::generic_id key, const char *value)
{
  assert(data && key);

  size_t index;

  if(entity_data_exists(data, key, &index))
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
entity_data_get_property_tag(const Entity_data *data, const util::generic_id key, uint32_t *out_value)
{
  size_t index;

  if(entity_data_exists(data, key, &index))
  {
    *out_value = data->property_tag[index];
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
entity_data_set_property_tag(Entity_data *data,  const util::generic_id key, const uint32_t value)
{
  assert(data && key);

  size_t index;

  if(entity_data_exists(data, key, &index))
  {
    data->property_tag[index] = value;
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
entity_data_get_property_components(const Entity_data *data, const util::generic_id key, uint32_t *out_value)
{
  size_t index;

  if(entity_data_exists(data, key, &index))
  {
    *out_value = data->property_components[index];
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
entity_data_set_property_components(Entity_data *data,  const util::generic_id key, const uint32_t value)
{
  assert(data && key);

  size_t index;

  if(entity_data_exists(data, key, &index))
  {
    data->property_components[index] = value;
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
entity_data_get_property_user_data(const Entity_data *data, const util::generic_id key, uintptr_t *out_value)
{
  size_t index;

  if(entity_data_exists(data, key, &index))
  {
    *out_value = data->property_user_data[index];
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
entity_data_set_property_user_data(Entity_data *data,  const util::generic_id key, const uintptr_t value)
{
  assert(data && key);

  size_t index;

  if(entity_data_exists(data, key, &index))
  {
    data->property_user_data[index] = value;
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
