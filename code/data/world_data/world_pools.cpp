#include "world_pools.hpp"
#include "entity_pool.hpp"
#include "physics_data.hpp"
#include "graph_change_pool.hpp"
#include <core/entity/entity.hpp>
#include <core/entity/entity_ref.hpp>
#include <atomic>


namespace
{
  // Instance id is incremented each time
  // a new entity is added to the world.
  std::atomic<uint32_t> instance(0);
}


namespace World_data {


bool
world_create_new_entity(World *world_data,
                        util::generic_id id)
{
  // Param check.
  assert(world_data);
  
  auto entity_pool = world_data->entity_pool;
  //util::generic_id new_id{type_id, ++instance};
  
  if(World_data::entity_pool_push_new_entity(entity_pool, id))
  {
    entity_graph_change_push(world_data->entity_graph_changes,
                             id,
                             Entity_graph_change::inserted);
    
    return true;
  }
  
  // Didn't find an index. Entity data is full.
  return false;
}


//bool
//world_find_entity(World *world_data,
//                  Core::Entity_ref *out_entity,
//                  const util::generic_id id)
//{
//  assert(world_data);
//  assert(id != util::generic_id_invalid());
//
//  auto entity_pool = world_data->entity_pool;
//
//  // Search the list for the entity.
//  size_t index;
//  if(util::generic_id_search_linearly(&index,
//                                      id,
//                                      entity_pool->entity_id,
//                                      entity_pool->size))
//  {
//    Core::Entity_ref found_entity(id, world_data);
//    *out_entity = found_entity;
//
//    return true;
//  }
//  
//  // Didn't find the entity that was requested.
//  return false;
//}


void
world_find_entities_with_tag(World *world_data,
                             const uint32_t tag,
                             uint32_t *out_entities_for_tag,
                             util::generic_id out_ids[],
                             const uint32_t size_of_out)
{
  assert(world_data);
  
  auto entity_pool = world_data->entity_pool;
  
  uint32_t number_found(0);
  
  for(uint32_t i = 0; i < entity_pool->size; ++i)
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
world_update_scene_graph_changes(World_data::World *world_data,
                                 const Entity_graph_changes_pool *graph_changes)
{
  for(uint32_t i = 0; i < graph_changes->size; ++i)
  {
    const auto change = graph_changes->entity_event[i];
    
    switch(change.change_type)
    {
      case(World_data::Entity_graph_change::removed):
      {
        entity_pool_remove_entity(world_data->entity_pool, change.entity_id);
        physics_remove(world_data->physics_data, change.entity_id);
      
        break;
      }
      
      default:
        break;
    }
  }
}


} // ns