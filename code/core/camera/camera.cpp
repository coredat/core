#include <core/camera/camera.hpp>
#include <core/world/world.hpp>
#include <core/world/detail/world_detail.hpp>
#include <core/color/color.hpp>
#include <core/entity/entity.hpp>
#include <core/entity/entity_ref.hpp>
#include <core/camera/camera_properties.hpp>
#include <transformations/camera/cam_priorities.hpp>
#include <data/world_data/world.hpp>
#include <data/world_data/world_pools.hpp>
#include <data/world_data/camera_data.hpp>
#include <systems/camera/camera_properties.hpp>
#include <common/error_strings.hpp>
#include <math/mat/mat4.hpp>
#include <utilities/logging.hpp>


namespace Core {


struct Camera::Impl
{
  util::generic_id camera_id = util::generic_id_invalid();
  ::Camera::Camera_properties properties;
  std::shared_ptr<World_detail::Data> world;
};


Camera::Camera()
: m_impl(new Impl)
{
  LOG_WARNING("Created Core::Camera with no world.");
}


Camera::Camera(Core::World &world)
: m_impl(new Impl)
{
  m_impl->world = world.get_world_data();
  
  // Creaet a new camera.
  {
    auto cam_data = m_impl->world->data.camera_data;
    
    World_data::data_lock(cam_data);
    
    // Push and set the defaults.
    if(World_data::camera_data_push_back(cam_data, cam_data->size + 1))
    {
      m_impl->camera_id = cam_data->size;
      World_data::camera_data_set_property_entity_id(cam_data, m_impl->camera_id, util::generic_id_invalid());
      World_data::camera_data_set_property_camera(cam_data, m_impl->camera_id, ::Camera::Camera_properties{});
      
      const uint32_t priority = Camera_utils::find_highest_priority(cam_data->property_priority, cam_data->size);
      World_data::camera_data_set_property_priority(cam_data, m_impl->camera_id, priority + 1);
    }
    
    World_data::data_unlock(cam_data);
  }

}


Camera::~Camera()
{
  set_attached_entity(Entity_ref()); // Passing an invalid entity will unload the current entity.
}


Camera::Camera(Camera &&other)
: m_impl(new Impl{
  other.m_impl->camera_id,
  other.m_impl->properties,
  other.m_impl->world
})
{
  // Null the other stuff
  other.m_impl->properties = ::Camera::Camera_properties();
}


Camera&
Camera::operator=(Camera &&other)
{
  // Copy other.
  m_impl->camera_id  = other.m_impl->camera_id;
  m_impl->properties = other.m_impl->properties;
  m_impl->world      = other.m_impl->world;

  // Null the other stuff
  other.m_impl->camera_id  = util::generic_id_invalid();
  other.m_impl->properties = ::Camera::Camera_properties();

  return *this;
}


void
Camera::set_attached_entity(Entity_ref entity)
{
  if(!m_impl || !m_impl->camera_id)
  {
    LOG_ERROR(Error_string::object_has_no_valid_world());
    return;
  }

  // Set the attached entity.
  {
    auto cam_data = m_impl->world->data.camera_data;
    
    World_data::data_lock(cam_data);
    
    World_data::camera_data_set_property_entity_id(cam_data, m_impl->camera_id, entity);
    
    World_data::data_unlock(cam_data);
  }
}


Entity_ref
Camera::get_attached_entity() const
{
  if(!m_impl || !m_impl->camera_id)
  {
    LOG_ERROR(Error_string::object_has_no_valid_world());
    return Entity_ref();
  }
  
  util::generic_id entity_id = util::generic_id_invalid();
  {
    auto cam_data = m_impl->world->data.camera_data;
    
    World_data::data_lock(cam_data);
    World_data::camera_data_get_property_entity_id(cam_data, m_impl->camera_id, &entity_id);
    World_data::data_unlock(cam_data);
  }
  
  Entity_ref entity(entity_id, m_impl->world);
  
  return entity;
}


void
Camera::set_priority(const uint32_t priority)
{
  if(!m_impl || !m_impl->camera_id)
  {
    LOG_ERROR(Error_string::object_has_no_valid_world());
    return;
  }
  
  auto cam_data = m_impl->world->data.camera_data;
  const auto cam_id = m_impl->camera_id;
  
  World_data::data_lock(cam_data);
  
  // Check priority is different.
  uint32_t old_priority;
  World_data::camera_data_get_property_priority(cam_data, cam_id, &old_priority);
  
  if(old_priority != priority)
  {
    // Get the rest of the data.
    ::Camera::Camera_properties props;
    World_data::camera_data_get_property_camera(cam_data, cam_id, &props);
    
    util::generic_id attached_entity;
    World_data::camera_data_get_property_entity_id(cam_data, cam_id, &attached_entity);
    
    // Remove old data.
    World_data::camera_data_erase(cam_data, cam_id);
  
    // Insert the new data
    const size_t insert_at = Camera_utils::find_insert_point_based_on_priority(priority, cam_data->property_priority, cam_data->size);
    if(World_data::camera_data_insert(cam_data, cam_id, insert_at))
    {
      World_data::camera_data_set_property_camera(cam_data, cam_id, props);
      World_data::camera_data_set_property_entity_id(cam_data, cam_id, attached_entity);
      World_data::camera_data_set_property_priority(cam_data, cam_id, priority);
    }
    else
    {
      LOG_ERROR(Error_string::failed_to_create_resource());
    }
  }
  
  World_data::data_unlock(cam_data);
}


uint32_t
Camera::get_priority() const
{
  uint32_t priority = 0;
  {
    auto cam_data = m_impl->world->data.camera_data;
    
    World_data::data_lock(cam_data);
    if(!World_data::camera_data_get_property_priority(cam_data, m_impl->camera_id, &priority))
    {
      LOG_ERROR(Error_string::failed_to_find_resource());
    }
    World_data::data_unlock(cam_data);
  }
  
  return priority;
}


void
Camera::set_type(const Core::Camera_type cam_type)
{
  if(!m_impl || !m_impl->camera_id)
  {
    LOG_ERROR(Error_string::object_has_no_valid_world());
    return;
  }

  // Set the attached entity.
  {
    auto cam_data = m_impl->world->data.camera_data;
    const auto cam_id = m_impl->camera_id;
    
    World_data::data_lock(cam_data);
    
    ::Camera::Camera_properties props;
    World_data::camera_data_get_property_camera(cam_data, cam_id, &props);
    
    props.type = cam_type;
    World_data::camera_data_set_property_camera(cam_data, cam_id, props);
    
    World_data::data_unlock(cam_data);
  }
}


Camera_type
Camera::get_type() const
{
  if(!m_impl || !m_impl->camera_id)
  {
    LOG_ERROR(Error_string::object_has_no_valid_world());
    return Camera_type::perspective;
  }
  
  ::Camera::Camera_properties props;

  // Set the attached entity.
  {
    auto cam_data = m_impl->world->data.camera_data;
    const auto cam_id = m_impl->camera_id;
    
    World_data::data_lock(cam_data);
    
    ::Camera::Camera_properties props;
    World_data::camera_data_get_property_camera(cam_data, cam_id, &props);
    
    World_data::data_unlock(cam_data);
  }
  
  return props.type;
}


void
Camera::set_clear_flags(const uint32_t flags)
{
  if(!m_impl || !m_impl->camera_id)
  {
    LOG_ERROR(Error_string::object_has_no_valid_world());
    return;
  }

  // Set the attached entity.
  {
    auto cam_data = m_impl->world->data.camera_data;
    const auto cam_id = m_impl->camera_id;
    
    World_data::data_lock(cam_data);
    
    ::Camera::Camera_properties props;
    World_data::camera_data_get_property_camera(cam_data, cam_id, &props);
    
    props.clear_flags = flags;
    World_data::camera_data_set_property_camera(cam_data, cam_id, props);
    
    World_data::data_unlock(cam_data);
  }
}


bool
Camera::will_clear_color() const
{
  return m_impl->properties.clear_flags & Camera_clear::color;
}


bool
Camera::will_clear_depth() const
{
  return m_impl->properties.clear_flags & Camera_clear::depth;
}


uint32_t
Camera::get_clear_flags() const
{
  return m_impl->properties.clear_flags;
}


void
Camera::set_width(const uint32_t width)
{
  if(!m_impl || !m_impl->camera_id)
  {
    LOG_ERROR(Error_string::object_has_no_valid_world());
    return;
  }

  // Set the attached entity.
  {
    auto cam_data = m_impl->world->data.camera_data;
    const auto cam_id = m_impl->camera_id;
    
    World_data::data_lock(cam_data);
    
    ::Camera::Camera_properties props;
    World_data::camera_data_get_property_camera(cam_data, cam_id, &props);
    
    props.viewport_width = width;
    World_data::camera_data_set_property_camera(cam_data, cam_id, props);
    
    World_data::data_unlock(cam_data);
  }
}


uint32_t
Camera::get_width() const
{
  return m_impl->properties.viewport_width;
}


void
Camera::set_height(const uint32_t height)
{
  if(!m_impl || !m_impl->camera_id)
  {
    LOG_ERROR(Error_string::object_has_no_valid_world());
    return;
  }

  // Set the attached entity.
  {
    auto cam_data = m_impl->world->data.camera_data;
    const auto cam_id = m_impl->camera_id;
    
    World_data::data_lock(cam_data);
    
    ::Camera::Camera_properties props;
    World_data::camera_data_get_property_camera(cam_data, cam_id, &props);
    
    props.viewport_height = height;
    World_data::camera_data_set_property_camera(cam_data, cam_id, props);
    
    World_data::data_unlock(cam_data);
  }
}


uint32_t
Camera::get_height() const
{
  return m_impl->properties.viewport_height;
}


void
Camera::set_feild_of_view(const float fov_radians)
{
  m_impl->properties.fov = fov_radians;

  if(m_impl->attached_entity != util::generic_id_invalid())
  {
    update_properties(m_impl->properties, m_impl->attached_entity);
  }
}


float
Camera::get_field_of_view() const
{
  return m_impl->properties.fov;
}


void
Camera::set_near_plane(const float near_plane)
{
  m_impl->properties.near_plane = near_plane;

  if(m_impl->attached_entity != util::generic_id_invalid())
  {
    update_properties(m_impl->properties, m_impl->attached_entity);
  }
}


float
Camera::get_near_plane() const
{
  return m_impl->properties.near_plane;
}


void
Camera::set_far_plane(const float far_plane)
{
  m_impl->properties.far_plane = far_plane;

  if(m_impl->attached_entity != util::generic_id_invalid())
  {
    update_properties(m_impl->properties, m_impl->attached_entity);
  }
}


float
Camera::get_far_plane() const
{
  return m_impl->properties.far_plane;
}


void
Camera::set_clear_color(const Core::Color color)
{
  m_impl->properties.clear_color = color.get_color();

  if(m_impl->attached_entity != util::generic_id_invalid())
  {
    update_properties(m_impl->properties, m_impl->attached_entity);
  }
}


Color
Camera::get_clear_color() const
{
  return Color(m_impl->properties.clear_color);
}


}; // ns