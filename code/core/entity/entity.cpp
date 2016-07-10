#include <core/entity/entity.hpp>
#include <core/entity/entity_ref.hpp>
#include <core/entity/entity_interface.hpp>
#include <core/transform/transform.hpp>
#include <core/model/model.hpp>
#include <core/renderer/renderer.hpp>
#include <core/renderer/material_renderer.hpp>
#include <core/resources/material.hpp>
#include <core/physics/collider.hpp>
#include <core/physics/rigidbody_properties.hpp>
#include <core/world/world.hpp>
#include <data/world_data/pending_scene_graph_change_data.hpp>
#include <data/world_data/world_pools.hpp>
#include <data/world_data/entity_data.hpp>
#include <data/world_data/transform_data.hpp>
#include <data/world_data/renderer_mesh_data.hpp>
#include <common/error_strings.hpp>
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
//  atomic_uint_least32_t instance_id(0);
}


namespace Core {


struct Entity::Impl
{
  util::generic_id id;
  std::shared_ptr<World_data::World> world;
};


Entity::Entity()
: m_impl(new Impl{util::generic_id_invalid(), nullptr})
{
}


Entity::Entity(Core::World &world)
: m_impl(new Impl{util::generic_id_invalid(), nullptr})
{
  m_impl->world = world.get_world_data();
  
  bool success = true;
 
  if(m_impl->world)
  {
    // Create Entity record
    {
      auto entity_data = m_impl->world->entity;
      
      World_data::data_lock(entity_data);
      
      const util::generic_id id = World_data::entity_data_push_back(entity_data);
      m_impl->id = id;
      
      if(id && success)
      {
        // Zero the data.
        const uint32_t zero(0);
        World_data::entity_data_set_property_components(entity_data, id, zero);
        World_data::entity_data_set_property_tag(entity_data, id, zero);
        World_data::entity_data_set_property_renderer(entity_data, id, zero);
        
        const char *nilstr = "";
        World_data::entity_data_set_property_name(entity_data, id, nilstr);
        
      }
      else
      {
        success = false;
      }
      
      World_data::data_unlock(entity_data);
    }
    
    // Create Transform record
    {
      auto transform_data = m_impl->world->transform;
      
      World_data::data_lock(transform_data);
      
      const util::generic_id entity_id = m_impl->id;
    
      if(World_data::transform_data_push_back(transform_data, entity_id) && success)
      {
        const math::transform trans{};
        World_data::transform_data_set_property_transform(transform_data, entity_id, trans);
        
        const math::aabb bounding_box{};
        World_data::transform_data_set_property_aabb(transform_data, entity_id, bounding_box);
      }
      
      World_data::data_unlock(transform_data);
    }
    
    // Create Mesh record
    {
      auto mesh_data = m_impl->world->mesh_data;
      
      World_data::data_lock(mesh_data);
      
//      if(World_data::renderer_mesh_data_insert(mesh_data, id, 0) && success)
      {
      }
      
      World_data::data_unlock(mesh_data);
    }
    
    // If we failed then destroy the entity.
    if(!success)
    {
      LOG_ERROR(Error_string::entity_failed_to_construct());
      destroy();
    }
  }
  else
  {
    LOG_ERROR(Error_string::entity_is_invalid());
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
  Entity_detail::destroy(m_impl->id, m_impl->world.get());
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
  return Entity_detail::is_valid(m_impl->id,
                                 m_impl->world.get());
}


uint32_t
Entity::get_tags() const
{
  return Entity_detail::get_tags(m_impl->id,
                                 m_impl->world.get());
}


void
Entity::set_user_data(const uintptr_t user_data)
{
  Entity_detail::set_user_data(m_impl->id,
                               m_impl->world.get(),
                               user_data);
}


uintptr_t
Entity::get_user_data() const
{
  return Entity_detail::get_user_data(m_impl->id,
                                      m_impl->world.get());
}


bool
Entity::has_tag(const uint32_t tag_id) const
{
  return Entity_detail::has_tag(m_impl->id,
                                m_impl->world.get(),
                                tag_id);
}


void
Entity::set_tags(const uint32_t set_tags)
{
  Entity_detail::set_tags(m_impl->id,
                          m_impl->world.get(),
                          set_tags);
}


void
Entity::add_tag(const uint32_t add_tag)
{
  Entity_detail::add_tag(m_impl->id,
                         m_impl->world.get(),
                         add_tag);
}


void
Entity::remove_tag(const uint32_t tag)
{
  Entity_detail::remove_tag(m_impl->id,
                            m_impl->world.get(),
                            tag);
}


void
Entity::set_name(const char* set_name)
{
  Entity_detail::set_name(m_impl->id,
                          m_impl->world.get(),
                          set_name);
}


const char*
Entity::get_name() const
{
  return Entity_detail::get_name(m_impl->id,
                                 m_impl->world.get());
}


void
Entity::set_transform(const Transform &transform)
{
  Entity_detail::set_transform(m_impl->id,
                               m_impl->world.get(),
                               transform);
}


Transform
Entity::get_transform() const
{
  return Entity_detail::get_transform(m_impl->id,
                                      m_impl->world.get());
}


void
Entity::set_renderer(const Core::Renderer &renderer)
{
  Entity_detail::set_renderer(m_impl->id, m_impl->world.get(), renderer);
}


Renderer
Entity::get_renderer() const
{
  return Entity_detail::get_renderer(m_impl->id, m_impl->world.get());
}


void
Entity::set_collider(const Core::Collider &collider)
{
  Entity_detail::set_collider(m_impl->id,
                              m_impl->world.get(),
                              collider);
}


Core::Collider
Entity::get_collider() const
{
  return Entity_detail::get_collider(m_impl->id,
                                     m_impl->world.get());
}


void
Entity::set_rigidbody_properties(const Core::Rigidbody_properties rb_props)
{
  Entity_detail::set_rigidbody_properties(m_impl->id,
                                          m_impl->world.get(),
                                          rb_props);
}


Core::Rigidbody_properties
Entity::set_rigidbody_properties() const
{
  return Entity_detail::get_rigidbody_properties(m_impl->id,
                                                 m_impl->world.get());
}


util::generic_id
Entity::get_id() const
{
  return Entity_detail::get_id(m_impl->id,
                               m_impl->world.get());
}


std::shared_ptr<const World_data::World>
Entity::get_world_data() const
{
  return m_impl->world;
}


std::shared_ptr<World_data::World>
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


Entity::operator util::generic_id() const
{
  return get_id();
}


} // ns