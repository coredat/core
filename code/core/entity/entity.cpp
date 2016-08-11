#include <core/entity/entity.hpp>
#include <core/entity/entity_ref.hpp>
#include <transformations/entity/entity_common.hpp>
#include <transformations/entity/entity_data.hpp>
#include <transformations/entity/entity_transform.hpp>
#include <transformations/entity/entity_rigidbody.hpp>
#include <transformations/entity/entity_renderer.hpp>
#include <core/transform/transform.hpp>
#include <core/model/model.hpp>
#include <core/renderer/renderer.hpp>
#include <core/renderer/material_renderer.hpp>
#include <core/resources/material.hpp>
#include <core/physics/collider.hpp>
#include <core/physics/rigidbody.hpp>
#include <core/world/world.hpp>
#include <data/world_data/pending_scene_graph_change_data.hpp>
#include <data/world_data/world_pools.hpp>
#include <data/world_data/entity_data.hpp>
#include <data/world_data/transform_data.hpp>
#include <data/world_data/renderer_mesh_data.hpp>
#include <common/error_strings.hpp>
#include <utilities/logging.hpp>


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
  LOG_TODO_ONCE("Make entity interface auto generated.");

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
    if(success)
    {
      auto transform_data = m_impl->world->transform;
      
      World_data::data_lock(transform_data);
      
      const util::generic_id entity_id = m_impl->id;
    
      if(World_data::transform_data_push_back(transform_data, entity_id))
      {
        const math::transform trans{};
        World_data::transform_data_set_property_transform(transform_data, entity_id, trans);
        
        const math::aabb bounding_box{};
        World_data::transform_data_set_property_aabb(transform_data, entity_id, bounding_box);
      }
      else
      {
        success = false;
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
      m_impl->id = util::generic_id_invalid();
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
  Entity_detail::destroy(m_impl->id,
                         m_impl->world->entity,
                         m_impl->world->entity_graph_changes);
  
  util::generic_id *id = const_cast<util::generic_id*>(&m_impl->id);
  *id = util::generic_id_invalid();
}

// ** Common Entity Interface ** //


bool
Entity::is_valid() const
{
  if(!m_impl || !m_impl->id || !m_impl->world)
  {
    return false;
  }

  return Entity_detail::is_valid(m_impl->id, m_impl->world->entity);
}


Entity::operator bool() const
{
  return is_valid();
}


uint32_t
Entity::get_tags() const
{
  return Entity_detail::get_tags(m_impl->id,
                                 m_impl->world->entity);
}


void
Entity::set_user_data(const uintptr_t user_data)
{
  Entity_detail::set_user_data(m_impl->id,
                               m_impl->world->entity,
                               user_data);
}


uintptr_t
Entity::get_user_data() const
{
  return Entity_detail::get_user_data(m_impl->id,
                                      m_impl->world->entity);
}


bool
Entity::has_tag(const uint32_t tag_id) const
{
  return Entity_detail::has_tag(m_impl->id,
                                m_impl->world->entity,
                                tag_id);
}


void
Entity::set_tags(const uint32_t set_tags)
{
  Entity_detail::set_tags(m_impl->id,
                          m_impl->world->entity,
                          set_tags);
}


void
Entity::add_tag(const uint32_t add_tag)
{
  Entity_detail::add_tag(m_impl->id,
                         m_impl->world->entity,
                         add_tag);
}


void
Entity::remove_tag(const uint32_t tag)
{
  Entity_detail::remove_tag(m_impl->id,
                            m_impl->world->entity,
                            tag);
}


void
Entity::set_name(const char* set_name)
{
  Entity_detail::set_name(m_impl->id,
                          m_impl->world->entity,
                          set_name);
}


const char*
Entity::get_name() const
{
  return Entity_detail::get_name(m_impl->id,
                                 m_impl->world->entity);
}


void
Entity::set_transform(const Transform &transform)
{
  Entity_detail::set_transform(m_impl->id,
                               m_impl->world->entity,
                               m_impl->world->transform,
                               m_impl->world->physics_data,
                               m_impl->world->mesh_data,
                               m_impl->world->text_data,
                               transform);
}


Transform
Entity::get_transform() const
{
  return Entity_detail::get_core_transform(m_impl->id,
                                           m_impl->world->entity,
                                           m_impl->world->transform);
}


void
Entity::set_renderer(const Core::Renderer &renderer)
{
  Entity_detail::set_renderer(m_impl->id,
                              m_impl->world->entity,
                              m_impl->world->transform,
                              m_impl->world->mesh_data,
                              m_impl->world->text_data,
                              renderer);
}


Renderer
Entity::get_renderer() const
{
  return Entity_detail::get_renderer(m_impl->id, m_impl->world->entity, m_impl->world->mesh_data);
}


void
Entity::set_rigidbody(const Rigidbody &rigidbody)
{
  Entity_detail::set_rigidbody(m_impl->id, m_impl->world.get(), rigidbody);
}


Rigidbody
Entity::get_rigidbody() const
{
  return Entity_detail::get_rigidbody(m_impl->id, m_impl->world.get());
}


util::generic_id
Entity::get_id() const
{
  return Entity_detail::get_id(m_impl->id, m_impl->world->entity);
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