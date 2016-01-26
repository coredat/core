#include "rigidbody_pool.hpp"
#include "world_data.hpp"
#include "entity.hpp"
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
                                 const ::Entity::Entity_id parent_id,
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
  
  rb_update->parent_id     = parent_id;
  rb_update->collider_info = collider;
  rb_update->properties    = props;
  
  return true;
}


void
rigidbody_pool_update_scene_graph_changes(Rigidbody_pool *pool,
                                          Data::World *world_data,
                                          const Entity_graph_changes_pool *graph_changes)
{
  // TODO: Keep an eye on this I can't imagine this is partcularly fast way of doing things.
  // But we do feesibly work with small amounts of data.
  
  // Remove all the rbs that exist **TODO** Change this to remove all graph removals.
  for(size_t i = 0; i < graph_changes->size; ++i)
  {
    size_t index;
    if(::Entity::find_index_linearly(&index,
                                     graph_changes->entity_event[i].entity_id,
                                     world_data->entity_pool->entity_id,
                                     world_data->entity_pool->size))
    {
      Physics::world_remove_rigidbody(world_data->physics_world, &pool->rigidbody[index]);
      pool->entity_id[index] = ::Entity::invalid_id();
    }
  }
  
  // Get a list of Entities that have not been removed.
  // And have no parents
  Data::Entity ent[128];
  size_t ent_count(0);
  
  for(size_t i = 0; i < graph_changes->size; ++i)
  {
    const auto graph_change = graph_changes->entity_event[i];
    
    if(graph_change.change_type != Data::Entity_graph_change::removed)
    {
      Data::Entity entity;
      Data::world_find_entity(world_data, &entity, graph_change.entity_id);
      
      // Is valid? Then get the top most entity.
      if(entity.is_valid())
      {
        while(entity.get_parent().get_id() != ::Entity::invalid_id())
        {
          entity = entity.get_parent();
        }
        
        ent[ent_count++] = entity;
      }
    }
  }
  
  // Remove all entities attached to these parents.
  {
  
  }
  
  // Build colliders etc.
  {
  
  }
  
  // Add the rigidbodies to the world.
  {
  
  }
}


void
rigidbody_pool_process_updates(Physics::World *phy_world,
                               Data::World *data,
                               Rigidbody_update_pool *update_pool,
                               Rigidbody_pool *rb_pool)
{
  // Create motion states, colliders.
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
    
    Physics::world_remove_rigidbody(phy_world, &rb_pool->rigidbody[index]);
    
    // Create motion
    rb_pool->rigidbody[index].motion_state.reset(new Physics::Motion_state(update_pool->entity_id[i], data->entity_pool));
    
    // Create collider.
    Physics::colliders_generate(&rb->collider_info, 1, &rb_pool->rigidbody[index], 1);
  }
  
  // Attach all colliders to their parents.
  for(size_t i = 0; i < update_pool->size; ++i)
  {
    auto rb = &update_pool->rb_updates[i];
    
    if(rb->parent_id != ::Entity::invalid_id())
    {
      // Get index of output
      size_t child_index;
      assert(::Entity::find_index_linearly(&child_index, update_pool->entity_id[i], rb_pool->entity_id, rb_pool->size));
    
      size_t parent_index;
      assert(::Entity::find_index_linearly(&parent_index, rb->parent_id, rb_pool->entity_id, rb_pool->size));
      
      btTransform trans;
      trans.setIdentity();
      
      btVector3 offset(1,1,1);
      trans.setOrigin(offset);
      
      rb_pool->rigidbody[parent_index].compound_shape->addChildShape(trans,
                                                                     rb_pool->rigidbody[child_index].shape.get());
    }
    
  }
  
  for(size_t i = 0; i < update_pool->size; ++i)
  {
    auto rb = &update_pool->rb_updates[i];
  
    size_t index;
    assert(::Entity::find_index_linearly(&index, update_pool->entity_id[i], rb_pool->entity_id, rb_pool->size));

    if(rb->parent_id == ::Entity::invalid_id())
    {
      // Add to world.
      Physics::world_add_rigidbodies(phy_world, &rb->properties, 1, &rb_pool->rigidbody[index], 1);
    }
  }
}


} // ns