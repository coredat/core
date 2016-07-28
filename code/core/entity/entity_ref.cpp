#include <core/entity/entity_ref.hpp>
#include <transformations/entity/entity_common.hpp>
#include <transformations/entity/entity_data.hpp>
#include <transformations/entity/entity_transform.hpp>
#include <transformations/entity/entity_rigidbody.hpp>
#include <transformations/entity/entity_renderer.hpp>
#include <core/transform/transform.hpp>
#include <core/model/model.hpp>
#include <core/physics/collider.hpp>
#include <core/physics/rigidbody.hpp>
#include <core/entity/entity.hpp>
#include <core/world/world.hpp>
#include <core/renderer/renderer.hpp>
#include <core/renderer/material_renderer.hpp>
#include <core/resources/material.hpp>
#include <utilities/logging.hpp>


namespace Core {


struct Entity_ref::Impl
{
  util::generic_id id = 0;
  std::shared_ptr<World_data::World> world = nullptr;
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


Entity_ref::Entity_ref(const util::generic_id id, std::shared_ptr<World_data::World> world_data)
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
  if(!m_impl)
  {
    m_impl.reset(new Impl);
  }

  m_impl->id = other.m_impl->id;
  m_impl->world = other.m_impl->world;
  
  return *this;
}


Entity_ref&
Entity_ref::operator=(Entity_ref &&other)
{
  if(!m_impl)
  {
    m_impl.reset(new Impl);
  }

  m_impl->id = other.m_impl->id;
  m_impl->world = other.m_impl->world;
  
  return *this;
}


// ** Common Entity Interface ** //


util::generic_id
Entity_ref::get_id() const
{
  return Entity_detail::get_id(m_impl->id, m_impl->world.get());
}


void
Entity_ref::destroy()
{
  Entity_detail::destroy(m_impl->id, m_impl->world.get());
  util::generic_id *id = const_cast<util::generic_id*>(&m_impl->id);
  *id = util::generic_id_invalid();
}


bool
Entity_ref::is_valid() const
{
  return Entity_detail::is_valid(m_impl->id, m_impl->world.get());
}


Entity_ref::operator bool() const
{
  return is_valid();
}


void
Entity_ref::set_user_data(const uintptr_t user_data)
{
  Entity_detail::set_user_data(m_impl->id, m_impl->world.get(), user_data);
}


uintptr_t
Entity_ref::get_user_data() const
{
  return Entity_detail::get_user_data(m_impl->id, m_impl->world.get());
}


uint32_t
Entity_ref::get_tags() const
{
  return Entity_detail::get_tags(m_impl->id, m_impl->world.get());
}


bool
Entity_ref::has_tag(const uint32_t tag_id) const
{
  return Entity_detail::has_tag(m_impl->id, m_impl->world.get(), tag_id);
}


void
Entity_ref::set_tags(const uint32_t set_tags)
{
  Entity_detail::set_tags(m_impl->id, m_impl->world.get(), set_tags);
}


void
Entity_ref::add_tag(const uint32_t add_tag)
{
  Entity_detail::add_tag(m_impl->id, m_impl->world.get(), add_tag);
}


void
Entity_ref::remove_tag(const uint32_t tag)
{
  Entity_detail::remove_tag(m_impl->id, m_impl->world.get(), tag);
}


void
Entity_ref::set_name(const char* set_name)
{
  Entity_detail::set_name(m_impl->id, m_impl->world.get(), set_name);
}


const char*
Entity_ref::get_name() const
{
  return Entity_detail::get_name(m_impl->id, m_impl->world.get());
}


void
Entity_ref::set_transform(const Transform &transform)
{
  Entity_detail::set_transform(m_impl->id, m_impl->world.get(), transform);
}


void
Entity_ref::set_renderer(const Core::Renderer &renderer)
{
  Entity_detail::set_renderer(m_impl->id, m_impl->world.get(), renderer);
}


Renderer
Entity_ref::get_renderer() const
{
  return Entity_detail::get_renderer(m_impl->id, m_impl->world.get());
}


Transform
Entity_ref::get_transform() const
{
  return Entity_detail::get_transform(m_impl->id, m_impl->world.get());
}


bool
Entity_ref::operator==(const Entity &other) const
{
  return this->get_id() == other.get_id();
}


std::shared_ptr<const World_data::World>
Entity_ref::get_world_data() const
{
  return m_impl->world;
}


std::shared_ptr<World_data::World>
Entity_ref::get_world_data()
{
  return m_impl->world;
}


void
Entity_ref::set_rigidbody(const Rigidbody &rigidbody)
{
  Entity_detail::set_rigidbody(m_impl->id, m_impl->world.get(), rigidbody);
}


Rigidbody
Entity_ref::get_rigidbody() const
{
  return Entity_detail::get_rigidbody(m_impl->id, m_impl->world.get());
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


Entity_ref::operator util::generic_id() const
{
  return get_id();
}


} // ns