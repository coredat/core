#include "rigidbody_pool.hpp"
#include "world_data.hpp"
#include <systems/physics/world.hpp>


namespace Data {


void
rigidbody_pool_init(Rigidbody_pool *pool)
{
  memset(pool->entity_id, 0, sizeof(pool->entity_id));
  memset(pool->rigidbody, 0, sizeof(pool->rigidbody));
}


void
rigidbody_pool_update_rb(Rigidbody_pool *pool,
                         const ::Entity::Entity_id id,
                         Physics::World *world,
                         Data::World *data,
                         const Physics::Rigidbody_properties &props,
                         const Physics::Rigidbody_collider &collider)
{
  // Find index or a new space.
  size_t index;
  if(!::Entity::find_index_linearly(&index, id, pool->entity_id, pool->size))
  {
    ::Entity::find_index_linearly(&index, ::Entity::invalid_id(), pool->entity_id, pool->size);
    pool->entity_id[index] = id;
  }
  
  pool->rigidbody[index].motion_state.reset(new Physics::Motion_state(id, data->entity_pool));
  
  // Generate collider
  Physics::colliders_generate(&collider, 1, &pool->rigidbody[index], 1);
  
  // Add rigidbody
  Physics::world_add_rigidbodies(world, &props, 1, &pool->rigidbody[index], 1);
}




} // ns