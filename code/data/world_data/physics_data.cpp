#include <data/world_data/physics_data.hpp>
#include <data/global_data/memory_data.hpp>
#include <common/error_strings.hpp>
#include <utilities/memory.hpp>
#include <utilities/logging.hpp>
#include <utilities/bits.hpp>
#include <assert.h>


namespace World_data {


void
physics_init(Physics_data *data,
             const uint32_t size_hint)
{
  assert(data && size_hint);
  
  // SIMD Buffer, apply to all data.
  constexpr size_t simd_buffer = 16;
  
  const size_t bytes_entity_id = sizeof(*data->entity_id) * size_hint + simd_buffer;
  const size_t bytes_transform = sizeof(*data->transform) * size_hint + simd_buffer;
  const size_t bytes_aabb      = sizeof(*data->aabb_collider) * size_hint + simd_buffer;
  const size_t bytes_mask      = sizeof(*data->collision_id) * size_hint + simd_buffer;
  
  const size_t bytes_to_alloc  = bytes_entity_id + bytes_transform + bytes_aabb + bytes_mask;
  
  // Allocate memory
  util::memory_chunk *data_memory = const_cast<util::memory_chunk*>(&data->memory);
  *data_memory = Memory::request_memory_chunk(bytes_to_alloc, "phys-data");
  
  assert(data_memory->bytes_in_chunk == bytes_to_alloc);
  
//  lock(data);

  // Setup memory
  {
    size_t byte_counter = 0;
    const void *alloc_start = data->memory.chunk_16_byte_aligned_start;
    
    // Set ids
    {
      void *offset  = util::mem_offset(alloc_start, byte_counter);
      void *aligned = util::mem_next_16byte_boundry(offset);
    
      data->entity_id = reinterpret_cast<util::generic_id*>(aligned);
      #ifndef NDEBUG
      memset(data->entity_id, 0, bytes_entity_id);
      #endif
      byte_counter += bytes_entity_id;
      assert(byte_counter <= bytes_to_alloc);
    }
    
    // Set transforms
    {
      void *offset  = util::mem_offset(alloc_start, byte_counter);
      void *aligned = util::mem_next_16byte_boundry(offset);

      data->transform = reinterpret_cast<math::transform*>(aligned);
      #ifndef NDEBUG
      memset(data->entity_id, 0, bytes_transform);
      #endif
      byte_counter += bytes_transform;
      assert(byte_counter <= bytes_to_alloc);
    }

    // AABB's
    {
      void *offset  = util::mem_offset(alloc_start, byte_counter);
      void *aligned = util::mem_next_16byte_boundry(offset);
    
      data->aabb_collider = reinterpret_cast<math::aabb*>(aligned);
      #ifndef NDEBUG
      memset(data->entity_id, 0, bytes_aabb);
      #endif
      byte_counter += bytes_aabb;
      assert(byte_counter <= bytes_to_alloc);
    }
    
    // Collision mask
    {
      void *offset  = util::mem_offset(alloc_start, byte_counter);
      void *aligned = util::mem_next_16byte_boundry(offset);
      
      data->collision_id = reinterpret_cast<uint64_t*>(aligned);
      #ifndef NDEBUG
      memset(data->entity_id, 0, bytes_mask);
      #endif
      byte_counter += bytes_mask;
      assert(byte_counter <= bytes_to_alloc);
    }
    
    // Set the size and capacity
    {
      data->size = 0;
      
      uint32_t *capacity = const_cast<uint32_t*>(&data->capacity);
      *capacity = size_hint;
    }
  }
  
//  unlock(data);
  
//  static util::generic_id entity_data[2048];
//  data->entity_id = entity_data;
//  memset(entity_data, 0, sizeof(entity_data));
//  
//  static math::transform trans[2048];
//  data->transform = trans;
//  memset(trans, 0, sizeof(trans));
//  
//  static math::aabb aabbs[2048];
//  data->aabb_collider = aabbs;
//
//  static uint64_t collision_mask[2048];
//  data->collision_id = collision_mask;
//  memset(collision_mask, 0, sizeof(collision_mask));
//  
//  data->capacity = 2048;
}


void
physics_free(Physics_data *data)
{
  // Delete the contents.
}


void
physics_add(Physics_data *data,
            const util::generic_id id,
            const math::aabb *aabb,
            const math::transform *trans)
{
  uint32_t index = -1;
  
  for(uint32_t i = 0; i < data->capacity; ++i)
  {
    // Found a free slot
    if(i < index && data->entity_id[i] == util::generic_id_invalid())
    {
      index = i;
    }
    
    // Found an existing entity that shares same id.
    if(data->entity_id[i] == id)
    {
      index = i;
      break;
    }
  }
  
  if(index < (uint32_t)-1)
  {
    data->entity_id[index] = id;
    data->aabb_collider[index] = *aabb;
    data->transform[index] = *trans;
  }
  else
  {
    LOG_ERROR(Error_string::no_free_space());
  }
  
  ++data->size;
}


void
physics_add(Physics_data *data,
            const util::generic_id entity_id)
{
  // If it doens't exist push a new entity in.
  if(!physics_exists(data, entity_id))
  {
    if(data->size <= data->capacity)
    {
      data->entity_id[data->size++] = entity_id;
    }
  }
}


bool
physics_exists(Physics_data *data,
               const util::generic_id entity_id,
               size_t *index)
{
  size_t search_index;
  if(!index)
  {
    index = &search_index;
  }

  return util::generic_id_search_binary(index,
                                        entity_id,
                                        data->entity_id,
                                        data->size);
}


void
physics_update_aabb(Physics_data *data,
                    const util::generic_id entity_id,
                    const math::aabb *aabb)
{
  size_t index;
  if(physics_exists(data, entity_id, &index))
  {
    data->aabb_collider[index] = *aabb;
  }
}


void
physics_update_transform(Physics_data *data,
                         const util::generic_id entity_id,
                         const math::transform *transform)
{
  size_t index;
  if(physics_exists(data, entity_id, &index))
  {
    data->transform[index] = *transform;
  }
}


void
physics_update_collision_mask(Physics_data *data,
                              const util::generic_id entity_id,
                              const uint32_t this_id,
                              const uint32_t mask_ids)
{
  size_t index;
  if(physics_exists(data, entity_id, &index))
  {
    const uint64_t flags = util::bits_pack(this_id, mask_ids);
    data->collision_id[index] = flags;
  }
}



void
physics_remove(Physics_data *data,
               const util::generic_id id)
{
  for(uint32_t i = 0; i < data->capacity; ++i)
  {
    if(data->entity_id[i] == id)
    {
      const uint32_t start = i;
      
      const uint32_t start_move = i + 1;
      const uint32_t end_move = data->capacity - i - 1;
      
      memmove(&data->entity_id[start], &data->entity_id[start_move], end_move * sizeof(*data->entity_id));
      memmove(&data->transform[start], &data->transform[start_move], end_move * sizeof(*data->transform));
      memmove(&data->aabb_collider[start], &data->aabb_collider[start_move], end_move * sizeof(*data->aabb_collider));
      memmove(&data->collision_id[start], &data->collision_id[start_move], end_move * sizeof(*data->collision_id));

      --data->size;
    }
  }
}


void
physics_update(Physics_data *data,
               const util::generic_id id,
               const math::aabb *aabb,
               const math::transform *trans)
{
  for(uint32_t i = 0; i < data->capacity; ++i)
  {
    // Found an existing entity that shares same id.
    if(data->entity_id[i] == id)
    {
      data->aabb_collider[i] = *aabb;
      data->transform[i] = *trans;
    }
  }
}



} // ns