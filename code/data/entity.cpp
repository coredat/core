#include "entity.hpp"
#include "world_data.hpp"
#include "entity_pool.hpp"
#include "rigidbody_pool.hpp"


namespace Data {


Entity::Entity()
{
}


::Entity::Entity_id
Entity::get_id() const
{
  return m_this_id;
}


bool
Entity::is_valid() const
{
  return !(m_this_id == ::Entity::invalid_id());
}


namespace
{
  inline bool
  get_index(size_t *index, const ::Entity::Entity_id id, const ::Entity::Entity_id ents[], const size_t size)
  {
    if(!ents)
    {
      return false;
    }
    
    if(::Entity::find_index_linearly(index,
                                     id,
                                     ents,
                                     size))
                                     {
                                        return true;
                                     }
      
    return false;
  }
}


void
Entity::set_parent(const ::Entity::Entity_id id)
{
  size_t index;
  assert(get_index(&index, m_this_id, m_world_data->entity_pool->entity_id, m_world_data->entity_pool->size));
  m_world_data->entity_pool->parent_id[index] = id;
}


Entity
Entity::get_parent() const
{
  Entity parent;
  Detail::set_members(&parent, m_world_data, m_this_id);
  
  return parent;
}


size_t
Entity::get_number_of_children() const
{
  size_t children_count(0);
  
  for(size_t i = 0; i < m_world_data->entity_pool->size; ++i)
  {
    const ::Entity::Entity_id id = m_world_data->entity_pool->parent_id[i];
  
    if(id == m_this_id)
    {
      ++children_count;
    }
  }
  
  return children_count;
}


void
Entity::set_transform(const math::transform &transform)
{
  size_t index;
  assert(get_index(&index, m_this_id, m_world_data->entity_pool->entity_id, m_world_data->entity_pool->size));
  m_world_data->entity_pool->transform[index] = transform;
  
  const auto ent_pool = m_world_data->entity_pool;
  
  if(ent_pool->rigidbody_collider[index].collider_type != Physics::Collider_type::none)
  {
    rigidbody_pool_update_rb(m_world_data->rigidbody_pool,
                             m_this_id,
                             m_world_data->physics_world,
                             m_world_data,
                             ent_pool->rigidbody_property[index],
                             ent_pool->rigidbody_collider[index]);
  }
}


math::transform
Entity::get_transform() const
{
  size_t index;
  assert(get_index(&index, m_this_id, m_world_data->entity_pool->entity_id, m_world_data->entity_pool->size));
  return m_world_data->entity_pool->transform[index];
}


void
Entity::set_material_id(const size_t id)
{
  size_t index;
  assert(get_index(&index, m_this_id, m_world_data->entity_pool->entity_id, m_world_data->entity_pool->size));
  m_world_data->entity_pool->texture[index] = (Resource::Texture::ENUM)id;
}


size_t
Entity::get_material_id() const
{
  size_t index;
  assert(get_index(&index, m_this_id, m_world_data->entity_pool->entity_id, m_world_data->entity_pool->size));
  return (size_t)m_world_data->entity_pool->texture[index];
}


void
Entity::set_model_id(const size_t id)
{
  size_t index;
  assert(get_index(&index, m_this_id, m_world_data->entity_pool->entity_id, m_world_data->entity_pool->size));
  m_world_data->entity_pool->model[index] = (Resource::Model::ENUM)id;
}


size_t
Entity::get_model_id() const
{
  size_t index;
  assert(get_index(&index, m_this_id, m_world_data->entity_pool->entity_id, m_world_data->entity_pool->size));
  return (size_t)m_world_data->entity_pool->model[index];
}


void
Entity::set_rigidbody_properties(const Physics::Rigidbody_properties props)
{
  size_t index;
  assert(get_index(&index, m_this_id, m_world_data->entity_pool->entity_id, m_world_data->entity_pool->size));
  
  const auto ent_pool = m_world_data->entity_pool;
  
  ent_pool->rigidbody_property[index] = props;
  ent_pool->rigidbody_property[index].id = m_this_id;
  if(ent_pool->rigidbody_collider[index].collider_type != Physics::Collider_type::none)
  {
    rigidbody_pool_update_rb(m_world_data->rigidbody_pool,
                             m_this_id,
                             m_world_data->physics_world,
                             m_world_data,
                             ent_pool->rigidbody_property[index],
                             ent_pool->rigidbody_collider[index]);
  }
}


Physics::Rigidbody_properties
Entity::get_rigidbody_properties() const
{
  size_t index;
  assert(get_index(&index, m_this_id, m_world_data->entity_pool->entity_id, m_world_data->entity_pool->size));
  return m_world_data->entity_pool->rigidbody_property[index];
}


void
Entity::set_rigidbody_collider(const Physics::Rigidbody_collider collider)
{
  size_t index;
  assert(get_index(&index, m_this_id, m_world_data->entity_pool->entity_id, m_world_data->entity_pool->size));
  
  const auto ent_pool = m_world_data->entity_pool;

  ent_pool->rigidbody_collider[index] = collider;
  
  rigidbody_pool_update_rb(m_world_data->rigidbody_pool,
                           m_this_id,
                           m_world_data->physics_world,
                           m_world_data,
                           ent_pool->rigidbody_property[index],
                           ent_pool->rigidbody_collider[index]);
}


Physics::Rigidbody_collider
Entity::get_rigidbody_collider() const
{
  size_t index;
  assert(get_index(&index, m_this_id, m_world_data->entity_pool->entity_id, m_world_data->entity_pool->size));
  return m_world_data->entity_pool->rigidbody_collider[index];
}


} // ns