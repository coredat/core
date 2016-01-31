#include "entity_pool.hpp"


namespace Data {


void
entity_pool_init(Entity_pool *pool)
{
  memset(pool->entity_id,          0, sizeof(pool->entity_id));
  memset(pool->parent_id,          0, sizeof(pool->parent_id));
  memset(pool->model,              0, sizeof(pool->model));
  memset(pool->display,            0, sizeof(pool->display));
  memset(pool->texture,            0, sizeof(pool->texture));
  memset(pool->transform,          0, sizeof(pool->transform));
  memset(pool->rigidbody_property, 0, sizeof(pool->rigidbody_property));
}


void
entity_graph_change_pool_init(Entity_graph_changes_pool *pool)
{
  memset(pool->entity_event, 0, sizeof(pool->entity_event));
  pool->size = 0;
}



void
entity_graph_change_push(Entity_graph_changes_pool *pool, const Core::Entity_id entity_id, Entity_graph_change change_type)
{
  assert(pool && pool->size < pool->capacity);
  
  pool->entity_event[pool->size].entity_id = entity_id;
  pool->entity_event[pool->size].change_type = change_type;
  
  ++(pool->size);
}



} // ns