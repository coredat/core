#include "entity.hpp"
#include "data.hpp"


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
  if(!is_valid()) { return; }

  size_t index;
  assert(get_index(&index, m_this_id, m_world_data->entity_pool->entity_id, m_world_data->entity_pool->size));
  m_world_data->entity_pool->parent_id[index] = id;
}


Entity
Entity::get_parent() const
{
  if(!is_valid()) { return Entity(); }

  Entity parent;
  Detail::set_entity_members(&parent, m_world_data, m_this_id);
  
  return parent;
}


size_t
Entity::get_number_of_children() const
{
  if(!is_valid()) { return 0; }

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
  if(!is_valid()) { return; }

  auto ent_pool = m_world_data->entity_pool;

  size_t index;
  assert(get_index(&index, m_this_id, ent_pool->entity_id, ent_pool->size));
  ent_pool->transform[index] = transform;
  
  // If this object has a colldier attached to it,
  // we need to update the rb.
  auto rb_coll = &ent_pool->rigidbody_collider[index];
  
  if(rb_coll->collider_type != Physics::Collider_type::none)
  {
    auto rb_prop = &ent_pool->rigidbody_property[index];
    rigidbody_update_pool_add_update(m_world_data->rigidbody_update_pool, m_this_id, *rb_coll, *rb_prop);
  }
}


math::transform
Entity::get_transform() const
{
  if(!is_valid()) { return math::transform(); }
  
  auto ent_pool = m_world_data->entity_pool;

  size_t index;
  assert(get_index(&index, m_this_id, ent_pool->entity_id, ent_pool->size));
  return ent_pool->transform[index];
}


void
Entity::set_material_id(const size_t id)
{
  if(!is_valid()) { return; }
  
  auto ent_pool = m_world_data->entity_pool;

  size_t index;
  assert(get_index(&index, m_this_id, ent_pool->entity_id, ent_pool->size));
  ent_pool->texture[index] = (Resource::Texture::ENUM)id;
}


size_t
Entity::get_material_id() const
{
  if(!is_valid()) { return 0; }

  auto ent_pool = m_world_data->entity_pool;

  size_t index;
  assert(get_index(&index, m_this_id, ent_pool->entity_id, ent_pool->size));
  return (size_t)ent_pool->texture[index];
}


void
Entity::set_model_id(const size_t id)
{
  if(!is_valid()) { return; }
  
  auto ent_pool = m_world_data->entity_pool;

  size_t index;
  assert(get_index(&index, m_this_id, ent_pool->entity_id, ent_pool->size));
  ent_pool->model[index] = (Resource::Model::ENUM)id;
}


size_t
Entity::get_model_id() const
{
  if(!is_valid()) { return 0; }
  
  auto ent_pool = m_world_data->entity_pool;

  size_t index;
  assert(get_index(&index, m_this_id, ent_pool->entity_id, ent_pool->size));
  return (size_t)ent_pool->model[index];
}


void
Entity::set_rigidbody_properties(const Physics::Rigidbody_properties props)
{
  if(!is_valid()) { return; }
  
  auto ent_pool = m_world_data->entity_pool;

  size_t index;
  assert(get_index(&index, m_this_id, ent_pool->entity_id, ent_pool->size));
  
  auto rb_prop = &ent_pool->rigidbody_property[index];

  *rb_prop = props;
  rb_prop->id = m_this_id;
  
  auto rb_coll = &ent_pool->rigidbody_collider[index];
  
  if(rb_coll->collider_type != Physics::Collider_type::none)
  {
    rigidbody_update_pool_add_update(m_world_data->rigidbody_update_pool, m_this_id, *rb_coll, *rb_prop);
  }
}


Physics::Rigidbody_properties
Entity::get_rigidbody_properties() const
{
  if(!is_valid()) { return Physics::Rigidbody_properties(); }

  size_t index;
  assert(get_index(&index, m_this_id, m_world_data->entity_pool->entity_id, m_world_data->entity_pool->size));
  return m_world_data->entity_pool->rigidbody_property[index];
}


void
Entity::set_rigidbody_collider(const Physics::Rigidbody_collider collider)
{
  if(!is_valid()) { return; }

  auto ent_pool = m_world_data->entity_pool;

  size_t index;
  assert(get_index(&index, m_this_id, ent_pool->entity_id, ent_pool->size));
  
  auto rb_coll = &ent_pool->rigidbody_collider[index];
  auto rb_prop = &ent_pool->rigidbody_property[index];
  
  *rb_coll = collider;
  
  rigidbody_update_pool_add_update(m_world_data->rigidbody_update_pool, m_this_id, *rb_coll, *rb_prop);
}


Physics::Rigidbody_collider
Entity::get_rigidbody_collider() const
{
  if(!is_valid()) { return Physics::Rigidbody_collider(); }

  auto ent_pool = m_world_data->entity_pool;

  size_t index;
  assert(get_index(&index, m_this_id, ent_pool->entity_id, ent_pool->size));
  return m_world_data->entity_pool->rigidbody_collider[index];
}


} // ns