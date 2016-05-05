#include "graph_change_pool.hpp"
#include <assert.h>
#include <cstring>


namespace World_data {


void
entity_graph_change_pool_init(Entity_graph_changes_pool *pool)
{
  memset(pool->entity_event, 0, sizeof(pool->entity_event));
  pool->size = 0;
}


void
entity_graph_change_push(Entity_graph_changes_pool *pool,
                         const util::generic_id entity_id,
                         Entity_graph_change change_type)
{
  assert(pool && pool->size < pool->capacity);
  
  // Need to check to see if the entity has already
  // been pushed.
  {
    for(uint32_t i = 0; i < pool->size; ++i)
    {
      if(pool->entity_event[i].entity_id == entity_id &&
         pool->entity_event[i].change_type == change_type)
       {
        return;
       }
    }
  }
  
  pool->entity_event[pool->size].entity_id = entity_id;
  pool->entity_event[pool->size].change_type = change_type;
  
  ++(pool->size);
}


} // ns