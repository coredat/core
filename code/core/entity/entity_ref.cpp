#include <core/entity/entity_ref.hpp>
#include <core/entity/entity_interface.hpp>
#include <core/transform/transform.hpp>
#include <core/model/model.hpp>
#include <core/physics/collider.hpp>


namespace Core {


struct Entity_ref::Impl
{
  const util::generic_id id;
  World_data::World *world;
};


Entity_ref::Entity_ref()
: Entity_ref::Entity_ref(util::generic_id_invalid(), nullptr)
{
}


Entity_ref::Entity_ref(const util::generic_id id, World_data::World *world)
: m_impl(new Impl{id, world})
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
  return Entity_detail::get_id(m_impl->id, m_impl->world);
}


bool
Entity_ref::is_valid() const
{
  return Entity_detail::is_valid(m_impl->id, m_impl->world);
}


uint32_t
Entity_ref::get_tags() const
{
  return Entity_detail::get_tags(m_impl->id, m_impl->world);
}


bool
Entity_ref::has_tag(const uint32_t tag_id) const
{
  return Entity_detail::has_tag(m_impl->id, m_impl->world, tag_id);
}


void
Entity_ref::set_tags(const uint32_t set_tags)
{
  Entity_detail::set_tags(m_impl->id, m_impl->world, set_tags);
}


void
Entity_ref::add_tag(const uint32_t add_tag)
{
  Entity_detail::add_tag(m_impl->id, m_impl->world, add_tag);
}


void
Entity_ref::remove_tag(const uint32_t tag)
{
  Entity_detail::remove_tag(m_impl->id, m_impl->world, tag);
}


void
Entity_ref::set_name(const char* set_name)
{
  Entity_detail::set_name(m_impl->id, m_impl->world, set_name);
}


const char*
Entity_ref::get_name() const
{
  return Entity_detail::get_name(m_impl->id, m_impl->world);
}


void
Entity_ref::send_event(const uint32_t id, const void *data, const uint32_t size_of_data)
{
  Entity_detail::send_event(m_impl->id, m_impl->world, id, data, size_of_data);
}


void
Entity_ref::set_transform(const Transform &transform)
{
  Entity_detail::set_transform(m_impl->id, m_impl->world, transform);
}


Transform
Entity_ref::get_transform() const
{
  return Entity_detail::get_transform(m_impl->id, m_impl->world);
}


void
Entity_ref::set_material_id(const uint32_t id)
{
  Entity_detail::set_material_id(m_impl->id, m_impl->world, id);
}


uint32_t
Entity_ref::get_material_id() const
{
  return Entity_detail::get_material_id(m_impl->id, m_impl->world);
}


void
Entity_ref::set_model(const Core::Model &model)
{
  Entity_detail::set_model(m_impl->id, m_impl->world, model);
}


Core::Model
Entity_ref::get_model() const
{
  return Entity_detail::get_model(m_impl->id, m_impl->world);
}


void
Entity_ref::set_collider(const Core::Collider &collider)
{
  Entity_detail::set_collider(m_impl->id, m_impl->world, collider);
}


Core::Collider
Entity_ref::get_collider() const
{
  return Entity_detail::get_collider(m_impl->id, m_impl->world);
}


} // ns