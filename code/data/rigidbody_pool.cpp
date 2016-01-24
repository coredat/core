#include "rigidbody_pool.hpp"
#include "world_data.hpp"
#include "entity_pool.hpp"
#include <systems/physics/world.hpp>


namespace Data {


void
rigidbody_pool_init(Rigidbody_pool *pool)
{
  memset(pool->entity_id, 0, sizeof(pool->entity_id));
  memset(pool->rigidbody, 0, sizeof(pool->rigidbody));
}


void
rigidbody_update_pool_init(Rigidbody_update_pool *pool)
{
  memset(pool->entity_id, 0, sizeof(pool->entity_id));
  memset(pool->rb_updates, 0, sizeof(pool->rb_updates));
  pool->size = 0;
}


void
rigidbody_update_pool_clear(Rigidbody_update_pool *pool)
{
  // Only clear out what was used in the pool
  memset(pool->entity_id, 0, sizeof(pool->entity_id[0]) * pool->size);
  memset(pool->rb_updates, 0, sizeof(pool->rb_updates[0]) * pool->size);
  pool->size = 0;
}


bool
rigidbody_update_pool_add_update(Rigidbody_update_pool *update_pool,
                                 const ::Entity::Entity_id id,
                                 const Physics::Rigidbody_collider collider,
                                 const Physics::Rigidbody_properties props)
{
  // Search to see if an id has already been pushed.
  // Else find an empty slot.
  size_t index;
  if(!::Entity::find_index_linearly(&index, id, update_pool->entity_id, update_pool->size))
  {
    if(!::Entity::find_index_linearly(&index, ::Entity::invalid_id(), update_pool->entity_id, update_pool->capacity))
    {
      // Failed to find space.
      assert(false);
      return false;
    }
    update_pool->size++;
    update_pool->entity_id[index] = id;
  }
  
  auto *rb_update = &update_pool->rb_updates[index];
  
  rb_update->parent_id     = ::Entity::invalid_id();
  rb_update->collider_info = collider;
  rb_update->properties    = props;
  
  return true;
}


void
rigidbody_pool_process_updates(Physics::World *phy_world,
                               Data::World *data,
                               Rigidbody_update_pool *update_pool,
                               Rigidbody_pool *rb_pool)
{
  for(size_t i = 0; i < update_pool->size; ++i)
  {
    auto rb = &update_pool->rb_updates[i];
  
    // Find entity in rb_pool
    size_t index;
    if(!::Entity::find_index_linearly(&index, update_pool->entity_id[i], rb_pool->entity_id, rb_pool->size))
    {
      ::Entity::find_index_linearly(&index, ::Entity::invalid_id(), rb_pool->entity_id, rb_pool->size);
      rb_pool->entity_id[index] = update_pool->entity_id[i];
    }
    
    // Create motion
    rb_pool->rigidbody[index].motion_state.reset(new Physics::Motion_state(update_pool->entity_id[i], data->entity_pool));
    
    // Create collider.
    Physics::colliders_generate(&rb->collider_info, 1, &rb_pool->rigidbody[index], 1);
    
    // Add to world.
    
    Physics::world_add_rigidbodies(phy_world, &rb->properties, 1, &rb_pool->rigidbody[index], 1);
  }
}


} // ns