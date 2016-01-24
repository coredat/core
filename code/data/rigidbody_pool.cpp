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
    if(!::Entity::find_index_linearly(&index, ::Entity::invalid_id(), update_pool->entity_id, update_pool->size))
    {
      // Failed to find space.
      assert(false);
      return;
    }
    
    update_pool->entity_id[index] = id;
  }
  
  auto *rb_update = &update_pool->rb_updates[index];
  
  rb_update->parent_id     = ::Entity::invalid_id();
  rb_update->collider_info = collider;
  rb_update->properties    = props;
  
  update_pool->size++;
}


void
rigidbody_pool_process_updates(Physics::World *phy_world, Data::World *data, Rigidbody_update_pool *update_pool, Rigidbody_pool *rb_pool)
{
  for(size_t i = 0; i < update_pool->size; ++i)
  {
    auto rb = &update_pool[i];
  
    // Find entity in rb_pool
    size_t index;
    if(!::Entity::find_index_linearly(&index, update_pool->entity_id[i], rb_pool->entity_id, rb_pool->size))
    {
      ::Entity::find_index_linearly(&index, ::Entity::invalid_id(), rb_pool->entity_id, rb_pool->size);
    }
    
    // Create motion
    rb_pool[index].rigidbody->motion_state.reset(new Physics::Motion_state(update_pool->entity_id[i], data->entity_pool));
    
    // Create collider.
    Physics::colliders_generate(&rb->rb_updates->collider_info, 1, rb_pool[index].rigidbody, 1);
    
    // Add to world.
    Physics::world_add_rigidbodies(phy_world, &rb->rb_updates->properties, 1, &rb_pool->rigidbody[index], 1);
  }
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
  
  Physics::world_remove_rigidbody(world, &pool->rigidbody[index]);
  
  pool->rigidbody[index].motion_state.reset(new Physics::Motion_state(id, data->entity_pool));
  
  // Generate collider
  Physics::colliders_generate(&collider, 1, &pool->rigidbody[index], 1);
  
  // Add rigidbody
  Physics::world_add_rigidbodies(world, &props, 1, &pool->rigidbody[index], 1);
}




} // ns