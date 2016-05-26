#include <data/world_data/transform_data.hpp>
#include <data/global_data/memory_data.hpp>
#include <common/error_strings.hpp>
#include <utilities/memory.hpp>
#include <utilities/logging.hpp>
#include <assert.h>


namespace World_data {


void
lock(Transform_data *data)
{
}


void
unlock(Transform_data *data)
{
}


void
transform_data_init(Transform_data *data,
                    const uint32_t size_hint)
{
  assert(data);
  assert(size_hint);
  
  // SIMD buffer, we apply to all arrays.
  constexpr size_t simd_buffer = 16;
  
  // Calculate consumed memory.
  const size_t bytes_entity_id  = sizeof(*data->entity_id) * size_hint + simd_buffer;
  const size_t bytes_transforms = sizeof(*data->transform) * size_hint + simd_buffer;
  const size_t bytes_aabb       = sizeof(*data->aabb) * size_hint + simd_buffer;
  
  const size_t bytes_to_alloc   = bytes_entity_id + bytes_transforms + bytes_aabb;
  
  // Allocate memory
  util::memory_chunk *data_memory = const_cast<util::memory_chunk*>(&data->memory);
  *data_memory = Memory::request_memory_chunk(bytes_to_alloc, "trans-data");
  
  assert(data_memory->bytes_in_chunk == bytes_to_alloc);
  
  lock(data);
  
  // Setup the memory
  {
    size_t byte_counter = 0;
    const void *alloc_start = data->memory.chunk_16_byte_aligned_start;
    
    // Set ids memory
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
    
    // Set ids memory
    {
      void *offset = util::mem_offset(alloc_start, byte_counter);
      void *aligned = util::mem_next_16byte_boundry(offset);
    
      data->transform = reinterpret_cast<math::transform*>(aligned);
      #ifndef NDEBUG
      memset(offset, 0, bytes_transforms);
      #endif
      byte_counter += bytes_transforms;
      assert(byte_counter <= bytes_to_alloc);
    }
    
    // Set ids memory
    {
      void *offset = util::mem_offset(alloc_start, byte_counter);
      void *aligned = util::mem_next_16byte_boundry(offset);
      
      data->aabb = reinterpret_cast<math::aabb*>(aligned);
      #ifndef NDEBUG
      memset(offset, 0, bytes_aabb);
      #endif
      byte_counter += bytes_aabb;
      assert(byte_counter <= bytes_to_alloc);
    }
  }

  // Set the size and capacity
  {
    data->size = 0;
    
    uint32_t *capacity = const_cast<uint32_t*>(&data->capacity);
    *capacity = size_hint;
  }
  
  unlock(data);
}


void
transform_data_free(Transform_data *data)
{
}


void
transform_data_add_transform(Transform_data *data,
                             const util::generic_id id,
                             const math::transform *trans,
                             const math::aabb *aabb)
{
  assert(data && data->size < data->capacity);
  assert(id);

  lock(data);
  
  const uint32_t index = data->size;
  ++(data->size);
  
  data->entity_id[index] = id;
  
  if(trans) { data->transform[index] = *trans; }
  if(aabb)  { data->aabb[index] = *aabb;       }
  
  unlock(data);
}


void
transform_data_remove_transform(Transform_data *data,
                                const util::generic_id id)
{
  assert(id && data);
  
  lock(data);
  
  size_t index_to_erase;
  if(transform_data_exists(data, id, &index_to_erase))
  {
    const size_t start_index = index_to_erase + 1;
    const size_t size_to_end = data->size - index_to_erase - 1;
    --(data->size);
    
    memmove(&data->entity_id[index_to_erase], &data->entity_id[start_index], size_to_end * sizeof(*data->entity_id));
    memmove(&data->transform[index_to_erase], &data->transform[start_index], size_to_end * sizeof(*data->transform));
    memmove(&data->aabb[index_to_erase],      &data->aabb[start_index],      size_to_end * sizeof(*data->aabb));
  }
  else
  {
    assert(false);
    LOG_ERROR(Error_string::entity_not_found());    
  }
  
  unlock(data);
}


bool
transform_data_exists(const Transform_data *data,
                      const util::generic_id id,
                      size_t *out_index)
{
  assert(data && id);
  
  bool found = false;
  
  size_t no_index;
  if(!out_index) { out_index = &no_index; }
  
  found = util::generic_id_search_binary(out_index, id, data->entity_id, data->size);
  
  return found;
}


} // ns