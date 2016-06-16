#include "world_pools.hpp"
#include "renderer_mesh_data.hpp"
#include "physics_data.hpp"
#include "pending_scene_graph_change_data.hpp"
#include "entity_data.hpp"
#include "transform_data.hpp"
#include <core/entity/entity.hpp>
#include <core/entity/entity_ref.hpp>
#include <utilities/logging.hpp>
#include <atomic>


namespace
{
  // Instance id is incremented each time
  // a new entity is added to the world.
  std::atomic<uint32_t> instance(0);
}


namespace World_data {


//bool
//world_create_new_entity(World *world_data,
//                        util::generic_id id)
//{
//  // Param check.
//  assert(world_data);
//  
//  LOG_INFO_ONCE("Does this get used at all?");
//  
//  auto entity_data = world_data->entity;
//  
//  {
//    const auto id = World_data::entity_data_push_back(entity_data);
//    if(id)
//    {
//      World_data::transform_data_push_back(world_data->transform, id);
//      World_data::renderer_mesh_data_push_back(world_data->mesh_data, id);
//    }
//    
//    return true;
//  }
//  
//  // Didn't find an index. Entity data is full.
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
  
  auto entity_data = world_data->entity;
  
  uint32_t number_found(0);
  
  for(uint32_t i = 0; i < entity_data->size; ++i)
  {
    auto tags = entity_data->property_tag[i];
    
    if(tags & tag)
    {
      if(size_of_out > number_found)
      {
        out_ids[number_found++] = entity_data->entity_id[i];
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
                                 const Pending_scene_graph_change_data *graph_changes)
{
  for(uint32_t i = 0; i < graph_changes->delete_size; ++i)
  {
    const util::generic_id id = graph_changes->entities_to_delete[i];
    
    entity_data_erase(world_data->entity, id);
    transform_data_erase(world_data->transform, id);
    renderer_mesh_data_erase(world_data->mesh_data, id);
    physics_data_erase(world_data->physics_data, id);
  }
}


} // ns