#include "logic_pool.hpp"
#include <cstring>
#include <assert.h>
#include <core/interface/component.hpp>
#include <core/interface/entity.hpp>
#include <data/world_data/world_data.hpp>
#include <core/memory/memory.hpp>
#include <string>
#include <sstream>


namespace World_data {


void
logic_pool_init(Logic_pool *pool,
                const bool allocate_memory)
{
  if(allocate_memory)
  {
    {
      const Core::Memory::Chunk chunk = Core::Memory::request_chunk(LOGIC_POOL_NUMBER_OF_SCRIPTS * sizeof(Core::Entity_id));
      pool->entity_id = static_cast<Core::Entity_id*>(chunk.start_of_chunk);
    }
    
    {
      const Core::Memory::Chunk chunk = Core::Memory::request_chunk(LOGIC_POOL_NUMBER_OF_SCRIPTS * sizeof(uint32_t*));
      pool->regd_hook = static_cast<uint32_t*>(chunk.start_of_chunk);
    }
    
    {
      const Core::Memory::Chunk chunk = Core::Memory::request_chunk(LOGIC_POOL_NUMBER_OF_SCRIPTS * LOGIC_POOL_SIZE_MAX_SCRIPT_SIZE * sizeof(uint8_t));
      pool->object_store = static_cast<uint8_t*>(chunk.start_of_chunk);
    }
  }

  pool->size = 0;
}


namespace
{
  inline void*
  get_object_ptr(uint8_t *store,
                 const uint32_t index)
  {
    return reinterpret_cast<void*>(&store[index * LOGIC_POOL_SIZE_MAX_SCRIPT_SIZE]);
  }
}


void*
logic_pool_get_slot(Logic_pool *pool,
                    const Core::Entity_id id)
{
  if(pool->size >= LOGIC_POOL_NUMBER_OF_SCRIPTS)
  {
    return nullptr;
  }

  const uint32_t index = pool->size++;
  
  pool->entity_id[index] = id;
  
  // Add all hooks
  const uint32_t hooks = Logic_hook::on_start |
                         Logic_hook::on_early_update |
                         Logic_hook::on_update |
                         Logic_hook::on_end |
                         Logic_hook::on_collision |
                         Logic_hook::on_event;
  
  pool->regd_hook[index] = hooks;
  
  void *return_location = get_object_ptr(pool->object_store, index);
  return return_location;
}


uint32_t
logic_pool_get_slot_count(Logic_pool *pool, const Core::Entity_id id)
{
  uint32_t count(0);
  
  for(uint32_t i = 0; i < pool->size; ++i)
  {
    if(pool->entity_id[i] == id)
    {
      ++count;
    }
  }
  
  return count;
}


void
logic_pool_clean_up(Logic_pool *pool)
{
  // We loop through and find all flags that have been set to_destroy.
  // We will then shuffle that memory up.
  uint32_t i = 0;
  
  while(i < pool->size)
  {
    if(pool->regd_hook[i] & Logic_hook::to_destroy)
    {
      const uint32_t src = i + 1;
      const uint32_t size = pool->size - i - 1;
      
      memmove(&pool->entity_id[i], &pool->entity_id[src], size * sizeof(Core::Entity_id));
      memmove(&pool->regd_hook[i], &pool->regd_hook[src], size * sizeof(uint32_t));

      // Special case since we need to deal with bytes here.
      const uint32_t dest_byte = (i * LOGIC_POOL_SIZE_MAX_SCRIPT_SIZE);
      const uint32_t src_byte  = src * LOGIC_POOL_SIZE_MAX_SCRIPT_SIZE;
      const uint32_t size_byte = size * LOGIC_POOL_SIZE_MAX_SCRIPT_SIZE;
      
      memmove(&pool->object_store[dest_byte], &pool->object_store[src_byte], size_byte);
      
      --(pool->size);
    }
    else
    {
      ++i;
    }
  }
}


namespace
{
  inline void
  build_objs_list(uint32_t flags[],
                  const uint32_t size_of_flags,
                  uint8_t objects[],
                  const uint32_t size_of_objects,
                  const uint32_t flag_to_find,
                  uint32_t *out_size,
                  Core::Component *out_comp[],
                  const uint32_t size_of_out_comp,
                  const bool clear_flag = false)
  {
    *out_size = 0;
  
    for(uint32_t i = 0; i < size_of_flags; ++i)
    {
      if(flags[i] & flag_to_find || flag_to_find == 0)
      {
        assert(size_of_objects > i); // We assume its the same size as flags[]
        assert(size_of_out_comp > *out_size); // We assume its the same size as flags[]
      
        out_comp[*out_size] = reinterpret_cast<Core::Component*>(get_object_ptr(objects, i));
        ++(*out_size);
        
        if(clear_flag)
        {
          flags[i] &= ~(flag_to_find);
        }
      }
    }
  }
}


void
logic_pool_on_start_hook(Logic_pool *pool)
{
  uint32_t objs_found(0);
  Core::Component *comps[LOGIC_POOL_NUMBER_OF_SCRIPTS];

  build_objs_list(pool->regd_hook,
                  pool->size,
                  pool->object_store,
                  pool->size,
                  Logic_hook::on_start,
                  &objs_found,
                  comps,
                  LOGIC_POOL_NUMBER_OF_SCRIPTS,
                  true);
  
  // Call all the objects to start
  for(uint32_t i = 0; i < objs_found; ++i)
  {
    comps[i]->on_start();
  }
}


void
logic_pool_on_early_update_hook(Logic_pool *pool, const float delta_time)
{
  uint32_t objs_found(0);
  Core::Component *comps[LOGIC_POOL_NUMBER_OF_SCRIPTS];

  build_objs_list(pool->regd_hook,
                  pool->size,
                  pool->object_store,
                  pool->size,
                  Logic_hook::on_early_update,
                  &objs_found,
                  comps,
                  LOGIC_POOL_NUMBER_OF_SCRIPTS);
  
  // Call all the objects to start
  for(uint32_t i = 0; i < objs_found; ++i)
  {
    comps[i]->on_early_update(delta_time);
  }
}


void
logic_pool_on_update_hook(Logic_pool *pool, const float delta_time)
{
  uint32_t objs_found(0);
  Core::Component *comps[LOGIC_POOL_NUMBER_OF_SCRIPTS];

  build_objs_list(pool->regd_hook,
                  pool->size,
                  pool->object_store,
                  pool->size,
                  Logic_hook::on_update,
                  &objs_found,
                  comps,
                  LOGIC_POOL_NUMBER_OF_SCRIPTS);
  
  // Call all the objects to start
  for(uint32_t i = 0; i < objs_found; ++i)
  {
    comps[i]->on_update(delta_time);
  }
}


void
logic_pool_on_collision_hook(Logic_pool *pool, const Core::Entity_id id_a, const Core::Entity_id id_b)
{
  uint32_t objs_found(0);
  Core::Component *comps[LOGIC_POOL_NUMBER_OF_SCRIPTS];

  build_objs_list(pool->regd_hook,
                  pool->size,
                  pool->object_store,
                  pool->size,
                  Logic_hook::on_collision,
                  &objs_found,
                  comps,
                  LOGIC_POOL_NUMBER_OF_SCRIPTS);
  
  // Call all the objects to start
  for(uint32_t i = 0; i < objs_found; ++i)
  {
    if(comps[i]->get_entity().get_id() == id_a)
    {
      Core::Entity entity;
      World_data::world_find_entity(comps[i]->m_world_data, &entity, id_b);
      
      comps[i]->on_collision(entity);
    }
  }
}


void
logic_pool_on_end_hook(Logic_pool *pool,
                       const Core::Entity_id ids[],
                       const uint32_t number_of_entities)
{
  uint32_t objs_found(0);
  Core::Component *comps[LOGIC_POOL_NUMBER_OF_SCRIPTS];
  
  for(uint32_t i = 0; i < number_of_entities; ++i)
  {
    const Core::Entity_id id = ids[i];
  
    uint32_t found_index(0);
    uint32_t curr_index(0);
    while(Core::Entity_id_util::find_index_linearly(&found_index,
                                                    id,
                                                    &pool->entity_id[curr_index],
                                                    pool->size - curr_index))
    {
      const uint32_t index = found_index + curr_index;
      curr_index += found_index + 1;
    
      if(pool->regd_hook[index] & Logic_hook::on_end)
      {
        comps[objs_found++] = reinterpret_cast<Core::Component*>(get_object_ptr(pool->object_store, index));
        
        pool->regd_hook[index] = 0;                      // Hooks should no longer be called.
        pool->regd_hook[index] = Logic_hook::to_destroy; // Flags this as space to be destroyed.
      }
    }
  }
  
  // Call all the objects to start
  for(uint32_t k = 0; k < objs_found; ++k)
  {
    comps[k]->on_end();
  }
}


void
logic_pool_on_event(Logic_pool *pool,
                    const Core::Entity_id id,
                    const uint32_t event_id,
                    const void *data,
                    const uint32_t size_of_data)
{
  uint32_t objs_found(0);
  Core::Component *comps[LOGIC_POOL_NUMBER_OF_SCRIPTS];

  build_objs_list(pool->regd_hook,
                  pool->size,
                  pool->object_store,
                  pool->size,
                  Logic_hook::on_event,
                  &objs_found,
                  comps,
                  LOGIC_POOL_NUMBER_OF_SCRIPTS);
  
  // Call all the objects to start
  for(uint32_t i = 0; i < objs_found; ++i)
  {
    if(comps[i]->get_entity().get_id() == id)
    {
      comps[i]->on_event(event_id, data, size_of_data);
    }
  }
}


const char *
debug_info(const Logic_pool *pool)
{
  // Get debug info.
  uint32_t number_pending_destruction(0);
  {
    for(uint32_t i = 0; i < pool->size; ++i)
    {
      if(pool->regd_hook[i] & Logic_hook::to_destroy)
      {
        ++number_pending_destruction;
      }
    }
  }

  static std::string debug_info;

  std::stringstream ss;
  ss << "Logic Pool Debug Info \n";
  ss << "--------------------- \n";
  ss << "size: " << pool->size << "\n";
  ss << "capacity: " << pool->capacity << "\n";
  ss << "pending destruction: " << number_pending_destruction << "\n";
  
  debug_info = ss.str();
  
  return debug_info.c_str();
}


} // ns