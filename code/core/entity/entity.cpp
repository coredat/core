#include <core/entity/entity.hpp>
#include <core/entity/entity_ref.hpp>
#include <core/entity/entity_interface.hpp>
#include <core/transform/transform.hpp>
#include <core/model/model.hpp>
#include <core/physics/collider.hpp>
#include <data/world_data/graph_change_pool.hpp>
#include <data/world_data/world_pools.hpp>
#include <core/world/world.hpp>
#include <core/world/detail/world_detail.hpp>
#include <assert.h>



namespace Core {


struct Entity::Impl
{
  const Core::Entity_id id = Core::Entity_id_util::invalid_id();
  World_data::World *world = nullptr;
};


Entity::Entity()
: m_impl()
{
}


Entity::Entity(const Core::World &world)
: m_impl(new Impl{Core::Entity_id{1,1}, nullptr})
{
  if(world.get_world_data())
  {
    m_impl->world = const_cast<World_data::World*>(&world.get_world_data()->data);
  }
}


Entity::~Entity()
{
  if(m_impl->id != Core::Entity_id_util::invalid_id())
  {
    destroy();
  }
}


Entity::Entity(Entity &&other)
: m_impl(new Impl{other.m_impl->id, other.m_impl->world})
{
  // Null other
  const_cast<Core::Entity_id&>(other.m_impl->id) = Core::Entity_id_util::invalid_id();
  other.m_impl->world = nullptr;
}


Entity&
Entity::operator=(Entity &&other)
{
  // If we just moved onto a valid entity destoy it.
  if(m_impl->id != Core::Entity_id_util::invalid_id())
  {
    destroy();
  }

  // Transfer the members.
  const_cast<Core::Entity_id&>(m_impl->id) = other.m_impl->id;
  m_impl->world = other.m_impl->world;
  
  // Null other
  const_cast<Core::Entity_id&>(other.m_impl->id) = Core::Entity_id_util::invalid_id();
  other.m_impl->world = nullptr;
  
  return *this;
}


Entity::operator Entity_ref() const
{
  return Entity_ref(m_impl->id, m_impl->world);
}


void
Entity::destroy()
{
  if(!is_valid()) { return; }
  
  // Destroy this.
  World_data::entity_graph_change_push(m_impl->world->entity_graph_changes,
                                       m_impl->id,
                                       World_data::Entity_graph_change::removed);
}


// ** Common Entity Interface ** //


Core::Entity_id
Entity::get_id() const
{
  return Entity_detail::get_id(m_impl->id, m_impl->world);
}


bool
Entity::is_valid() const
{
  return Entity_detail::is_valid(m_impl->id, m_impl->world);
}


bool
Entity::is_active() const
{
  return Entity_detail::is_active(m_impl->id, m_impl->world);
}


void
Entity::set_active(const bool set_active)
{
  Entity_detail::set_active(m_impl->id, m_impl->world, set_active);
}


uint32_t
Entity::get_tags() const
{
  return Entity_detail::get_tags(m_impl->id, m_impl->world);
}


bool
Entity::has_tag(const uint32_t tag_id) const
{
  return Entity_detail::has_tag(m_impl->id, m_impl->world, tag_id);
}


void
Entity::set_tags(const uint32_t set_tags)
{
  Entity_detail::set_tags(m_impl->id, m_impl->world, set_tags);
}


void
Entity::add_tag(const uint32_t add_tag)
{
  Entity_detail::add_tag(m_impl->id, m_impl->world, add_tag);
}


void
Entity::remove_tag(const uint32_t tag)
{
  Entity_detail::remove_tag(m_impl->id, m_impl->world, tag);
}


void
Entity::set_name(const char* set_name)
{
  Entity_detail::set_name(m_impl->id, m_impl->world, set_name);
}


const char*
Entity::get_name() const
{
  return Entity_detail::get_name(m_impl->id, m_impl->world);
}


void
Entity::send_event(const uint32_t id, const void *data, const uint32_t size_of_data)
{
  Entity_detail::send_event(m_impl->id, m_impl->world, id, data, size_of_data);
}


void
Entity::set_transform(const Transform &transform)
{
  Entity_detail::set_transform(m_impl->id, m_impl->world, transform);
}


Transform
Entity::get_transform() const
{
  return Entity_detail::get_transform(m_impl->id, m_impl->world);
}


void
Entity::set_material_id(const uint32_t id)
{
  Entity_detail::set_material_id(m_impl->id, m_impl->world, id);
}


uint32_t
Entity::get_material_id() const
{
  return Entity_detail::get_material_id(m_impl->id, m_impl->world);
}


void
Entity::set_model(const Core::Model &model)
{
  Entity_detail::set_model(m_impl->id, m_impl->world, model);
}


Core::Model
Entity::get_model() const
{
  return Entity_detail::get_model(m_impl->id, m_impl->world);
}


void
Entity::set_collider(const Core::Collider &collider)
{
  Entity_detail::set_collider(m_impl->id, m_impl->world, collider);
}


Core::Collider
Entity::get_collider() const
{
  return Entity_detail::get_collider(m_impl->id, m_impl->world);
}


} // ns