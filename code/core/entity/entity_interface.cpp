#include <core/entity/entity_interface.hpp>
#include <core/entity/entity.hpp>
#include <core/entity/entity_ref.hpp>
#include <core/entity/detail/entity_id.hpp>

#include <core/transform/transform.hpp>
#include <core/renderer/renderer.hpp>
#include <core/physics/rigidbody.hpp>
#include <core/world/world.hpp>
#include <core/world/detail/world_index.hpp>

#include <data/world/pending_entity_removal_data.hpp>
#include <data/world_data.hpp>
#include <data/world/entity_data.hpp>
#include <data/world/transform_data.hpp>
#include <data/world/mesh_draw_call_data.hpp>

#include <common/data_types.hpp>

#include <transformations/entity/entity_common.hpp>
#include <transformations/entity/entity_data.hpp>
#include <transformations/entity/entity_transform.hpp>
#include <transformations/entity/entity_rigidbody.hpp>
#include <transformations/entity/entity_renderer.hpp>
#include <transformations/entity/entity_callback.hpp>

#include <common/error_strings.hpp>
#include <utilities/logging.hpp>


namespace
{
  std::atomic<uint32_t> entity_instance_counter(0);
}


namespace Core {


Entity_interface::Entity_interface()
{
}


Entity_interface::Entity_interface(const Entity_interface &other)
{
  copy(other);
}


Entity_interface::Entity_interface(Entity_interface &&other)
{
  move(other);
}

Entity_interface::Entity_interface(const Core_detail::Entity_id id)
: m_id(Core_detail::entity_id_to_uint(id))
{
}

Entity_interface::Entity_interface(Core::World &world)
: m_id(0)
{
  bool success = true;
 
  auto world_data = Core_detail::world_index_get_world_data(world.get_id());
 
  if(world_data)
  {
    // Create Entity record
    {
      auto entity_data = world_data->entity;
      
      Data::data_lock(entity_data);
      
      entity_instance_counter++;
      Core_detail::Entity_id entity_id;
      
      entity_id.entity_instance = entity_instance_counter;
      entity_id.world_instance  = world.get_id();
      m_id = Core_detail::entity_id_to_uint(entity_id);
      
      const bool added = Data::entity_push(entity_data, m_id);
      
      if(added && success)
      {
        // Zero the data.
        const uint32_t has_transform(Common::Data_type::transform);
        const uint32_t zero(0);
        
        Data::entity_set_components(entity_data, m_id, &has_transform);
        Data::entity_set_tags(entity_data, m_id, &zero);
        
        const char *nilstr = "";
        Data::entity_set_name(entity_data, m_id, nilstr, 0);
      }
      else
      {
        success = false;
      }
      
      Data::data_unlock(entity_data);
    }
    
    // Create Transform record
    if(success)
    {
      auto transform_data = world_data->transform;
      
      Data::data_lock(transform_data);
      
      if(Data::transform_push(transform_data, m_id))
      {
        const math::transform trans{};
        Data::transform_set_transform(transform_data, m_id, &trans);
        
        const math::aabb bounding_box{};
        Data::transform_set_aabb(transform_data, m_id, &bounding_box);
      }
      else
      {
        success = false;
      }
      
      Data::data_unlock(transform_data);
    }
    
    // If we failed then destroy the entity.
    if(!success)
    {
      LOG_ERROR(Error_string::entity_failed_to_construct());
      destroy();
      m_id = 0;
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
  if(!m_id)
  {
    return;
  }
  
  const Core_detail::Entity_id entity_id = Core_detail::entity_id_from_uint(m_id);
  auto world_data = Core_detail::world_index_get_world_data(entity_id.world_instance);
  
  Entity_detail::destroy(m_id,
                         world_data->entity,
                         world_data->entity_removal);
  
  m_id = 0;
}


bool
Entity_interface::is_valid() const
{
  if(!m_id)
  {
    return false;
  }
  
  const Core_detail::Entity_id entity_id = Core_detail::entity_id_from_uint(m_id);
  auto world_data = Core_detail::world_index_get_world_data(entity_id.world_instance);
  
  if(!world_data)
  {
    return false;
  }
  
  return Entity_detail::is_valid(m_id, world_data->entity);
}


Entity_interface::operator bool() const
{
  return is_valid();
}


// ** General Interface ** //


uint32_t
Entity_interface::get_tags() const
{
  // Validity check
  if(!m_id)
  {
    return 0;
  }

  const Core_detail::Entity_id entity_id = Core_detail::entity_id_from_uint(m_id);
  auto world_data = Core_detail::world_index_get_world_data(entity_id.world_instance);
  
  return Entity_detail::get_tags(m_id,
                                 world_data->entity);
}


void
Entity_interface::set_user_data(const uintptr_t user_data)
{
  // Validity check
  if(!m_id)
  {
    return;
  }

  const Core_detail::Entity_id entity_id = Core_detail::entity_id_from_uint(m_id);
  auto world_data = Core_detail::world_index_get_world_data(entity_id.world_instance);
  
  Entity_detail::set_user_data(m_id,
                               world_data->entity,
                               user_data);
}


uintptr_t
Entity_interface::get_user_data() const
{
  // Validity check
  if(!m_id)
  {
    return 0;
  }

  const Core_detail::Entity_id entity_id = Core_detail::entity_id_from_uint(m_id);
  auto world_data = Core_detail::world_index_get_world_data(entity_id.world_instance);
  
  return Entity_detail::get_user_data(m_id,
                                      world_data->entity);
}


bool
Entity_interface::has_tag(const uint32_t tag_id) const
{
  // Validity check
  if(!m_id)
  {
    return false;
  }

  const Core_detail::Entity_id entity_id = Core_detail::entity_id_from_uint(m_id);
  auto world_data = Core_detail::world_index_get_world_data(entity_id.world_instance);
  
  return Entity_detail::has_tag(m_id,
                                world_data->entity,
                                tag_id);
}


void
Entity_interface::set_tags(const uint32_t set_tags)
{
  // Validity check
  if(!m_id)
  {
    return;
  }

  const Core_detail::Entity_id entity_id = Core_detail::entity_id_from_uint(m_id);
  auto world_data = Core_detail::world_index_get_world_data(entity_id.world_instance);
  
  Entity_detail::set_tags(m_id,
                          world_data->entity,
                          set_tags);
}


void
Entity_interface::add_tag(const uint32_t add_tag)
{
  // Validity check
  if(!m_id)
  {
    return;
  }

  const Core_detail::Entity_id entity_id = Core_detail::entity_id_from_uint(m_id);
  auto world_data = Core_detail::world_index_get_world_data(entity_id.world_instance);
  
  Entity_detail::add_tag(m_id,
                         world_data->entity,
                         add_tag);
}


void
Entity_interface::remove_tag(const uint32_t tag)
{
  // Validity check
  if(!m_id)
  {
    return;
  }

  const Core_detail::Entity_id entity_id = Core_detail::entity_id_from_uint(m_id);
  auto world_data = Core_detail::world_index_get_world_data(entity_id.world_instance);
  
  Entity_detail::remove_tag(m_id,
                            world_data->entity,
                            tag);
}


void
Entity_interface::set_name(const char *set_name)
{
  // Validity check
  if(!m_id)
  {
    return;
  }

  const Core_detail::Entity_id entity_id = Core_detail::entity_id_from_uint(m_id);
  auto world_data = Core_detail::world_index_get_world_data(entity_id.world_instance);
  
  Entity_detail::set_name(m_id,
                          world_data->entity,
                          set_name);
}


const char*
Entity_interface::get_name() const
{
  // Validity check
  if(!m_id)
  {
    return "";
  }

  const Core_detail::Entity_id entity_id = Core_detail::entity_id_from_uint(m_id);
  auto world_data = Core_detail::world_index_get_world_data(entity_id.world_instance);
  
  return Entity_detail::get_name(m_id,
                                 world_data->entity);
}


uint32_t
Entity_interface::get_id() const
{
  return m_id;
}


// ** Callbacks ** //


void
Entity_interface::on_collision_callback(const uintptr_t user_data, const on_collision_callback_fn &callback)
{
  // Validity check
  if(!m_id)
  {
    return;
  }

  const Core_detail::Entity_id entity_id = Core_detail::entity_id_from_uint(m_id);
  auto world_data = Core_detail::world_index_get_world_data(entity_id.world_instance);
  
  Entity_detail::set_entity_collision_callback(m_id,
                                               world_data->entity,
                                               (uintptr_t)callback,
                                               user_data);
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
  m_id = other.m_id;
}


void
Entity_interface::move(Entity_interface &other)
{
  copy(other);
  
  other.m_id = 0;
}


} // ns