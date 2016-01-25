#include "world_data.hpp"
#include "entity_pool.hpp"
#include "entity.hpp"
#include <systems/physics/physics.hpp>
#include <atomic>


namespace
{
  std::atomic<uint32_t> instance(0);
}


namespace Data {


bool
world_create_new_entity(World *world_data, Entity *out_entity, const uint32_t type_id)
{
  // Find empty index.
  size_t empty_index;
  if(::Entity::find_index_linearly(&empty_index,
                                   ::Entity::invalid_id(),
                                   world_data->entity_pool->entity_id,
                                   world_data->entity_pool->size))
  {
    Detail::set_entity_members(out_entity, world_data, ::Entity::Entity_id{type_id, ++instance});
    
    world_data->entity_pool->entity_id[empty_index] = out_entity->get_id();

    entity_graph_change_push(world_data->entity_graph_changes,
                             out_entity->get_id(),
                             Entity_graph_change::inserted);

    return true;
  }
  
  return false;
}


bool
world_find_entity(World *world_data, Entity *out_entity, const ::Entity::Entity_id id)
{
  size_t index;
  if(::Entity::find_index_linearly(&index,
                                   id,
                                   world_data->entity_pool->entity_id,
                                   world_data->entity_pool->size))
  {
    Detail::set_entity_members(out_entity, world_data, id);
    return true;
  }
  
  return false;
}


} // ns