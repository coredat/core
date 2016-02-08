#include "rigidbody_pool.hpp"
#include "world_data.hpp"
#include "entity_pool.hpp"
#include <core/interface/entity.hpp>
#include <systems/physics/world.hpp>
#include <functional>


namespace Data {


void
rigidbody_pool_init(Rigidbody_pool *pool)
{
  memset(pool->entity_id, 0, sizeof(pool->entity_id));
  memset(pool->rigidbody, 0, sizeof(pool->rigidbody));
}


bool
rigidbody_pool_find(Rigidbody_pool *pool,
                    const Core::Entity_id id,
                    Physics::Rigidbody **out_rb)
{
  size_t index;
  if(Core::Entity_id_util::find_index_linearly(&index, id, pool->entity_id, pool->size))
  {
    *out_rb = &(pool->rigidbody[index]);
    return true;
  }
  
  return false;
}


bool
rigidbody_pool_remove(Rigidbody_pool *pool,
                      const Core::Entity_id id)
{
  size_t index;
  if(Core::Entity_id_util::find_index_linearly(&index, id, pool->entity_id, pool->size))
  {
    pool->entity_id[index] = Core::Entity_id_util::invalid_id();
    return true;
  }
  
  return false;
}


bool
rigidbody_pool_exists(Rigidbody_pool *pool,
                      const Core::Entity_id id)
{
  size_t index;
  return Core::Entity_id_util::find_index_linearly(&index, id, pool->entity_id, pool->size);
}


bool
rigidbody_pool_push(Rigidbody_pool *pool,
                    const Core::Entity_id id,
                    Physics::Rigidbody **new_rb)
{
  // This is there I am at.
  size_t index;
  if(Core::Entity_id_util::find_index_linearly(&index, Core::Entity_id_util::invalid_id(), pool->entity_id, pool->size))
  {
    pool->entity_id[index] = id;
    *new_rb = &pool->rigidbody[index];
    
    return true;
  }
  
  return false;
}


namespace
{
  void
  add_child_colliders(Data::World *world_data, Core::Entity e, btCompoundShape *parent_compound)
  {  
    for(size_t c = 0; c < e.get_number_of_children(); ++c)
    {
      Core::Entity child = e.get_child(c);
    
      add_child_colliders(world_data, child, parent_compound);
    }
    
    auto parent = e.get_parent();
    
    if(parent.is_valid())
    {
      auto parent_trans = parent.get_transform().position;
      auto child_trans = e.get_transform().position;
      auto pos = math::vec3_subtract(child_trans, parent_trans);
    
      btTransform transform;
      transform.setIdentity();
      transform.setOrigin(btVector3(math::vec3_get_x(pos), math::vec3_get_y(pos), math::vec3_get_z(pos)));

      //assert(false);
      // Need to get new transform.
      // Of the nested entity.
      
      Physics::Rigidbody *rb = nullptr;
      rigidbody_pool_find(world_data->rigidbody_pool, e.get_id(), &rb);
    
      parent_compound->addChildShape(transform, rb->shape.get());
    }
  };
}


void
rigidbody_pool_update_scene_graph_changes(Rigidbody_pool *pool,
                                          Data::World *world_data,
                                          const Entity_graph_changes_pool *graph_changes)
{
  // TODO: Keep an eye on this I can't imagine this is partcularly fast way of doing things.
  // But we do feesibly work with small amounts of data.
  
  // Get a list of Entities that have not been removed.
  // And have no parents
  Core::Entity ent[128];
  size_t ent_count(0);
  
  for(size_t i = 0; i < graph_changes->size; ++i)
  {
    const auto graph_change = graph_changes->entity_event[i];
    
    Core::Entity entity;
    Data::world_find_entity(world_data, &entity, graph_change.entity_id);
    
    if(graph_change.change_type != Data::Entity_graph_change::removed)
    {
      // Is valid? Then get the top most entity.
      if(entity.is_valid())
      {
//        while(entity.get_parent().get_id() != ::Entity::invalid_id())
//        {
//          entity = entity.get_parent();
//        }
        
        ent[ent_count++] = entity;
      }
    }
    else
    {
      Physics::Rigidbody *rb = nullptr;
      rigidbody_pool_find(world_data->rigidbody_pool, entity.get_id(), &rb);
      
      if(rb)
      {
        Physics::world_remove_rigidbody(world_data->physics_world, rb);
        rigidbody_pool_remove(world_data->rigidbody_pool, entity.get_id());
      }
    }
  }
  
  // Remove all entities attached to these parents. TODO just going to remove parents atm.
  // TODO: Didn't we just do removals huh?
  for(size_t i = 0; i < ent_count; ++i)
  {
    Core::Entity parent = ent[i];
    
    // Find index in rb and remove it.
    
    Physics::Rigidbody *rb = nullptr;
    rigidbody_pool_find(world_data->rigidbody_pool, parent.get_id(), &rb);
    
    if(rb)
    {
      Physics::world_remove_rigidbody(world_data->physics_world, rb);
      rigidbody_pool_remove(world_data->rigidbody_pool, parent.get_id());
    }
  }
  
  // Build colliders.
  for(size_t i = 0; i < ent_count; ++i)
  {
    Core::Entity entity = ent[i];
    
    auto rb_collider = entity.get_rigidbody_collider();
    
    // Legit collider?
    if(rb_collider.collider_type == Physics::Collider_type::none)
    {
      continue;
    }
    
    // Not a duplicate is it? Hate those!
    if(rigidbody_pool_exists(world_data->rigidbody_pool, entity.get_id()))
    {
      continue;
    }
    
    // Get Rb and generate its colldier.
    Physics::Rigidbody *rb = nullptr;
    rigidbody_pool_push(world_data->rigidbody_pool, entity.get_id(), &rb);

    rb->motion_state.reset(new Physics::Motion_state(entity.get_id(), world_data->entity_pool));
    
    Physics::colliders_generate(&rb_collider, 1, rb, 1);
  }
  
  // Stick them into the world.
  for(size_t i = 0; i < ent_count; ++i)
  {
    Core::Entity entity = ent[i];
    
    auto rb_collider = entity.get_rigidbody_collider();

    if(rb_collider.collider_type == Physics::Collider_type::none)
    {
      continue;
    }
    
    Physics::Rigidbody *rb = nullptr;
    rigidbody_pool_find(world_data->rigidbody_pool, entity.get_id(), &rb);
    assert(rb);

    auto get_child_colliders = [&](Core::Entity e, btCompoundShape *parent_compound)
    {
        auto lambda = [&](Core::Entity e, btCompoundShape *parent_compound, const auto& ff) -> void
        {  
          for(size_t c = 0; c < e.get_number_of_children(); ++c)
          {
            Core::Entity child = e.get_child(c);
          
            return ff(child, parent_compound, ff);
          }
          
          auto parent = e.get_parent();
          
          if(parent.is_valid())
          {
            auto parent_trans = parent.get_transform().position;
            auto child_trans = e.get_transform().position;
            auto pos = math::vec3_subtract(child_trans, parent_trans);
          
            btTransform transform;
            transform.setIdentity();
            transform.setOrigin(btVector3(math::vec3_get_x(pos), math::vec3_get_y(pos), math::vec3_get_z(pos)));
    
            
            
            //assert(false);
            // Need to get new transform.
            // Of the nested entity.
            
            Physics::Rigidbody *rb = nullptr;
            rigidbody_pool_find(world_data->rigidbody_pool, e.get_id(), &rb);
          
            parent_compound->addChildShape(transform, rb->shape.get());
          }
        };
          
        return lambda(e, parent_compound, lambda);
    };
    
    if(entity.get_parent().get_id() == Core::Entity_id_util::invalid_id())
    {
      //get_child_colliders(entity, rb->compound_shape.get());
      add_child_colliders(world_data, entity, rb->compound_shape.get());
      
      auto rb_props = entity.get_rigidbody_properties();
      
      Physics::world_add_rigidbodies(world_data->physics_world, &rb_props, 1, rb, 1);
    }
  }
}


} // ns