#include <core/entity/entity_interface.hpp>
#include <core/entity/entity.hpp>
#include <core/entity/entity_ref.hpp>
#include <core/entity/detail/entity_id.hpp>

#include <core/transform/transform.hpp>
#include <core/renderer/renderer.hpp>
#include <core/physics/rigidbody.hpp>
#include <core/world/world.hpp>
#include <core/world/detail/world_index.hpp>

#include <data/world_data/pending_entity_removal.hpp>
#include <data/world_data/world_pools.hpp>
#include <data/world_data/entity_data.hpp>
#include <data/world_data/transform_data.hpp>
#include <data/world_data/renderer_mesh_data.hpp>

#include <transformations/entity/entity_common.hpp>
#include <transformations/entity/entity_data.hpp>
#include <transformations/entity/entity_transform.hpp>
#include <transformations/entity/entity_rigidbody.hpp>
#include <transformations/entity/entity_renderer.hpp>

#include <common/error_strings.hpp>
#include <utilities/logging.hpp>


namespace
{
  std::atomic<uint32_t> entity_instance_counter(0);
}


namespace Core {


struct Entity_interface::Impl
{
//  util::generic_id id;
  Core_detail::Entity_id id;
  mutable std::shared_ptr<World_data::World> world;
};


Entity_interface::Entity_interface()
: m_impl(new Impl{Core_detail::entity_id_invalid(), nullptr})
{
}


Entity_interface::Entity_interface(const Entity_interface &other)
: m_impl(new Impl)
{
  copy(other);
}


Entity_interface::Entity_interface(Entity_interface &&other)
: m_impl(new Impl)
{
  move(other);
}

Entity_interface::Entity_interface(const Core_detail::Entity_id id)
: m_impl(new Impl{id, Core_detail::world_index_get_world_data(id.world_instance)})
{
}

Entity_interface::Entity_interface(Core::World &world)
: m_impl(new Impl{Core_detail::entity_id_invalid(), nullptr})
{
  m_impl->world = world.get_world_data();
  
  bool success = true;
 
  if(m_impl->world)
  {
    // Create Entity record
    {
      auto entity_data = m_impl->world->entity;
      
      World_data::data_lock(entity_data);
      
      entity_instance_counter++;
      Core_detail::Entity_id entity_id;
      
      entity_id.entity_instance = entity_instance_counter;
      entity_id.world_instance  = world.get_id();
      const uint32_t id = Core_detail::entity_id_to_uint(entity_id);
      m_impl->id = entity_id;
      
      const bool added = World_data::entity_data_push_back(entity_data, id);
      
      if(added && success)
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
      
      Data::data_lock(transform_data);
      
      const util::generic_id entity_id = Core_detail::entity_id_to_uint(m_impl->id);
    
      if(Data::transform_insert(transform_data, entity_id))
      {
        const math::transform trans{};
        Data::transform_set_transform(transform_data, entity_id, &trans);
        
        const math::aabb bounding_box{};
        Data::transform_set_aabb(transform_data, entity_id, &bounding_box);
      }
      else
      {
        success = false;
      }
      
      Data::data_unlock(transform_data);
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
      m_impl->id = Core_detail::entity_id_invalid();
      destroy();
    }
  }
  else
  {
    LOG_ERROR(Error_string::entity_is_invalid());
  }
}


Entity_interface::~Entity_interface()
{
}


// ** Life time ** //


void
Entity_interface::destroy()
{
  if(!m_impl || !m_impl->world)
  {
    return;
  }
  
  Entity_detail::destroy(Core_detail::entity_id_to_uint(m_impl->id),
                         m_impl->world->entity,
                         m_impl->world->entity_removal);
  
  m_impl->id = Core_detail::entity_id_invalid();
}


bool
Entity_interface::is_valid() const
{
  if(!m_impl || !Core_detail::entity_id_to_uint(m_impl->id) || !m_impl->world)
  {
    return false;
  }

  return Entity_detail::is_valid(Core_detail::entity_id_to_uint(m_impl->id), m_impl->world->entity);
}


Entity_interface::operator bool() const
{
  return is_valid();
}


// ** General Interface ** //


uint32_t
Entity_interface::get_tags() const
{
  return Entity_detail::get_tags(Core_detail::entity_id_to_uint(m_impl->id),
                                 m_impl->world->entity);
}


void
Entity_interface::set_user_data(const uintptr_t user_data)
{
  Entity_detail::set_user_data(Core_detail::entity_id_to_uint(m_impl->id),
                               m_impl->world->entity,
                               user_data);
}


uintptr_t
Entity_interface::get_user_data() const
{
  return Entity_detail::get_user_data(Core_detail::entity_id_to_uint(m_impl->id),
                                      m_impl->world->entity);
}


bool
Entity_interface::has_tag(const uint32_t tag_id) const
{
  return Entity_detail::has_tag(Core_detail::entity_id_to_uint(m_impl->id),
                                m_impl->world->entity,
                                tag_id);
}


void
Entity_interface::set_tags(const uint32_t set_tags)
{
  Entity_detail::set_tags(Core_detail::entity_id_to_uint(m_impl->id),
                          m_impl->world->entity,
                          set_tags);
}


void
Entity_interface::add_tag(const uint32_t add_tag)
{
  Entity_detail::add_tag(Core_detail::entity_id_to_uint(m_impl->id),
                         m_impl->world->entity,
                         add_tag);
}


void
Entity_interface::remove_tag(const uint32_t tag)
{
  Entity_detail::remove_tag(Core_detail::entity_id_to_uint(m_impl->id),
                            m_impl->world->entity,
                            tag);
}


void
Entity_interface::set_name(const char* set_name)
{
  Entity_detail::set_name(Core_detail::entity_id_to_uint(m_impl->id),
                          m_impl->world->entity,
                          set_name);
}


const char*
Entity_interface::get_name() const
{
  return Entity_detail::get_name(Core_detail::entity_id_to_uint(m_impl->id),
                                 m_impl->world->entity);
}


void
Entity_interface::set_transform(const Transform &transform)
{
  Entity_detail::set_transform(Core_detail::entity_id_to_uint(m_impl->id),
                               m_impl->world->entity,
                               m_impl->world->transform,
                               m_impl->world->rigidbody_data,
                               m_impl->world->mesh_data,
                               m_impl->world->text_data,
                               transform);
}


Transform
Entity_interface::get_transform() const
{
  return Entity_detail::get_core_transform(Core_detail::entity_id_to_uint(m_impl->id),
                                           m_impl->world->entity,
                                           m_impl->world->transform);
}


void
Entity_interface::set_renderer(const Core::Renderer &renderer)
{
  Entity_detail::set_renderer(Core_detail::entity_id_to_uint(m_impl->id),
                              m_impl->world->entity,
                              m_impl->world->transform,
                              m_impl->world->mesh_data,
                              m_impl->world->text_data,
                              renderer);
}


Renderer
Entity_interface::get_renderer() const
{
  return Entity_detail::get_renderer(Core_detail::entity_id_to_uint(m_impl->id), m_impl->world->entity, m_impl->world->mesh_data);
}


void
Entity_interface::set_rigidbody(const Rigidbody &rigidbody)
{
  Entity_detail::set_rigidbody(Core_detail::entity_id_to_uint(m_impl->id), m_impl->world.get(), rigidbody);
}


Rigidbody
Entity_interface::get_rigidbody() const
{
  return Entity_detail::get_rigidbody(Core_detail::entity_id_to_uint(m_impl->id));
}


uint32_t
Entity_interface::get_id() const
{
  return Core_detail::entity_id_to_uint(m_impl->id);
//  return Entity_detail::get_id(Core_detail::entity_id_to_uint(m_impl->id), m_impl->world->entity);
}


// ** Equality ** //


bool
Entity_interface::operator==(const Entity_interface &other) const
{
  return this->get_id() == other.get_id();
}


bool
Entity_interface::operator!=(const Entity_interface &other) const
{
  return this->get_id() != other.get_id();
}


// ** Protected/private utilities ** //


void
Entity_interface::copy(const Entity_interface &other)
{
  const Core_detail::Entity_id id = Core_detail::entity_id_from_uint(other.get_id());
  auto world_data = Core_detail::world_index_get_world_data(id.world_instance);
  
  m_impl.reset(new Impl{id, world_data});
}


void
Entity_interface::move(Entity_interface &other)
{
  copy(other);
  
  other.m_impl.reset();
}


} // ns