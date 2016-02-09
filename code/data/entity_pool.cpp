#include "entity_pool.hpp"
#include <assert.h>
#include <stddef.h>


namespace Data {


void
entity_pool_init(Entity_pool *pool)
{
  memset(pool->entity_id,          0, sizeof(pool->entity_id));
  memset(pool->parent_id,          0, sizeof(pool->parent_id));
  memset(pool->entity_properties,  0, sizeof(pool->entity_properties));
  memset(pool->model,              0, sizeof(pool->model));
  memset(pool->display,            0, sizeof(pool->display));
  memset(pool->texture,            0, sizeof(pool->texture));
  memset(pool->transform,          0, sizeof(pool->transform));
  memset(pool->rigidbody_property, 0, sizeof(pool->rigidbody_property));
}


void
entity_pool_de_init(Entity_pool *pool)
{
  assert(pool);
  
  for(size_t i = 0; i < pool->size; ++i)
  {
    if(pool->entity_id[i] != Core::Entity_id_util::invalid_id())
    {
      // TODO: Re add this when more scene graph things in.
      //assert(false);
    }
  }
}


bool
entity_pool_push_new_entity(Entity_pool *pool, const Core::Entity_id id)
{
  size_t search_index;
  if(Core::Entity_id_util::find_index_linearly(&search_index,
                                               Core::Entity_id_util::invalid_id(),
                                               pool->entity_id,
                                               pool->size))
  {
    pool->entity_id[search_index] = id;
    
    return true;
  }

  assert(false); // uhuo
  return false;
}


bool
entity_pool_remove_entity(Entity_pool *pool, const Core::Entity_id id)
{
  return true;
}


} // ns