#include <data/world_data/renderer_mesh_data.hpp>
#include <data/global_data/memory_data.hpp>
#include <utilities/memory.hpp>
#include <string.h>
#include <assert.h>


namespace World_data {


void
lock(Mesh_renderer_data *data)
{
}


void
unlock(Mesh_renderer_data *data)
{
}
  

void
mesh_renderer_init(Mesh_renderer_data *data,
                   const uint32_t size_hint)
{
  assert(data);
  assert(size_hint);

  // We align everything by 16 bytes.
  constexpr size_t simd_buffer = 16;

  // Calculate the size of things
  const size_t bytes_entity_id  = sizeof(*data->entity_id) * size_hint + simd_buffer;
  const size_t bytes_draw_calls = sizeof(*data->mesh_draw_calls) * size_hint + simd_buffer;
  
  const size_t bytes_to_alloc   = bytes_entity_id + bytes_draw_calls;
  
  // Allocate some memory
  util::memory_chunk *data_memory = const_cast<util::memory_chunk*>(&data->memory);
  *data_memory = Memory::request_memory_chunk(bytes_to_alloc, "draw-call-data");
  
  assert(data_memory->bytes_in_chunk == bytes_to_alloc);
  
  lock(data);
  
  // Setup the memory
  {
    size_t byte_counter = 0;
    const void *alloc_start = data->memory.chunk_16_byte_aligned_start;

    // Entity ids
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

    // Draw calls
    {
      void *offset = util::mem_offset(alloc_start, byte_counter);
      void *aligned = util::mem_next_16byte_boundry(offset);
    
      data->mesh_draw_calls = reinterpret_cast<Mesh_renderer_draw_call*>(aligned);
      #ifndef NDEBUG
      memset(offset, 0, bytes_draw_calls);
      #endif
      byte_counter += bytes_draw_calls;
      assert(byte_counter <= bytes_to_alloc);
    }
  }
  
  {
    data->size = 0;
    
    uint32_t *capacity = const_cast<uint32_t*>(&data->capacity);
    *capacity = size_hint;
  }
  
  unlock(data);
}


void
mesh_renderer_add(Mesh_renderer_data *data,
                  const util::generic_id id,
                  const uint32_t model_id,
                  const uint32_t texture_id)
{
  assert(data);
  assert(id);
  
  lock(data);

  size_t index;
  if(mesh_renderer_exists(data, id, &index))
  {
    assert(false); // already exsits, maybe just update the detail with log msg.
    return;
  }
  
  const uint32_t new_index = data->size;
  
  data->entity_id[new_index] = id;
  data->mesh_draw_calls[new_index].model = model_id;
  data->mesh_draw_calls[new_index].texture = texture_id;
  
  ++(data->size);
  
  unlock(data);
}


void
mesh_renderer_update_model(Mesh_renderer_data *data,
                           const util::generic_id id,
                           const uint32_t model_id)
{
  assert(data && id);

  lock(data);

  size_t index;
  if(mesh_renderer_exists(data, id, &index))
  {
    data->mesh_draw_calls[index].model = model_id;
  }
  
  unlock(data);
}


void
mesh_renderer_update_texture(Mesh_renderer_data *data,
                             const util::generic_id id,
                             const uint32_t texture_id)
{
  assert(data && id);
  
  lock(data);

  size_t index;
  if(mesh_renderer_exists(data, id, &index))
  {
    data->mesh_draw_calls[index].texture = texture_id;
  }
  
  unlock(data);
}


void
mesh_renderer_remove(Mesh_renderer_data *data,
                     const util::generic_id id)
{
  assert(data && id);
  
  lock(data);

  size_t index;
  if(mesh_renderer_exists(data, id, &index))
  {
    const size_t start_move = index + 1;
    const size_t end_move = data->size - index - 1;
    --(data->size);
    
    memmove(&data->entity_id[index],        &data->entity_id[start_move],       end_move * sizeof(*data->entity_id));
    memmove(&data->mesh_draw_calls[index],  &data->mesh_draw_calls[start_move], end_move * sizeof(*data->mesh_draw_calls));
  }
  else
  {
    assert(false); // change to warning.
  }
  
  unlock(data);
}
  
  
bool
mesh_renderer_exists(const Mesh_renderer_data *data,
                     const util::generic_id id,
                     size_t *index)
{
  assert(data && id);

  size_t *search_index = index;
  size_t dummy_index;
  
  if(data->size == 0)
  {
    return false;
  }
  
  if(search_index == nullptr)
  {
    search_index = &dummy_index;
  }
  
  return util::generic_id_search_binary(search_index, id, data->entity_id, data->size);
}
  
  
} // ns