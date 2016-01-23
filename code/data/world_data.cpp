#include "world_data.hpp"
#include "entity_pool.hpp"
#include "pending_rigidbody_pool.hpp"
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
  struct Private_access
  {
    ENTITY_MEMBERS
  };

  Entity new_entity;
  
  Private_access *set = reinterpret_cast<Private_access*>(&new_entity);
  set->m_this_id      = ::Entity::Entity_id{type_id, ++instance};
  set->m_world_data   = world_data;
  
  // Find empty index.
  size_t empty_index;
  ::Entity::find_index_linearly(&empty_index,
                                ::Entity::invalid_id(),
                                world_data->entity_pool->entity_id,
                                world_data->entity_pool->size);
  
  world_data->entity_pool->entity_id[empty_index] = new_entity.get_id();
  
  return new_entity;
}


} // ns