#include "world_data.hpp"
#include "entity_pool.hpp"
#include "pending_rigidbody_pool.hpp"
#include "entity.hpp"

#include <systems/physics/physics.hpp>


namespace Data {


Entity
world_create_new_entity(World *world_data, const uint32_t type_id)
{
  Entity new_entity(type_id, world_data);
  
  // Find empty index.
  size_t empty_index;
  ::Entity::find_index_linearly(&empty_index,
                                ::Entity::invalid_id(),
                                world_data->entity_pool->entity_id,
                                world_data->entity_pool->size);
  
  world_data->entity_pool->entity_id[empty_index] = new_entity.get_id();
  
  return new_entity;
}


} // ns