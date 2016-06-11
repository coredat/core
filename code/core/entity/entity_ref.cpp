#include <core/entity/entity_ref.hpp>
#include <core/entity/entity_interface.hpp>
#include <core/transform/transform.hpp>
#include <core/model/model.hpp>
#include <core/physics/collider.hpp>
#include <core/physics/rigidbody_properties.hpp>
#include <core/entity/entity.hpp>
#include <core/world/world.hpp>
#include <core/world/detail/world_detail.hpp>
#include <core/resources/material.hpp>


namespace Core {


struct Entity_ref::Impl
{
  const util::generic_id id;
  std::shared_ptr<World_detail::Data> world;
};


Entity_ref::Entity_ref()
: Entity_ref(util::generic_id_invalid(), nullptr)
{
}


Entity_ref::Entity_ref(Entity &entity)
: Entity_ref(entity.get_id(), entity.get_world_data())
{
}


Entity_ref::Entity_ref(const util::generic_id id, World &world)
: Entity_ref(id, world.get_world_data())
{
}


Entity_ref::Entity_ref(const util::generic_id id, std::shared_ptr<World_detail::Data> world_data)
: m_impl(new Impl{id, world_data})
{
}


Entity_ref::~Entity_ref()
{
  // Entity Ref does nothing, it is an observer here.
}

  
Entity_ref::Entity_ref(const Entity_ref &other)
: m_impl(new Impl{other.m_impl->id, other.m_impl->world})
{
}


Entity_ref::Entity_ref(Entity_ref &&other)
: m_impl(new Impl{other.m_impl->id, other.m_impl->world})
{
}


Entity_ref&
Entity_ref::operator=(const Entity_ref &other)
{
  const_cast<util::generic_id&>(m_impl->id) = other.m_impl->id;
  m_impl->world = other.m_impl->world;
  
  return *this;
}


Entity_ref&
Entity_ref::operator=(Entity_ref &&other)
{
  const_cast<util::generic_id&>(m_impl->id) = other.m_impl->id;
  m_impl->world = other.m_impl->world;
  
  return *this;
}


// ** Common Entity Interface ** //


util::generic_id
Entity_ref::get_id() const
{
  return Entity_detail::get_id(m_impl->id, &m_impl->world->data);
}


void
Entity_ref::destroy()
{
  Entity_detail::destroy(m_impl->id, &m_impl->world->data);
  util::generic_id *id = const_cast<util::generic_id*>(&m_impl->id);
  *id = util::generic_id_invalid();
}


bool
Entity_ref::is_valid() const
{
  return Entity_detail::is_valid(m_impl->id, &m_impl->world->data);
}


void
Entity_ref::set_user_data(const uintptr_t user_data)
{
  Entity_detail::set_user_data(m_impl->id, &m_impl->world->data, user_data);
}


uintptr_t
Entity_ref::get_user_data() const
{
  return Entity_detail::get_user_data(m_impl->id, &m_impl->world->data);
}


uint32_t
Entity_ref::get_tags() const
{
  return Entity_detail::get_tags(m_impl->id, &m_impl->world->data);
}


bool
Entity_ref::has_tag(const uint32_t tag_id) const
{
  return Entity_detail::has_tag(m_impl->id, &m_impl->world->data, tag_id);
}


void
Entity_ref::set_tags(const uint32_t set_tags)
{
  Entity_detail::set_tags(m_impl->id, &m_impl->world->data, set_tags);
}


void
Entity_ref::add_tag(const uint32_t add_tag)
{
  Entity_detail::add_tag(m_impl->id, &m_impl->world->data, add_tag);
}


void
Entity_ref::remove_tag(const uint32_t tag)
{
  Entity_detail::remove_tag(m_impl->id, &m_impl->world->data, tag);
}


void
Entity_ref::set_name(const char* set_name)
{
  Entity_detail::set_name(m_impl->id, &m_impl->world->data, set_name);
}


const char*
Entity_ref::get_name() const
{
  return Entity_detail::get_name(m_impl->id, &m_impl->world->data);
}


void
Entity_ref::set_transform(const Transform &transform)
{
  Entity_detail::set_transform(m_impl->id, &m_impl->world->data, transform);
}


Transform
Entity_ref::get_transform() const
{
  return Entity_detail::get_transform(m_impl->id, &m_impl->world->data);
}


void
Entity_ref::set_material(const Core::Material &mat)
{
  Entity_detail::set_material(m_impl->id, &m_impl->world->data, mat);
}


Core::Material
Entity_ref::get_material() const
{
  return Entity_detail::get_material(m_impl->id, &m_impl->world->data);
}


void
Entity_ref::set_model(const Core::Model &model)
{
  Entity_detail::set_model(m_impl->id, &m_impl->world->data, model);
}


Core::Model
Entity_ref::get_model() const
{
  return Entity_detail::get_model(m_impl->id, &m_impl->world->data);
}


void
Entity_ref::set_collider(const Core::Collider &collider)
{
  Entity_detail::set_collider(m_impl->id, &m_impl->world->data, collider);
}


Core::Collider
Entity_ref::get_collider() const
{
  return Entity_detail::get_collider(m_impl->id, &m_impl->world->data);
}


bool
Entity_ref::operator==(const Entity &other) const
{
  return this->get_id() == other.get_id();
}


void
Entity_ref::set_rigidbody_properties(const Core::Rigidbody_properties rb_props)
{
  Entity_detail::set_rigidbody_properties(m_impl->id, &m_impl->world->data, rb_props);
}


Core::Rigidbody_properties
Entity_ref::set_rigidbody_properties() const
{
  return Entity_detail::get_rigidbody_properties(m_impl->id, &m_impl->world->data);
}


std::shared_ptr<const World_detail::Data>
Entity_ref::get_world_data() const
{
  return m_impl->world;
}


std::shared_ptr<World_detail::Data>
Entity_ref::get_world_data()
{
  return m_impl->world;
}


bool
Entity_ref::operator==(const Entity_ref &other) const
{
  return this->get_id() == other.get_id();
}


bool
Entity_ref::operator!=(const Entity &other) const
{
  return this->get_id() != other.get_id();
}


bool
Entity_ref::operator !=(const Entity_ref &other) const
{
  return this->get_id() != other.get_id();
}


} // ns