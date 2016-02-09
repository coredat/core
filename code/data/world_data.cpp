#include "world_data.hpp"
#include "entity_pool.hpp"
#include <core/interface/entity.hpp>
#include <systems/physics/physics.hpp>
#include <atomic>


namespace
{
  // Instance id is incremented each time
  // a new entity is added to the world.
  std::atomic<uint32_t> instance(0);
  
  Data::World *world = nullptr;
}


namespace Data {


void
set_world_data(World *set_world) { world = set_world; }


Physics::World*
get_physics_world() { return world->physics_world; }

World*
get_world() { return world; }


bool
world_create_new_entity(World *world_data, Core::Entity *out_entity, const uint32_t type_id)
{
  // Param check.
  assert(world_data && out_entity && type_id);

  // Find empty index.
  {
    auto entity_pool = world_data->entity_pool;

    size_t empty_index;
    if(Core::Entity_id_util::find_index_linearly(&empty_index,
                                                 Core::Entity_id_util::invalid_id(),
                                                 entity_pool->entity_id,
                                                 entity_pool->size))
    {
      Core::Detail::set_entity_members(out_entity, world_data, Core::Entity_id{type_id, ++instance});
      
      entity_pool->entity_id[empty_index] = out_entity->get_id();

      entity_graph_change_push(world_data->entity_graph_changes,
                               out_entity->get_id(),
                               Entity_graph_change::inserted);

      return true;
    }
  }
  
  // Didn't find an index. Entity data is full.
  return false;
}


bool
world_find_entity(World *world_data, Core::Entity *out_entity, const Core::Entity_id id)
{
  assert(world_data);
  assert(id != Core::Entity_id_util::invalid_id());

  auto entity_pool = world_data->entity_pool;

  // Search the list for the entity.
  size_t index;
  if(Core::Entity_id_util::find_index_linearly(&index,
                                               id,
                                               entity_pool->entity_id,
                                               entity_pool->size))
  {
    Core::Detail::set_entity_members(out_entity, world_data, id);
    return true;
  }
  
  // Didn't find the entity that was requested.
  return false;
}


void
world_find_entities_with_tag(World *world_data,
                             const uint32_t tag,
                             size_t *out_entities_for_tag,
                             Core::Entity_id out_ids[],
                             const size_t size_of_out)
{
  assert(world_data);
  
  auto entity_pool = world_data->entity_pool;
  
  size_t number_found(0);
  
  for(size_t i = 0; i < entity_pool->size; ++i)
  {
    auto prop = entity_pool->entity_properties[i];
    
    if(prop.tags & tag)
    {
      if(size_of_out > number_found)
      {
        out_ids[number_found++] = entity_pool->entity_id[i];
      }
      else
      {
        break;
      }
    }
  }
  
  (*out_entities_for_tag) = number_found;
}


void
world_update_scene_graph_changes(Data::World *world_data,
                                 const Entity_graph_changes_pool *graph_changes)
{
  for(size_t i = 0; i < graph_changes->size; ++i)
  {
    const auto change = graph_changes->entity_event[i];
    
    switch(change.change_type)
    {
      case(Data::Entity_graph_change::removed):
      
      {
        auto pool = world_data->entity_pool;
      
        size_t remove_id;
        if(Core::Entity_id_util::find_index_linearly(&remove_id,
                                                     change.entity_id,
                                                     pool->entity_id,
                                                     pool->size))
        {
          memset(&pool->entity_id[remove_id],          0, sizeof(pool->entity_id[remove_id]));
          memset(&pool->parent_id[remove_id],          0, sizeof(pool->parent_id[remove_id]));
          memset(&pool->entity_properties[remove_id],  0, sizeof(pool->entity_properties[remove_id]));
//          memset(&pool->model[remove_id],              0, sizeof(pool->model[remove_id]));
//          memset(&pool->display[remove_id],            0, sizeof(pool->display[remove_id]));
//          memset(&pool->texture[remove_id],            0, sizeof(pool->texture[remove_id]));
          memset(&pool->transform[remove_id],          0, sizeof(pool->transform[remove_id]));
//          memset(&pool->rigidbody_property[remove_id], 0, sizeof(pool->rigidbody_property[remove_id]));
        }
        
          
        break;
      }
    }
  }
}


} // ns