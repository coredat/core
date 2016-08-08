

/*
  WARNING
  -------
  This file is auto generated any changes here may be overwritten.
  See code_gen.rake in scripts folder.

  This file was last generated on: Mon 08 Aug 2016
*/


#include <data/world_data/physics_data.hpp>
#include <data/global_data/memory_data.hpp>
#include <common/error_strings.hpp>
#include <utilities/logging.hpp>
#include <utilities/memory.hpp>
#include <utilities/string_helpers.hpp>
#include <assert.h>
#include <cstring>


namespace World_data {


void
physics_data_init(Physics_data *data, const size_t size_hint)
{
  // Argument validation.
  assert(data && size_hint);

  // 16 byte alignment buffer, apply to all for safety.
  constexpr size_t simd_buffer = 16;

  // Calculate the various sizes of things.
  const size_t bytes_physics_id = sizeof(*data->physics_id) * size_hint + simd_buffer;
  const size_t bytes_property_transform = sizeof(*data->property_transform) * size_hint + simd_buffer;
  const size_t bytes_property_aabb_collider = sizeof(*data->property_aabb_collider) * size_hint + simd_buffer;
  const size_t bytes_property_transformed_aabb_collider = sizeof(*data->property_transformed_aabb_collider) * size_hint + simd_buffer;
  const size_t bytes_property_collision_id = sizeof(*data->property_collision_id) * size_hint + simd_buffer;
  const size_t bytes_property_rigidbody = sizeof(*data->property_rigidbody) * size_hint + simd_buffer;

  const size_t bytes_to_alloc = bytes_physics_id + bytes_property_transform + bytes_property_aabb_collider + bytes_property_transformed_aabb_collider + bytes_property_collision_id + bytes_property_rigidbody;

  // Allocate some memory.
  util::memory_chunk *data_memory = const_cast<util::memory_chunk*>(&data->memory);
  *data_memory = Memory::request_memory_chunk(bytes_to_alloc, "physics_data");

  assert(data_memory->bytes_in_chunk == bytes_to_alloc);

  data_lock(data);

  // Init memory
  {
    size_t byte_counter = 0;
    const void *alloc_start = data->memory.chunk_start;

    // Assign physics_id memory
    {
      void *offset = util::mem_offset(alloc_start, byte_counter);
      void *aligned = util::mem_next_16byte_boundry(offset);

      data->physics_id = reinterpret_cast<util::generic_id*>(aligned);
      #ifndef NDEBUG
      memset(offset, 0, bytes_physics_id);
      #endif

      byte_counter += bytes_physics_id;
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
    // Assign property_aabb_collider memory
    {
      void *offset = util::mem_offset(alloc_start, byte_counter);
      void *aligned = util::mem_next_16byte_boundry(offset);

      data->property_aabb_collider = reinterpret_cast<math::aabb*>(aligned);
      #ifndef NDEBUG
      memset(offset, 0, bytes_property_aabb_collider);
      #endif

      byte_counter += bytes_property_aabb_collider;
      assert(byte_counter <= bytes_to_alloc);
    }
    // Assign property_transformed_aabb_collider memory
    {
      void *offset = util::mem_offset(alloc_start, byte_counter);
      void *aligned = util::mem_next_16byte_boundry(offset);

      data->property_transformed_aabb_collider = reinterpret_cast<math::aabb*>(aligned);
      #ifndef NDEBUG
      memset(offset, 0, bytes_property_transformed_aabb_collider);
      #endif

      byte_counter += bytes_property_transformed_aabb_collider;
      assert(byte_counter <= bytes_to_alloc);
    }
    // Assign property_collision_id memory
    {
      void *offset = util::mem_offset(alloc_start, byte_counter);
      void *aligned = util::mem_next_16byte_boundry(offset);

      data->property_collision_id = reinterpret_cast<uint64_t*>(aligned);
      #ifndef NDEBUG
      memset(offset, 0, bytes_property_collision_id);
      #endif

      byte_counter += bytes_property_collision_id;
      assert(byte_counter <= bytes_to_alloc);
    }
    // Assign property_rigidbody memory
    {
      void *offset = util::mem_offset(alloc_start, byte_counter);
      void *aligned = util::mem_next_16byte_boundry(offset);

      data->property_rigidbody = reinterpret_cast<uintptr_t*>(aligned);
      #ifndef NDEBUG
      memset(offset, 0, bytes_property_rigidbody);
      #endif

      byte_counter += bytes_property_rigidbody;
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
physics_data_free(Physics_data *data)
{
  assert(data);
}


size_t
physics_data_get_size(const Physics_data *data)
{
  assert(data);
  return data->size;
}


size_t
physics_data_get_capacity(const Physics_data *data)
{
  assert(data);
  return data->capacity;
}


void
data_lock(const Physics_data *data)
{
  assert(data);
}


void
data_unlock(const Physics_data *data)
{
  assert(data);
}


bool
physics_data_push_back(Physics_data *data, const util::generic_id key, size_t *out_index)
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

  data->physics_id[index] = key;

  // Memset the properties
  {
    memset(&data->property_transform[index], 0, sizeof(*data->property_transform));
    memset(&data->property_aabb_collider[index], 0, sizeof(*data->property_aabb_collider));
    memset(&data->property_transformed_aabb_collider[index], 0, sizeof(*data->property_transformed_aabb_collider));
    memset(&data->property_collision_id[index], 0, sizeof(*data->property_collision_id));
    memset(&data->property_rigidbody[index], 0, sizeof(*data->property_rigidbody));
  }

  return true;
}


bool
physics_data_erase(Physics_data *data, const util::generic_id key)
{
  // Param check
  assert(data && key);

  size_t index_to_erase;

  if(physics_data_exists(data, key, &index_to_erase))
  {
    assert(index_to_erase < data->size);

    const size_t start_index = index_to_erase + 1;
    const size_t size_to_end = data->size - index_to_erase - 1;

    --(data->size);

    // Shuffle the memory down.
    memmove(&data->physics_id[index_to_erase], &data->physics_id[start_index], size_to_end * sizeof(*data->physics_id));
    memmove(&data->property_transform[index_to_erase], &data->property_transform[start_index], size_to_end * sizeof(*data->property_transform));
    memmove(&data->property_aabb_collider[index_to_erase], &data->property_aabb_collider[start_index], size_to_end * sizeof(*data->property_aabb_collider));
    memmove(&data->property_transformed_aabb_collider[index_to_erase], &data->property_transformed_aabb_collider[start_index], size_to_end * sizeof(*data->property_transformed_aabb_collider));
    memmove(&data->property_collision_id[index_to_erase], &data->property_collision_id[start_index], size_to_end * sizeof(*data->property_collision_id));
    memmove(&data->property_rigidbody[index_to_erase], &data->property_rigidbody[start_index], size_to_end * sizeof(*data->property_rigidbody));
  }
  else
  {
    return false;
  }

  return true;
}


bool
physics_data_insert(Physics_data *data, const util::generic_id key, const size_t insert_index)
{
  assert(data && key);

  // If we are past the end of the size
  // Use push back.
  if(insert_index > data->size)
  {
    return physics_data_push_back(data, key);
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
    memmove(&data->physics_id[dest_index], &data->physics_id[insert_index], size_to_end * sizeof(*data->physics_id));
    memmove(&data->property_transform[dest_index], &data->property_transform[insert_index], size_to_end * sizeof(*data->property_transform));
    memmove(&data->property_aabb_collider[dest_index], &data->property_aabb_collider[insert_index], size_to_end * sizeof(*data->property_aabb_collider));
    memmove(&data->property_transformed_aabb_collider[dest_index], &data->property_transformed_aabb_collider[insert_index], size_to_end * sizeof(*data->property_transformed_aabb_collider));
    memmove(&data->property_collision_id[dest_index], &data->property_collision_id[insert_index], size_to_end * sizeof(*data->property_collision_id));
    memmove(&data->property_rigidbody[dest_index], &data->property_rigidbody[insert_index], size_to_end * sizeof(*data->property_rigidbody));

    // Add key to new entry.
    data->physics_id[insert_index] = key;

    return true;
  }

  return false;
}


bool
physics_data_exists(const Physics_data *data, const util::generic_id key, size_t *out_index)
{
  assert(data && key);

  if(data->size == 0)
  {
    return false;
  }

  bool found = false;

  size_t no_index;
  if(!out_index) { out_index = &no_index; }

  found = util::generic_id_search_linear(out_index, key, data->physics_id, data->size);

  return found;
}


bool
physics_data_get_property_transform(const Physics_data *data, const util::generic_id key, math::transform *out_value)
{
  size_t index;

  if(physics_data_exists(data, key, &index))
  {
    *out_value = data->property_transform[index];
  }
  else
  {
    return false;
  }

  return true;
}


bool
physics_data_set_property_transform(Physics_data *data,  const util::generic_id key, const math::transform value)
{
  assert(data && key);

  size_t index;

  if(physics_data_exists(data, key, &index))
  {
    data->property_transform[index] = value;
  }
  else
  {
    return false;
  }

  return true;
}


bool
physics_data_get_property_aabb_collider(const Physics_data *data, const util::generic_id key, math::aabb *out_value)
{
  size_t index;

  if(physics_data_exists(data, key, &index))
  {
    *out_value = data->property_aabb_collider[index];
  }
  else
  {
    return false;
  }

  return true;
}


bool
physics_data_set_property_aabb_collider(Physics_data *data,  const util::generic_id key, const math::aabb value)
{
  assert(data && key);

  size_t index;

  if(physics_data_exists(data, key, &index))
  {
    data->property_aabb_collider[index] = value;
  }
  else
  {
    return false;
  }

  return true;
}


bool
physics_data_get_property_transformed_aabb_collider(const Physics_data *data, const util::generic_id key, math::aabb *out_value)
{
  size_t index;

  if(physics_data_exists(data, key, &index))
  {
    *out_value = data->property_transformed_aabb_collider[index];
  }
  else
  {
    return false;
  }

  return true;
}


bool
physics_data_set_property_transformed_aabb_collider(Physics_data *data,  const util::generic_id key, const math::aabb value)
{
  assert(data && key);

  size_t index;

  if(physics_data_exists(data, key, &index))
  {
    data->property_transformed_aabb_collider[index] = value;
  }
  else
  {
    return false;
  }

  return true;
}


bool
physics_data_get_property_collision_id(const Physics_data *data, const util::generic_id key, uint64_t *out_value)
{
  size_t index;

  if(physics_data_exists(data, key, &index))
  {
    *out_value = data->property_collision_id[index];
  }
  else
  {
    return false;
  }

  return true;
}


bool
physics_data_set_property_collision_id(Physics_data *data,  const util::generic_id key, const uint64_t value)
{
  assert(data && key);

  size_t index;

  if(physics_data_exists(data, key, &index))
  {
    data->property_collision_id[index] = value;
  }
  else
  {
    return false;
  }

  return true;
}


bool
physics_data_get_property_rigidbody(const Physics_data *data, const util::generic_id key, uintptr_t *out_value)
{
  size_t index;

  if(physics_data_exists(data, key, &index))
  {
    *out_value = data->property_rigidbody[index];
  }
  else
  {
    return false;
  }

  return true;
}


bool
physics_data_set_property_rigidbody(Physics_data *data,  const util::generic_id key, const uintptr_t value)
{
  assert(data && key);

  size_t index;

  if(physics_data_exists(data, key, &index))
  {
    data->property_rigidbody[index] = value;
  }
  else
  {
    return false;
  }

  return true;
}


} // ns
