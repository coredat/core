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


Entity
world_create_new_entity(World *world_data, const uint32_t type_id)
{
  Entity new_entity;
  Detail::set_members(&new_entity, world_data, ::Entity::Entity_id{type_id, ++instance});
  
  // Find empty index.
  size_t empty_index;
  ::Entity::find_index_linearly(&empty_index,
                                ::Entity::invalid_id(),
                                world_data->entity_pool->entity_id,
                                world_data->entity_pool->size);
  
  world_data->entity_pool->entity_id[empty_index] = new_entity.get_id();
  
  return new_entity;
}


void
world_find_entity(Entity *out_entity, World *world_data, const ::Entity::Entity_id id)
{
  size_t index;
  if(::Entity::find_index_linearly(&index, id, world_data->entity_pool->entity_id, world_data->entity_pool->size))
  {
    Detail::set_members(out_entity, world_data, id);
  }
}


} // ns