#include "logic_pool.hpp"
#include <cstring>
#include <assert.h>
#include <core/interface/component.hpp>

namespace Data {


void
logic_pool_init(Logic_pool *pool)
{
  memset(pool->storage, 0, sizeof(pool->storage));
  memset(pool->entity_id, 0, sizeof(pool->entity_id));
  memset(pool->objects_in_use, 0, sizeof(pool->objects_in_use));
  memset(pool->storage, 0, sizeof(pool->storage));
  
  pool->objects_in_use_size = 0;
  
  // Create free list
  for(std::size_t i = 0; i < pool->size; ++i)
  {
    pool->object_locations[i] = &pool->storage[i * pool->storage_size];
  }
}


void*
logic_pool_get_slot(Logic_pool *pool,
                    const Core::Entity_id id)
{
  size_t index;
  if(Core::Entity_id_util::find_index_linearly(&index,
                                               Core::Entity_id_util::invalid_id(),
                                               pool->entity_id,
                                               pool->size))
  {
    pool->entity_id[index] = id;
    
    void *result = pool->object_locations[index];
    
    // Push object onto list
    {
      pool->objects_in_use[pool->objects_in_use_size++] = result;
      
      const size_t new_on_start = pool->objects_on_start_pending_hooks_size;
      pool->objects_on_start_pending_hooks[new_on_start] = result;
      
      pool->objects_on_start_pending_hooks_size++;
    }
    
    return result;
  }
  
  
  return nullptr;
}


size_t
logic_pool_get_slot_count(Logic_pool *pool, const Core::Entity_id id)
{
  size_t count(0);
  
  for(size_t i = 0; i < pool->size; ++i)
  {
    if(pool->entity_id[i] == id)
    {
      ++count;
    }
  }
  
  return count;
}


void
logic_pool_free_slots(Logic_pool *pool, const Core::Entity_id id)
{
  // Search the entity list and find objects to removes
  const size_t slots_to_remove = logic_pool_get_slot_count(pool, id);
  
  for(size_t i = 0; i < slots_to_remove; ++i)
  {
    size_t index;
    if(Core::Entity_id_util::find_index_linearly(&index, id, pool->entity_id, pool->size))
    {
      // Remove this logic.
      pool->entity_id[index] = Core::Entity_id_util::invalid_id();
      auto obj_to_remove = pool->object_locations[index];
      reinterpret_cast<Core::Component*>(obj_to_remove)->~Component();
      pool->object_locations[index] = nullptr;
      
      // Remove from objects in use.
      size_t o = 0;
      while(o < pool->objects_in_use_size)
      {
        if(pool->objects_in_use[o] == obj_to_remove)
        {
          void* start       = &pool->objects_in_use[o];
          const void* end   = &pool->objects_in_use[o+1];
          const size_t size = (pool->size-o-1) * sizeof(*pool->objects_in_use);
        
          --(pool->objects_in_use_size);
        
          memmove(start, end, size);
          continue;
        }
        
        ++o;
      }
      
      // Check pending on start just incase.
      
    }
    else
    {
      assert(false); // Why did the count give us something different?
    }
  }
}


void
logic_pool_on_start_hook(Logic_pool *pool)
{
  const size_t pending = pool->objects_on_start_pending_hooks_size;

  if(pending)
  {
    for(size_t i = 0; i < pending; ++i)
    {
      auto obj = pool->objects_on_start_pending_hooks[i];
      reinterpret_cast<Core::Component*>(obj)->on_start();
    }
  }
  
  pool->objects_on_start_pending_hooks_size = 0;
}


void
logic_pool_on_update_hook(Logic_pool *pool, const float delta_time)
{
  const size_t pending = pool->objects_in_use_size;

  if(pending)
  {
    for(size_t i = 0; i < pending; ++i)
    {
      auto obj = pool->objects_in_use[i];
      reinterpret_cast<Core::Component*>(obj)->on_update(delta_time);
    }
  }
}


} // ns