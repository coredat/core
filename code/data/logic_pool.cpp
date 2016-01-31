#include "logic_pool.hpp"
#include <cstring>


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
                    const ::Entity::Entity_id id)
{
  size_t index;
  if(::Entity::find_index_linearly(&index,
                                   ::Entity::invalid_id(),
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