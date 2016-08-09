

/*
  WARNING
  -------
  This file is auto generated any changes here may be overwritten.
  See code_gen.rake in scripts folder.

  This file was last generated on: Tue 09 Aug 2016
*/


#include <data/global_data/material_data.hpp>
#include <data/global_data/memory_data.hpp>
#include <common/error_strings.hpp>
#include <utilities/logging.hpp>
#include <utilities/memory.hpp>
#include <utilities/string_helpers.hpp>
#include <assert.h>
#include <cstring>


namespace Resource_data {


void
material_data_init(Material_data *data, const size_t size_hint)
{
  // Argument validation.
  assert(data && size_hint);

  // 16 byte alignment buffer, apply to all for safety.
  constexpr size_t simd_buffer = 16;

  // Calculate the various sizes of things.
  const size_t bytes_material_id = sizeof(*data->material_id) * size_hint + simd_buffer;
  const size_t bytes_property_name = sizeof(*data->property_name) * 32 * size_hint + simd_buffer;
  const size_t bytes_property_material_hash_id = sizeof(*data->property_material_hash_id) * size_hint + simd_buffer;
  const size_t bytes_property_material = sizeof(*data->property_material) * size_hint + simd_buffer;

  const size_t bytes_to_alloc = bytes_material_id + bytes_property_name + bytes_property_material_hash_id + bytes_property_material;

  // Allocate some memory.
  util::memory_chunk *data_memory = const_cast<util::memory_chunk*>(&data->memory);
  *data_memory = Memory::request_memory_chunk(bytes_to_alloc, "material_data");

  assert(data_memory->bytes_in_chunk == bytes_to_alloc);

  data_lock(data);

  // Init memory
  {
    size_t byte_counter = 0;
    const void *alloc_start = data->memory.chunk_start;

    // Assign material_id memory
    {
      void *offset = util::mem_offset(alloc_start, byte_counter);
      void *aligned = util::mem_next_16byte_boundry(offset);

      data->material_id = reinterpret_cast<util::generic_id*>(aligned);
      #ifndef NDEBUG
      memset(offset, 0, bytes_material_id);
      #endif

      byte_counter += bytes_material_id;
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
    // Assign property_material_hash_id memory
    {
      void *offset = util::mem_offset(alloc_start, byte_counter);
      void *aligned = util::mem_next_16byte_boundry(offset);

      data->property_material_hash_id = reinterpret_cast<Material_renderer::Material_id*>(aligned);
      #ifndef NDEBUG
      memset(offset, 0, bytes_property_material_hash_id);
      #endif

      byte_counter += bytes_property_material_hash_id;
      assert(byte_counter <= bytes_to_alloc);
    }
    // Assign property_material memory
    {
      void *offset = util::mem_offset(alloc_start, byte_counter);
      void *aligned = util::mem_next_16byte_boundry(offset);

      data->property_material = reinterpret_cast<Material_renderer::Material*>(aligned);
      #ifndef NDEBUG
      memset(offset, 0, bytes_property_material);
      #endif

      byte_counter += bytes_property_material;
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
material_data_free(Material_data *data)
{
  assert(data);
}


size_t
material_data_get_size(const Material_data *data)
{
  assert(data);
  return data->size;
}


size_t
material_data_get_capacity(const Material_data *data)
{
  assert(data);
  return data->capacity;
}


void
data_lock(const Material_data *data)
{
  assert(data);
}


void
data_unlock(const Material_data *data)
{
  assert(data);
}


util::generic_id
material_data_push_back(Material_data *data, size_t *out_index)
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

  data->material_id[index] = key;

  // Memset the properties
  {
    memset(&data->property_name[index * 32], 0, sizeof(*data->property_name));
    memset(&data->property_material_hash_id[index], 0, sizeof(*data->property_material_hash_id));
    memset(&data->property_material[index], 0, sizeof(*data->property_material));
  }

  return key;
}


bool
material_data_erase(Material_data *data, const util::generic_id key)
{
  // Param check
  assert(data && key);

  size_t index_to_erase;

  if(material_data_exists(data, key, &index_to_erase))
  {
    assert(index_to_erase < data->size);

    const size_t start_index = index_to_erase + 1;
    const size_t size_to_end = data->size - index_to_erase - 1;

    --(data->size);

    // Shuffle the memory down.
    memmove(&data->material_id[index_to_erase], &data->material_id[start_index], size_to_end * sizeof(*data->material_id));
    memmove(&data->property_name[index_to_erase * 32], &data->property_name[start_index * 32], (size_to_end * 32) * sizeof(*data->property_name));
    memmove(&data->property_material_hash_id[index_to_erase], &data->property_material_hash_id[start_index], size_to_end * sizeof(*data->property_material_hash_id));
    memmove(&data->property_material[index_to_erase], &data->property_material[start_index], size_to_end * sizeof(*data->property_material));
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
material_data_exists(const Material_data *data, const util::generic_id key, size_t *out_index)
{
  assert(data && key);

  if(data->size == 0)
  {
    return false;
  }

  bool found = false;

  size_t no_index;
  if(!out_index) { out_index = &no_index; }

  found = util::generic_id_search_binary(out_index, key, data->material_id, data->size);

  return found;
}


bool
material_data_get_property_name(const Material_data *data, const util::generic_id key, char const **out_value)
{
  size_t index;

  if(material_data_exists(data, key, &index))
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
material_data_set_property_name(Material_data *data,  const util::generic_id key, const char *value)
{
  assert(data && key);

  size_t index;

  if(material_data_exists(data, key, &index))
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
material_data_search_property_name(const Material_data *data, const char *value, util::generic_id *out_key)
{
  bool found = false;

  for(size_t i = 0; i < data->size; ++i)
  {
    if(!strcmp(value, &data->property_name[i * 32]))
    {
      found = true;

      if(out_key)
      {
        *out_key = data->material_id[i];
      }

      break;
    }
  }

  return found;
}


bool
material_data_get_property_material_hash_id(const Material_data *data, const util::generic_id key, Material_renderer::Material_id *out_value)
{
  size_t index;

  if(material_data_exists(data, key, &index))
  {
    *out_value = data->property_material_hash_id[index];
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
material_data_set_property_material_hash_id(Material_data *data,  const util::generic_id key, const Material_renderer::Material_id value)
{
  assert(data && key);

  size_t index;

  if(material_data_exists(data, key, &index))
  {
    data->property_material_hash_id[index] = value;
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
material_data_search_property_material_hash_id(const Material_data *data, const Material_renderer::Material_id value, util::generic_id *out_key)
{
  bool found = false;

  for(size_t i = 0; i < data->size; ++i)
  {
    if(value == data->property_material_hash_id[i])
    {
      found = true;

      if(out_key)
      {
        *out_key = data->material_id[i];
      }

      break;
    }
  }

  return found;
}


bool
material_data_get_property_material(const Material_data *data, const util::generic_id key, Material_renderer::Material **out_value)
{
  size_t index;

  if(material_data_exists(data, key, &index))
  {
    *out_value = &data->property_material[index];
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
material_data_set_property_material(Material_data *data,  const util::generic_id key, const Material_renderer::Material *value)
{
  assert(data && key);

  size_t index;

  if(material_data_exists(data, key, &index))
  {
    data->property_material[index] = *value;
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
