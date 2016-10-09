#include <core/camera/camera.hpp>
#include <core/entity/detail/entity_id.hpp>
#include <core/world/detail/world_index.hpp>
#include <core/world/world.hpp>
#include <core/color/color.hpp>
#include <core/entity/entity.hpp>
#include <core/entity/entity_ref.hpp>
#include <core/resources/render_target.hpp>
#include <core/camera/camera_properties.hpp>
#include <core/camera/post_process.hpp>
#include <transformations/camera/cam_priorities.hpp>
#include <data/world_data/world_pools.hpp>
#include <data/world_data/camera_data.hpp>
#include <systems/camera/camera_properties.hpp>
#include <systems/renderer_post/post_shader.hpp>
#include <common/error_strings.hpp>
#include <math/mat/mat4.hpp>
#include <utilities/logging.hpp>



/*
  <TODO>
    Splitting out the type of rendering would be good.
    like attaching a renderer to an entity. You could attach a 
    renderer to a camera. my_camera.set_renderer(post_process_renderer);
    This would elinate settings that only happen in certain modes.
    ie. only_render_tags() would only work for mesh_rendering.
    We can also use it to enable debug rendinering.
    my_camera.set_renderer(collider_renderer).
    my_camera.set_renderer(overdraw_renderer).
  </TODO>
*/


namespace Core {


struct Camera::Impl
{
  util::generic_id camera_id;
  ::Camera::Camera_properties properties;
  std::shared_ptr<World_data::World> world;
};


Camera::Camera()
: m_impl(new Impl)
{
  LOG_WARNING("Created Core::Camera with no world.");
}


namespace {


inline util::generic_id
create_new_camera(World_data::Camera_data *cam_data)
{
  World_data::data_lock(cam_data);
  
  // Push and set the defaults.
  util::generic_id id = cam_data->size + 1;
  World_data::camera_data_push_back(cam_data, id);
  
  if(id)
  {
    World_data::camera_data_set_property_entity_id(cam_data, id, util::generic_id_invalid());
    World_data::camera_data_set_property_camera(cam_data, id, ::Camera::Camera_properties{});
    
    const uint32_t priority = Camera_utils::find_highest_priority(cam_data->property_priority, cam_data->size);
    World_data::camera_data_set_property_priority(cam_data, id, priority + 1);
  }
  
  World_data::data_unlock(cam_data);
  
  return id;
}


}


Camera::Camera(Core::Entity_ref attach_entity,
               const uint32_t width,
               const uint32_t height,
               const float near_plane,
               const float far_plane,
               const float fov)
: m_impl(new Impl)
{
  const Core_detail::Entity_id id = Core_detail::entity_id_from_uint(attach_entity.get_id());
  m_impl->world = Core_detail::world_index_get_world_data(id.world_instance);

  auto cam_data = m_impl->world->camera_data;
  m_impl->camera_id = create_new_camera(cam_data);

  set_attached_entity(attach_entity);
  set_width(width);
  set_height(height);
  set_near_plane(near_plane);
  set_far_plane(far_plane);
  set_feild_of_view(fov);
}

Camera::Camera(Core::World &world)
: m_impl(new Impl)
{
  m_impl->world = world.get_world_data();
  
  auto cam_data = m_impl->world->camera_data;
  m_impl->camera_id = create_new_camera(cam_data);
}


Camera::~Camera()
{
  set_attached_entity(Entity_ref()); // Passing an invalid entity will unload the current entity.
  LOG_TODO_ONCE("Actually delete the camera from the data");
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
Camera::set_tags_to_render(const uint32_t tags)
{
  if(!m_impl || !m_impl->camera_id)
  {
    LOG_ERROR(Error_string::object_has_no_valid_world());
    return;
  }
  
  // Update data
  {
    auto cam_data = m_impl->world->camera_data;
    const auto cam_id = m_impl->camera_id;
    
    World_data::data_lock(cam_data);
    
    ::Camera::Camera_properties props;
    World_data::camera_data_get_property_camera(cam_data, cam_id, &props);
    
    props.cull_mask = tags;
    World_data::camera_data_set_property_camera(cam_data, cam_id, props);
    
    World_data::data_unlock(cam_data);
  }
}


namespace
{

inline ::Camera::Camera_properties
get_properties(World_data::Camera_data *cam_data, util::generic_id cam_id)
{
  ::Camera::Camera_properties props;

  World_data::data_lock(cam_data);

  World_data::camera_data_get_property_camera(cam_data, cam_id, &props);

  World_data::data_unlock(cam_data);
  
  return props;
}


}


uint32_t
Camera::get_tags_to_render() const
{
  if(!m_impl || !m_impl->camera_id)
  {
    LOG_ERROR(Error_string::object_has_no_valid_world());
    return 0;
  }
  
  return get_properties(m_impl->world->camera_data, m_impl->camera_id).cull_mask;
}


void
Camera::set_post_process(const Core::Post_process &post)
{
  if(!m_impl || !m_impl->camera_id)
  {
    LOG_ERROR(Error_string::object_has_no_valid_world());
    return;
  }
  
  // Get
  {
    auto cam_data = m_impl->world->camera_data;
    
    World_data::data_lock(cam_data);
    
    Post_renderer::Post_shader post_shader;
//    Post_renderer::create_post_shader();
    
    World_data::camera_data_set_property_post_process_id(cam_data, m_impl->camera_id, post.get_id());
    
    
    World_data::data_unlock(cam_data);
  }
}


Post_process
Camera::get_post_process() const
{
  assert(false);
  return Post_process("");
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
    auto cam_data = m_impl->world->camera_data;
    
    World_data::data_lock(cam_data);
    
    World_data::camera_data_set_property_entity_id(cam_data, m_impl->camera_id, entity.get_id());
    
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
    auto cam_data = m_impl->world->camera_data;
    
    World_data::data_lock(cam_data);
    World_data::camera_data_get_property_entity_id(cam_data, m_impl->camera_id, &entity_id);
    World_data::data_unlock(cam_data);
  }
  
  Entity_ref entity(Core_detail::entity_id_from_uint(entity_id));
  
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
  
  auto cam_data = m_impl->world->camera_data;
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
    
    util::generic_id texture_id;
    World_data::camera_data_get_property_texture_id(cam_data, cam_id, &texture_id);
    
    util::generic_id post_id;
    World_data::camera_data_get_property_post_process_id(cam_data, cam_id, &post_id);
    
    // Remove old data.
    World_data::camera_data_erase(cam_data, cam_id);
  
    // Insert the new data
    const size_t insert_at = Camera_utils::find_insert_point_based_on_priority(priority, cam_data->property_priority, cam_data->size);
    if(World_data::camera_data_insert(cam_data, cam_id, insert_at))
    {
      World_data::camera_data_set_property_camera(cam_data, cam_id, props);
      World_data::camera_data_set_property_entity_id(cam_data, cam_id, attached_entity);
      World_data::camera_data_set_property_priority(cam_data, cam_id, priority);
      World_data::camera_data_set_property_post_process_id(cam_data, cam_id, post_id);
      World_data::camera_data_set_property_texture_id(cam_data, cam_id, texture_id);
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
    auto cam_data = m_impl->world->camera_data;
    
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

  // Set the type
  {
    auto cam_data = m_impl->world->camera_data;
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
  
  return get_properties(m_impl->world->camera_data, m_impl->camera_id).type;
}


void
Camera::set_clear_flags(const uint32_t flags)
{
  if(!m_impl || !m_impl->camera_id)
  {
    LOG_ERROR(Error_string::object_has_no_valid_world());
    return;
  }

  // Set the flags
  {
    auto cam_data = m_impl->world->camera_data;
    const auto cam_id = m_impl->camera_id;
    
    World_data::data_lock(cam_data);
    
    ::Camera::Camera_properties props;
    World_data::camera_data_get_property_camera(cam_data, cam_id, &props);
    
    props.clear_flags = flags;
    World_data::camera_data_set_property_camera(cam_data, cam_id, props);
    
    World_data::data_unlock(cam_data);
  }
}


uint32_t
Camera::get_clear_flags() const
{
  if(!m_impl || !m_impl->camera_id)
  {
    LOG_ERROR(Error_string::object_has_no_valid_world());
    return 0;
  }

  return get_properties(m_impl->world->camera_data, m_impl->camera_id).clear_flags;
}


void
Camera::set_render_target(const Render_target &target)
{
  auto cam_data = m_impl->world->camera_data;
  
  World_data::data_lock(cam_data);
  
  World_data::camera_data_set_property_texture_id(cam_data, m_impl->camera_id, target.get_id());
  
  World_data::data_unlock(cam_data);
}


Render_target
Camera::get_render_target() const
{
  assert(false);
  return Render_target();
}



void
Camera::set_width(const uint32_t width)
{
  if(!m_impl || !m_impl->camera_id)
  {
    LOG_ERROR(Error_string::object_has_no_valid_world());
    return;
  }

  // Set the viewport width
  
  {
    auto cam_data = m_impl->world->camera_data;
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
  if(!m_impl || !m_impl->camera_id)
  
  {
    LOG_ERROR(Error_string::object_has_no_valid_world());
    return 0;
  }

  return get_properties(m_impl->world->camera_data, m_impl->camera_id).viewport_width;
}


void
Camera::set_height(const uint32_t height)
{
  if(!m_impl || !m_impl->camera_id)
  {
    LOG_ERROR(Error_string::object_has_no_valid_world());
    return;
  }

  // Set the viewport height
  {
    auto cam_data = m_impl->world->camera_data;
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
  if(!m_impl || !m_impl->camera_id)
  {
    LOG_ERROR(Error_string::object_has_no_valid_world());
    return 0;
  }

  return get_properties(m_impl->world->camera_data, m_impl->camera_id).viewport_height;
}


void
Camera::set_feild_of_view(const float fov_radians)
{
  if(!m_impl || !m_impl->camera_id)
  {
    LOG_ERROR(Error_string::object_has_no_valid_world());
    return;
  }

  // Set fov
  {
    auto cam_data = m_impl->world->camera_data;
    const auto cam_id = m_impl->camera_id;
    
    World_data::data_lock(cam_data);
    
    ::Camera::Camera_properties props;
    World_data::camera_data_get_property_camera(cam_data, cam_id, &props);
    
    props.fov = fov_radians;
    World_data::camera_data_set_property_camera(cam_data, cam_id, props);
    
    World_data::data_unlock(cam_data);
  }
}


float
Camera::get_field_of_view() const
{
  if(!m_impl || !m_impl->camera_id)
  {
    LOG_ERROR(Error_string::object_has_no_valid_world());
    return 0;
  }

  return get_properties(m_impl->world->camera_data, m_impl->camera_id).fov;
}


void
Camera::set_near_plane(const float near_plane)
{
  if(!m_impl || !m_impl->camera_id)
  {
    LOG_ERROR(Error_string::object_has_no_valid_world());
    return;
  }

  // Set the near plane
  {
    auto cam_data = m_impl->world->camera_data;
    const auto cam_id = m_impl->camera_id;
    
    World_data::data_lock(cam_data);
    
    ::Camera::Camera_properties props;
    World_data::camera_data_get_property_camera(cam_data, cam_id, &props);
    
    props.near_plane = near_plane;
    World_data::camera_data_set_property_camera(cam_data, cam_id, props);
    
    World_data::data_unlock(cam_data);
  }
}


float
Camera::get_near_plane() const
{
  if(!m_impl || !m_impl->camera_id)
  {
    LOG_ERROR(Error_string::object_has_no_valid_world());
    return 0;
  }

  return get_properties(m_impl->world->camera_data, m_impl->camera_id).near_plane;
}


void
Camera::set_far_plane(const float far_plane)
{
  if(!m_impl || !m_impl->camera_id)
  {
    LOG_ERROR(Error_string::object_has_no_valid_world());
    return;
  }

  // Set the far plane
  {
    auto cam_data = m_impl->world->camera_data;
    const auto cam_id = m_impl->camera_id;
    
    World_data::data_lock(cam_data);
    
    ::Camera::Camera_properties props;
    World_data::camera_data_get_property_camera(cam_data, cam_id, &props);
    
    props.far_plane = far_plane;
    World_data::camera_data_set_property_camera(cam_data, cam_id, props);
    
    World_data::data_unlock(cam_data);
  }
}


float
Camera::get_far_plane() const
{
  if(!m_impl || !m_impl->camera_id)
  {
    LOG_ERROR(Error_string::object_has_no_valid_world());
    return 0;
  }

  return get_properties(m_impl->world->camera_data, m_impl->camera_id).far_plane;
}


void
Camera::set_clear_color(const Core::Color color)
{
  if(!m_impl || !m_impl->camera_id)
  {
    LOG_ERROR(Error_string::object_has_no_valid_world());
    return;
  }

  // Set the clear color
  {
    auto cam_data = m_impl->world->camera_data;
    const auto cam_id = m_impl->camera_id;
    
    World_data::data_lock(cam_data);
    
    ::Camera::Camera_properties props;
    World_data::camera_data_get_property_camera(cam_data, cam_id, &props);
    
    props.clear_color = color.get_color();
    World_data::camera_data_set_property_camera(cam_data, cam_id, props);
    
    World_data::data_unlock(cam_data);
  }
}


Color
Camera::get_clear_color() const
{
  if(!m_impl || !m_impl->camera_id)
  {
    LOG_ERROR(Error_string::object_has_no_valid_world());
    return Core::Color(0xFFFFFFFF);
  }

  return Core::Color(get_properties(m_impl->world->camera_data, m_impl->camera_id).clear_color);
}


}; // ns