#include "logic_pool.hpp"
#include <cstring>
#include <assert.h>
#include <core/interface/component.hpp>
#include <core/interface/entity.hpp>
#include <data/world_data/world_data.hpp>
#include <core/memory/memory.hpp>


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
  
  // Add all hooks
  const uint32_t hooks = Logic_hook::on_start |
                         Logic_hook::on_early_update |
                         Logic_hook::on_update |
                         Logic_hook::on_end;
  
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
logic_pool_free_slots(Logic_pool *pool,
                      const Core::Entity_id ids[],
                      const uint32_t number_of_entities)
{
  // Remove all the entities from the pool
  // We will call on_end if it is subscribed.
  for(uint32_t i = 0; i < number_of_entities; ++i)
  {
    uint32_t index(0);
    if(Core::Entity_id_util::find_index_linearly(&index,
                                                 Core::Entity_id_util::invalid_id(),
                                                 pool->entity_id,
                                                 pool->size))
    {
      const uint32_t start_move = index + 1;
      const uint32_t end_move = pool->size - index - 1;
    
      memmove(&pool->entity_id[index],      &pool->entity_id[start_move],    end_move * sizeof(*pool->entity_id));
      memmove(&pool->regd_hook[index],      &pool->regd_hook[start_move],    end_move * sizeof(*pool->regd_hook));
      
      // Special case since this is just a storeage.
      const uint32_t dest_index = index * LOGIC_POOL_SIZE_MAX_SCRIPT_SIZE;
      const uint32_t src_index = (index + 1) * LOGIC_POOL_SIZE_MAX_SCRIPT_SIZE;
      const size_t count = src_index * sizeof(uint8_t) * LOGIC_POOL_SIZE_MAX_SCRIPT_SIZE;
      
      memmove(&pool->object_store[dest_index],
              &pool->object_store[src_index],
              count);
      
      --pool->size;
    }
  }
}


void
logic_pool_on_start_hook(Logic_pool *pool)
{
//  const uint32_t pending = pool->objects_on_start_pending_hooks_size;
//  
//  void **pending_objs[128];
//  memcpy(pending_objs, &pool->objects_on_start_pending_hooks_size, sizeof(void*) * pending);
//  
//  pool->objects_on_start_pending_hooks_size = 0;  
//
//  for(uint32_t i = 0; i < pending; ++i)
//  {
//    auto obj = pool->objects_on_start_pending_hooks[i];
//    reinterpret_cast<Core::Component*>(obj)->on_start();
//  }
}


void
logic_pool_on_early_update_hook(Logic_pool *pool, const float delta_time)
{
//  const uint32_t pending = pool->objects_in_use_size;
//
//  if(pending)
//  {
//    for(uint32_t i = 0; i < pending; ++i)
//    {
//      auto obj = pool->objects_in_use[i];
//      reinterpret_cast<Core::Component*>(obj)->on_early_update(delta_time);
//    }
//  }
}


void
logic_pool_on_update_hook(Logic_pool *pool, const float delta_time)
{
//  const uint32_t pending = pool->objects_in_use_size;
//
//  if(pending)
//  {
//    for(uint32_t i = 0; i < pending; ++i)
//    {
//      auto obj = pool->objects_in_use[i];
//      reinterpret_cast<Core::Component*>(obj)->on_update(delta_time);
//    }
//  }
}


void
logic_pool_on_collision_hook(Logic_pool *pool, const Core::Entity_id id_a, const Core::Entity_id id_b)
{
//  // Find the entity.
//  uint32_t index(0);
//  uint32_t search_from(0);
//  
//  // Could be mutliple components attached to the same entity.
//  while(Core::Entity_id_util::find_index_linearly(&index,
//                                                  id_a,
//                                                  &(pool->entity_id[search_from]),
//                                                  pool->objects_in_use_size - search_from))
//  {
//    index += search_from;
//  
//    auto obj = reinterpret_cast<Core::Component*>(pool->object_locations[index]);
//    
//    // Create the entity.
//    Core::Entity collision;
//    World_data::world_find_entity(obj->m_world_data, &collision, id_b);
//    
//    obj->on_collision(collision);
//  
//    search_from = index + 1;
//  }
//  
}


void
logic_pool_on_end_hook(Logic_pool *pool)
{
  
}


} // ns