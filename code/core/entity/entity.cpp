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
#include <stdatomic.h>
#include <assert.h>


namespace
{
  atomic_uint_least32_t instance_id(0);
}


namespace Core {


struct Entity::Impl
{
  const Core::Entity_id id = Core::Entity_id_util::invalid_id();
  std::shared_ptr<World_detail::World_data> world;
};


Entity::Entity()
: m_impl(new Impl{Core::Entity_id_util::invalid_id(), nullptr})
{
}


Entity::Entity(const Core::World &world)
: m_impl(new Impl{Core::Entity_id{1,++instance_id}, nullptr})
{
  if(world.get_world_data())
  {
    // Not pretty! Alternativily we could have the World create the entity
    // but that would expose the internal data types.
    m_impl->world = std::const_pointer_cast<World_detail::World_data>(world.get_world_data());
    
    World_data::world_create_new_entity(&m_impl->world->data, this, 99);
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
  return Entity_ref(m_impl->id, &m_impl->world->data);
}


void
Entity::destroy()
{
  if(!is_valid()) { return; }
  
  // Destroy this.
  World_data::entity_graph_change_push(m_impl->world->data.entity_graph_changes,
                                       m_impl->id,
                                       World_data::Entity_graph_change::removed);
}


Entity::operator bool() const
{
  return m_impl->id != Entity_id_util::invalid_id();
}

// ** Common Entity Interface ** //


Core::Entity_id
Entity::get_id() const
{
  return Entity_detail::get_id(m_impl->id, &m_impl->world->data);
}


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
Entity::send_event(const uint32_t id, const void *data, const uint32_t size_of_data)
{
  Entity_detail::send_event(m_impl->id, &m_impl->world->data, id, data, size_of_data);
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


} // ns