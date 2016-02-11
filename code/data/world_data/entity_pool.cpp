#include "entity_pool.hpp"
#include <assert.h>
#include <stddef.h>
#include <vector>


namespace World_data {


void
entity_pool_init(Entity_pool *pool)
{
  #ifndef NDEBUG
  memset(pool->entity_id,          0, sizeof(pool->entity_id));
  memset(pool->parent_id,          0, sizeof(pool->parent_id));
  memset(pool->entity_properties,  0, sizeof(pool->entity_properties));
  memset(pool->model,              0, sizeof(pool->model));
  memset(pool->texture,            0, sizeof(pool->texture));
  memset(pool->transform,          0, sizeof(pool->transform));
  memset(pool->rigidbody_property, 0, sizeof(pool->rigidbody_property));
  #endif
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
  if(pool->size < pool->capacity)
  {
    pool->entity_id[pool->size] = id;
    ++(pool->size);
    
    return true;
  }

  // Full capacity.
  assert(false);
  return false;
}


bool
entity_pool_remove_entity(Entity_pool *pool, const Core::Entity_id id)
{
  // We move down all the elements in the data down one.
  // This way we can keep fragmentation and cache misses out
  // when processing the data.
  size_t remove_id;
  if(Core::Entity_id_util::find_index_linearly(&remove_id,
                                               id,
                                               pool->entity_id,
                                               pool->size))
  {
    memmove(&pool->entity_id[remove_id],          &pool->entity_id[remove_id + 1],          (pool->size - remove_id - 1) * sizeof(*pool->entity_id));
    memmove(&pool->parent_id[remove_id],          &pool->parent_id[remove_id + 1],          (pool->size - remove_id - 1) * sizeof(*pool->parent_id));
    memmove(&pool->entity_properties[remove_id],  &pool->entity_properties[remove_id + 1],  (pool->size - remove_id - 1) * sizeof(*pool->entity_properties));
    memmove(&pool->model[remove_id],              &pool->model[remove_id + 1],              (pool->size - remove_id - 1) * sizeof(*pool->model));
    memmove(&pool->texture[remove_id],            &pool->texture[remove_id + 1],            (pool->size - remove_id - 1) * sizeof(*pool->texture));
    memmove(&pool->transform[remove_id],          &pool->transform[remove_id + 1],          (pool->size - remove_id - 1) * sizeof(*pool->transform));
    memmove(&pool->rigidbody_property[remove_id], &pool->rigidbody_property[remove_id + 1], (pool->size - remove_id - 1) * sizeof(*pool->rigidbody_property));
    
    return true;
  }

  assert(false); // uhuo
  return false;
}


} // ns