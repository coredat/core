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


bool
rigidbody_pool_find(Rigidbody_pool *pool,
                    const ::Entity::Entity_id id,
                    Physics::Rigidbody *out_rb)
{
  size_t index;
  if(::Entity::find_index_linearly(&index, id, pool->entity_id, pool->size))
  {
    out_rb = &pool->rigidbody[index];
    return true;
  }
  
  return false;
}


bool
rigidbody_pool_remove(Rigidbody_pool *pool,
                      const ::Entity::Entity_id id)
{
  size_t index;
  if(::Entity::find_index_linearly(&index, id, pool->entity_id, pool->size))
  {
    pool->entity_id[index] = ::Entity::invalid_id();
    return true;
  }
  
  return false;
}


bool
rigidbody_pool_exists(Rigidbody_pool *pool,
                      const ::Entity::Entity_id id)
{
  size_t index;
  return ::Entity::find_index_linearly(&index, id, pool->entity_id, pool->size);
}


bool
rigidbody_pool_push(Rigidbody_pool *pool,
                    const ::Entity::Entity_id id,
                    Physics::Rigidbody *out_rb)
{
  // This is there I am at.
  
  return "Don't return a string you fool!"
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
    Physics::Rigidbody* rb = nullptr;
    if(rigidbody_pool_find(world_data->rigidbody_pool, graph_changes->entity_event[i].entity_id, rb) && rb)
    {
      Physics::world_remove_rigidbody(world_data->physics_world, rb);
      rigidbody_pool_remove(world_data->rigidbody_pool, graph_changes->entity_event[i].entity_id);
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
  
  // Remove all entities attached to these parents. TODO just going to remove parents atm.
  // TODO: Didn't we just do removals huh?
  for(size_t i = 0; i < ent_count; ++i)
  {
    Entity parent = ent[i];
    
    // Find index in rb and remove it.
    size_t index;
    if(::Entity::find_index_linearly(&index, parent.get_id(), world_data->rigidbody_pool->entity_id, world_data->rigidbody_pool->size))
    {
      Physics::world_remove_rigidbody(world_data->physics_world, &world_data->rigidbody_pool->rigidbody[index]);
      world_data->rigidbody_pool->entity_id[index] = ::Entity::invalid_id();
    }
  }
  
  // Build colliders and insert into world.
  // TODO: Maybe have to split this up into two steps, would be better cache stoof.
  for(size_t i = 0; i < ent_count; ++i)
  {
    Entity entity = ent[i];
    
    auto rb_collider = entity.get_rigidbody_collider();
    
    
    if(rb_collider.collider_type == Physics::Collider_type::none)
    {
      continue;
    }
    
    // Search to see if we already added it.
    {
      if(rigidbody_pool_exists(world_data->rigidbody_pool, entity.get_id()))
      {
        continue;
      }
    }
  
    // Get an empty slot in rb.

    size_t index;
    assert(::Entity::find_index_linearly(&index, ::Entity::invalid_id(), world_data->rigidbody_pool->entity_id, world_data->rigidbody_pool->size));
    Physics::world_remove_rigidbody(world_data->physics_world, &world_data->rigidbody_pool->rigidbody[index]);
    world_data->rigidbody_pool->entity_id[index] = entity.get_id();
    
    // Create required thingies.
    auto rb = &world_data->rigidbody_pool->rigidbody[index];
    auto rb_props = entity.get_rigidbody_properties();

    rb->motion_state.reset(new Physics::Motion_state(entity.get_id(), world_data->entity_pool));
    
    Physics::colliders_generate(&rb_collider, 1, rb, 1);
    Physics::world_add_rigidbodies(world_data->physics_world, &rb_props, 1, rb, 1);
  }
}


} // ns