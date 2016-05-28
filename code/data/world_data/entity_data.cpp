#include <data/world_data/entity_data.hpp>
#include <data/global_data/memory_data.hpp>
#include <utilities/memory.hpp>
#include <utilities/logging.hpp>
#include <utilities/conversion.hpp>
#include <utilities/string_helpers.hpp>
#include <common/error_strings.hpp>
#include <assert.h>


namespace
{
  constexpr uint32_t entity_data_size_of_name = 32;
}


namespace World_data {


void
lock(Entity_data *data)
{
}


void
unlock(Entity_data *data)
{
}


void
entity_data_init(Entity_data *data,
                 const uint32_t size_hint)
{
  assert(data);
  assert(size_hint);
  
  // 16 byte alignement buffer, apply to all for safty.
  constexpr size_t simd_buffer = 16;
  
  // Calculate the various sizes of things.
  const size_t bytes_entity_id = sizeof(*data->entity_id) * size_hint + simd_buffer;
  const size_t bytes_tags      = sizeof(*data->tags) * size_hint + simd_buffer;
  const size_t bytes_name      = sizeof(*data->entity_name) * entity_data_size_of_name * size_hint + simd_buffer;
  const size_t bytes_user_data = sizeof(*data->user_data) * size_hint + simd_buffer;
  
  const size_t mem_to_alloc    = bytes_entity_id + bytes_tags + bytes_name + bytes_user_data;
  
  // Allocate some memory.
  util::memory_chunk *data_memory = const_cast<util::memory_chunk*>(&data->memory);
  *data_memory = Memory::request_memory_chunk(mem_to_alloc, "entity-data");
  
  assert(data_memory->bytes_in_chunk == mem_to_alloc);
  
  lock(data);
  
  // Setup the memory
  {
    size_t byte_counter = 0;
    const void *alloc_start = data->memory.chunk_start;
    
    // Set ids memory
    {
      void *offset = util::mem_offset(alloc_start, byte_counter);
      void *aligned = util::mem_next_16byte_boundry(offset);
    
      data->entity_id = reinterpret_cast<util::generic_id*>(aligned);
      #ifndef NDEBUG
      memset(offset, 0, bytes_entity_id);
      #endif
      byte_counter += bytes_entity_id;
      assert(byte_counter <= mem_to_alloc);
    }
    
    // Set name memory
    {
      void *offset = util::mem_offset(alloc_start, byte_counter);
      void *aligned = util::mem_next_16byte_boundry(offset);
    
      data->entity_name = reinterpret_cast<char*>(aligned);
      #ifndef NDEBUG
      memset(offset, 0, bytes_name);
      #endif
      byte_counter += bytes_name;
      assert(byte_counter <= mem_to_alloc);
    }
    
    // Set tag memory
    {
      void *offset = util::mem_offset(alloc_start, byte_counter);
      void *aligned = util::mem_next_16byte_boundry(offset);
    
      data->tags = reinterpret_cast<uint32_t*>(aligned);
      #ifndef NDEBUG
      memset(offset, 0, bytes_tags);
      #endif
      byte_counter += bytes_tags;
      assert(byte_counter <= mem_to_alloc);
    }
    
    // Set user data memory
    {
      void *offset = util::mem_offset(alloc_start, byte_counter);
      void *aligned = util::mem_next_16byte_boundry(offset);
      
      data->user_data = reinterpret_cast<uintptr_t*>(aligned);
      #ifndef NDEBUG
      memset(offset, 0, bytes_user_data);
      #endif
      byte_counter += bytes_user_data;
      assert(byte_counter <= mem_to_alloc);
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
entity_data_free(Entity_data *data)
{
  lock(data);
  unlock(data);
}


void
entity_data_add_entity(Entity_data *data,
                       const util::generic_id id,
                       const char *name,
                       const uint32_t *tags)
{
  assert(data && data->size < data->capacity);
  assert(id);

  lock(data);
  
  const uint32_t index = data->size;
  ++(data->size);
  
  data->entity_id[index] = id;
  
  // Set Tags
  if(tags)
  {
    data->tags[index] = *tags;
  }
  else
  {
    data->tags[index] = 0;
  }

  // Set Name
  if(name)
  {
    strlcpy(&data->entity_name[index * entity_data_size_of_name], name, entity_data_size_of_name);
  }
  else
  {
    data->entity_name[index * entity_data_size_of_name] = '\0';
  }

  unlock(data);
}


void
entity_data_set_name(Entity_data *data,
                     const util::generic_id id,
                     const char *name)
{
  assert(data && id && name);

  lock(data);

  size_t index;
  
  if(entity_data_exists(data, id, &index))
  {
    char *dest = &data->entity_name[index * entity_data_size_of_name];
    strncpy(dest, name, entity_data_size_of_name - 1);
  }
  else
  {
    LOG_ERROR(Error_string::entity_not_found());
  }
  
  unlock(data);
}
  
  
char*
entity_data_get_name(Entity_data *data,
                     const util::generic_id id)
{
  assert(data && id);
  
  lock(data);
  
  size_t index;
  char *result = nullptr;
  
  if(entity_data_exists(data, id, &index))
  {
    result = &data->entity_name[index * entity_data_size_of_name];
  }
  else
  {
    LOG_ERROR(Error_string::entity_not_found());
  }
  
  unlock(data);
  
  return result;
}


void
entity_data_remove_entity(Entity_data *data,
                          const util::generic_id id)
{
  assert(id && data);
  
  lock(data);
  
  size_t index_to_erase;
  if(entity_data_exists(data, id, &index_to_erase))
  {
    assert(index_to_erase < data->size);
  
    const size_t start_index = index_to_erase + 1;
    const size_t size_to_end = data->size - index_to_erase - 1;
    
    --(data->size);
    
    memmove(&data->entity_id[index_to_erase],   &data->entity_id[start_index],    size_to_end * sizeof(*data->entity_id));
    memmove(&data->user_data[index_to_erase],   &data->user_data[start_index],    size_to_end * sizeof(*data->user_data));
    memmove(&data->tags[index_to_erase],        &data->tags[start_index],         size_to_end * sizeof(*data->tags));
    
    // Name we need to mul by the size of the string.
    memmove(&data->entity_name[index_to_erase * entity_data_size_of_name],
            &data->entity_name[start_index * entity_data_size_of_name],
            (size_to_end * entity_data_size_of_name) * sizeof(*data->entity_name));
  }
  else
  {
    LOG_ERROR(Error_string::entity_not_found());
    assert(false);
  }
  
  unlock(data);
}


bool
entity_data_exists(Entity_data *data,
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