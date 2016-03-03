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
  #endif
}


void
entity_pool_de_init(Entity_pool *pool)
{
  assert(pool);
  
  for(uint32_t i = 0; i < pool->size; ++i)
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
  // when processing the data, but take a hit here.1
  uint32_t remove_id;
  if(Core::Entity_id_util::find_index_linearly(&remove_id,
                                               id,
                                               pool->entity_id,
                                               pool->size))
  {
    const uint32_t start_move = remove_id + 1;
    const uint32_t end_move = pool->size - remove_id - 1;
  
    memmove(&pool->entity_id[remove_id],          &pool->entity_id[start_move],          end_move * sizeof(*pool->entity_id));
    memmove(&pool->parent_id[remove_id],          &pool->parent_id[start_move],          end_move * sizeof(*pool->parent_id));
    memmove(&pool->entity_properties[remove_id],  &pool->entity_properties[start_move],  end_move * sizeof(*pool->entity_properties));
    memmove(&pool->model[remove_id],              &pool->model[start_move],              end_move * sizeof(*pool->model));
    memmove(&pool->texture[remove_id],            &pool->texture[start_move],            end_move * sizeof(*pool->texture));
    memmove(&pool->transform[remove_id],          &pool->transform[start_move],          end_move * sizeof(*pool->transform));
    
    --(pool->size);
    
    return true;
  }

  assert(false); // uhuo
  return false;
}


} // ns