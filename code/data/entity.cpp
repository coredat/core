#include "entity.hpp"


namespace Data {


Entity::Entity()
{
}


void
Entity::set_parent(const ::Entity::Entity_id id)
{
  m_parent_id = id;
}


::Entity::Entity_id
Entity::get_parent() const
{
  return m_parent_id;
}


void
Entity::set_material_id(const size_t id)
{
  m_material_id = id;
}


size_t
Entity::get_material_id() const
{
  return m_material_id;
}


void
Entity::set_model_id(const size_t id)
{
  m_model_id = id;
}


size_t
Entity::get_model_id() const
{
  return m_model_id;
}


void
Entity::set_rigidbody_properties(const Physics::Rigidbody_properties props)
{
  m_rb_props = props;
}


Physics::Rigidbody_properties
Entity::get_rigidbody_properties() const
{
  return m_rb_props;
}


void
Entity::set_rigidbody_collider(const Physics::Rigidbody_collider collider)
{
  m_rb_collider = collider;
}


Physics::Rigidbody_collider
Entity::get_rigidbody_collider() const
{
  return m_rb_collider;
}


} // ns