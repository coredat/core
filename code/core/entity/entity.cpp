#include <core/entity/entity.hpp>
#include <core/entity/entity_ref.hpp>
#include <core/entity/entity_interface.hpp>
#include <core/transform/transform.hpp>
#include <core/model/model.hpp>
#include <core/physics/collider.hpp>
#include <core/physics/rigidbody_properties.hpp>
#include <core/world/world.hpp>
#include <core/world/detail/world_detail.hpp>
#include <data/world_data/pending_scene_graph_change_data.hpp>
#include <data/world_data/world_pools.hpp>
#include <data/world_data/entity_data.hpp>
#include <data/world_data/transform_data.hpp>
#include <utilities/logging.hpp>
#include <assert.h>

#ifdef _WIN32
#include <atomic>
using std::atomic_uint_least32_t;
#else
#include <stdatomic.h>
#endif


namespace
{
  // Each new entity will get a new instance id.
  atomic_uint_least32_t instance_id(0);
}


namespace Core {


struct Entity::Impl
{
  util::generic_id id;
  std::shared_ptr<World_detail::Data> world;
};


Entity::Entity()
: m_impl(new Impl{util::generic_id_invalid(), nullptr})
{
}


Entity::Entity(Core::World &world)
: m_impl(new Impl{util::generic_id{++instance_id}, nullptr})
{
  m_impl->world = world.get_world_data();
 
  if(m_impl->world)
  {
    World_data::entity_data_add_entity(m_impl->world->data.entity, get_id());
    World_data::transform_data_add_transform(m_impl->world->data.transform, get_id());
    World_data::mesh_renderer_add(m_impl->world->data.mesh_data, get_id(), 0, 0);
  }
  else
  {
    LOG_ERROR("Failed to create a new entity.");
  }
}


Entity::~Entity()
{
  if(m_impl->id != util::generic_id_invalid())
  {
    destroy();
  }
}


Entity::Entity(Entity &&other)
: m_impl(new Impl{other.m_impl->id, other.m_impl->world})
{
  // Null other
  const_cast<util::generic_id&>(other.m_impl->id) = util::generic_id_invalid();
  other.m_impl->world = nullptr;
}


Entity&
Entity::operator=(Entity &&other)
{
  // If we just moved onto a valid entity destoy it.
  if(m_impl->id != util::generic_id_invalid())
  {
    destroy();
  }

  // Transfer the members.
  const_cast<util::generic_id&>(m_impl->id) = other.m_impl->id;
  m_impl->world = other.m_impl->world;
  
  // Null other
  const_cast<util::generic_id&>(other.m_impl->id) = util::generic_id_invalid();
  other.m_impl->world = nullptr;
  
  return *this;
}


Entity::operator Entity_ref() const
{
  return Entity_ref(*const_cast<Entity*>(this));
}


void
Entity::destroy()
{
  Entity_detail::destroy(m_impl->id, &m_impl->world->data);
  m_impl->id = util::generic_id_invalid();
}


Entity::operator bool() const
{
  return m_impl->id != util::generic_id_invalid();
}

// ** Common Entity Interface ** //


bool
Entity::is_valid() const
{
  return Entity_detail::is_valid(m_impl->id, &m_impl->world->data);
}


uint32_t
Entity::get_tags() const
{
  return Entity_detail::get_tags(m_impl->id, &m_impl->world->data);
}


void
Entity::set_user_data(const uintptr_t user_data)
{
  Entity_detail::set_user_data(m_impl->id, &m_impl->world->data, user_data);
}


uintptr_t
Entity::get_user_data() const
{
  return Entity_detail::get_user_data(m_impl->id, &m_impl->world->data);
}


bool
Entity::has_tag(const uint32_t tag_id) const
{
  return Entity_detail::has_tag(m_impl->id, &m_impl->world->data, tag_id);
}


void
Entity::set_tags(const uint32_t set_tags)
{
  Entity_detail::set_tags(m_impl->id, &m_impl->world->data, set_tags);
}


void
Entity::add_tag(const uint32_t add_tag)
{
  Entity_detail::add_tag(m_impl->id, &m_impl->world->data, add_tag);
}


void
Entity::remove_tag(const uint32_t tag)
{
  Entity_detail::remove_tag(m_impl->id, &m_impl->world->data, tag);
}


void
Entity::set_name(const char* set_name)
{
  Entity_detail::set_name(m_impl->id, &m_impl->world->data, set_name);
}


const char*
Entity::get_name() const
{
  return Entity_detail::get_name(m_impl->id, &m_impl->world->data);
}


void
Entity::set_transform(const Transform &transform)
{
  Entity_detail::set_transform(m_impl->id, &m_impl->world->data, transform);
}


Transform
Entity::get_transform() const
{
  return Entity_detail::get_transform(m_impl->id, &m_impl->world->data);
}


void
Entity::set_material_id(const uint32_t id)
{
  Entity_detail::set_material_id(m_impl->id, &m_impl->world->data, id);
}


uint32_t
Entity::get_material_id() const
{
  return Entity_detail::get_material_id(m_impl->id, &m_impl->world->data);
}


void
Entity::set_model(const Core::Model &model)
{
  Entity_detail::set_model(m_impl->id, &m_impl->world->data, model);
}


Core::Model
Entity::get_model() const
{
  return Entity_detail::get_model(m_impl->id, &m_impl->world->data);
}


void
Entity::set_collider(const Core::Collider &collider)
{
  Entity_detail::set_collider(m_impl->id, &m_impl->world->data, collider);
}


Core::Collider
Entity::get_collider() const
{
  return Entity_detail::get_collider(m_impl->id, &m_impl->world->data);
}


void
Entity::set_rigidbody_properties(const Core::Rigidbody_properties rb_props)
{
  Entity_detail::set_rigidbody_properties(m_impl->id, &m_impl->world->data, rb_props);
}


Core::Rigidbody_properties
Entity::set_rigidbody_properties() const
{
  return Entity_detail::get_rigidbody_properties(m_impl->id, &m_impl->world->data);
}


util::generic_id
Entity::get_id() const
{
  return Entity_detail::get_id(m_impl->id, &m_impl->world->data);
}


std::shared_ptr<const World_detail::Data>
Entity::get_world_data() const
{
  return m_impl->world;
}


std::shared_ptr<World_detail::Data>
Entity::get_world_data()
{
  return m_impl->world;
}


bool
Entity::operator==(const Entity &other) const
{
  return this->get_id() == other.get_id();
}


bool
Entity::operator==(const Entity_ref &other) const
{
  return this->get_id() == other.get_id();
}


bool
Entity::operator!=(const Entity &other) const
{
  return this->get_id() != other.get_id();
}


bool
Entity::operator !=(const Entity_ref &other) const
{
  return this->get_id() != other.get_id();
}


} // ns