#include "entity.hpp"
#include "world_data.hpp"
#include "entity_pool.hpp"


namespace
{
  std::atomic<uint32_t> instance(0);
}


namespace Data {


Entity::Entity(const uint8_t type_id, Data::World *world_data)
: m_this_id{type_id, ++instance}
, m_world_data(world_data)
{
}


::Entity::Entity_id
Entity::get_id() const
{
  return m_this_id;
}


namespace
{
  inline bool
  get_index(size_t *index, ::Entity::Entity_id id, Data::World *world)
  {
    if(!world)
    {
      return false;
    }
  
    const auto ent_pool = world->entity_pool;
    
    if(::Entity::find_index_linearly(index,
                                     id,
                                     ent_pool->entity_id,
                                     ent_pool->size))
                                     {
                                        return true;
                                     }
      
    return false;
  }
}


void
Entity::set_parent(const ::Entity::Entity_id id)
{
  // TODO: Check id is a valid parent.
  // TODO: Need to queue this change as rbs will need sorting.
  // TODO: Param check.
  
  size_t index;
  assert(get_index(&index, m_this_id, m_world_data));
  m_world_data->entity_pool->parent_id[index] = id;
}


::Entity::Entity_id
Entity::get_parent_id() const
{
  size_t index;
  assert(get_index(&index, m_this_id, m_world_data));
  return m_world_data->entity_pool->parent_id[index];
}


void
Entity::set_transform(const math::transform &transform)
{
  size_t index;
  assert(get_index(&index, m_this_id, m_world_data));
  m_world_data->entity_pool->transform[index] = transform;
}


math::transform
Entity::get_transform() const
{
  size_t index;
  assert(get_index(&index, m_this_id, m_world_data));
  return m_world_data->entity_pool->transform[index];
}


void
Entity::set_material_id(const size_t id)
{
  size_t index;
  assert(get_index(&index, m_this_id, m_world_data));
  m_world_data->entity_pool->texture[index] = (Resource::Texture::ENUM)id;
}


size_t
Entity::get_material_id() const
{
  size_t index;
  assert(get_index(&index, m_this_id, m_world_data));
  return (size_t)m_world_data->entity_pool->texture[index];
}


void
Entity::set_model_id(const size_t id)
{
  size_t index;
  assert(get_index(&index, m_this_id, m_world_data));
  m_world_data->entity_pool->model[index] = (Resource::Model::ENUM)id;
}


size_t
Entity::get_model_id() const
{
  size_t index;
  assert(get_index(&index, m_this_id, m_world_data));
  return (size_t)m_world_data->entity_pool->model[index];
}


void
Entity::set_rigidbody_properties(const Physics::Rigidbody_properties props)
{
  size_t index;
  assert(get_index(&index, m_this_id, m_world_data));
  m_world_data->entity_pool->rigidbody_property[index] = props;
}


Physics::Rigidbody_properties
Entity::get_rigidbody_properties() const
{
  size_t index;
  assert(get_index(&index, m_this_id, m_world_data));
  return m_world_data->entity_pool->rigidbody_property[index];
}


void
Entity::set_rigidbody_collider(const Physics::Rigidbody_collider collider)
{
  size_t index;
  assert(get_index(&index, m_this_id, m_world_data));
  m_world_data->entity_pool->rigidbody_collider[index] = collider;
}


Physics::Rigidbody_collider
Entity::get_rigidbody_collider() const
{
  size_t index;
  assert(get_index(&index, m_this_id, m_world_data));
  return m_world_data->entity_pool->rigidbody_collider[index];
}


} // ns