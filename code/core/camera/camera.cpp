#include <core/camera/camera.hpp>
#include <core/entity/detail/entity_id.hpp>
#include <core/world/detail/world_index.hpp>
#include <core/world/world.hpp>
#include <core/color/color.hpp>
#include <core/entity/entity.hpp>
#include <core/entity/entity_ref.hpp>
#include <core/resources/render_target.hpp>
#include <core/camera/post_process.hpp>
#include <transformations/camera/cam_priorities.hpp>
#include <data/world_data.hpp>
#include <data/world/camera_data.hpp>
#include <data/world/entity_data.hpp>
#include <systems/camera/camera_properties.hpp>
#include <systems/renderer_post/post_shader.hpp>
#include <common/error_strings.hpp>
#include <common/data_types.hpp>
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


Camera::Camera()
{

}


Camera::Camera(const uint32_t width,
               const uint32_t height,
               const float near_plane,
               const float far_plane,
               const float fov)
: m_camera_type(Camera_type::perspective)
, m_viewport_width(width)
, m_viewport_height(height)
, m_fov(fov)
, m_near_plane(near_plane)
, m_far_plane(far_plane)
, m_clear_flags(Camera_clear::color | Camera_clear::depth)
, m_clear_color(0)
, m_cull_mask(-1)
, m_priority(1)
{

}

Camera::~Camera()
{
}

void
Camera::set_tags_to_render(const uint32_t tags)
{
  m_cull_mask = tags;
}


uint32_t
Camera::get_tags_to_render() const
{
  return m_cull_mask;
}


void
Camera::set_post_process(const Core::Post_process &post)
{
//  if(!m_impl || !m_impl->camera_id)
//  {
//    LOG_ERROR(Error_string::object_has_no_valid_world());
//    return;
//  }
//  
//  // Get
//  {
//    auto cam_data = m_impl->world->camera_data;
//    
//    Data::data_lock(cam_data);
//    
//    Post_renderer::Post_shader post_shader;
////    Post_renderer::create_post_shader();
//    
//    const auto id = post.get_id();
//    Data::camera_set_post_process_id(cam_data, m_impl->camera_id, &id);
//    
//    
//    Data::data_unlock(cam_data);
//  }
}


Post_process
Camera::get_post_process() const
{
  assert(false);
  return Post_process("");
}


void
Camera::set_priority(const uint32_t priority)
{
  m_priority = priority;
}


uint32_t
Camera::get_priority() const
{
  return m_priority;
}


void
Camera::set_type(const Core::Camera_type cam_type)
{
  m_camera_type = cam_type;
}


Camera_type
Camera::get_type() const
{
  return m_camera_type;
}


void
Camera::set_clear_flags(const uint32_t flags)
{
  m_clear_flags = flags;
}


uint32_t
Camera::get_clear_flags() const
{
  return m_clear_flags;
}


void
Camera::set_render_target(const Render_target &target)
{
//  auto cam_data = m_impl->world->camera_data;
//  
//  Data::data_lock(cam_data);
//  
//  const auto id = target.get_id();
//  Data::camera_set_texture_id(cam_data, m_impl->camera_id, &id);
//  
//  Data::data_unlock(cam_data);
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
  m_viewport_width = width;
}


uint32_t
Camera::get_width() const
{
  return m_viewport_width;
}


void
Camera::set_height(const uint32_t height)
{
  m_viewport_height = height;
}


uint32_t
Camera::get_height() const
{
  return m_viewport_height;
}


void
Camera::set_feild_of_view(const float fov_radians)
{
  m_fov = fov_radians;
}


float
Camera::get_field_of_view() const
{
  return m_fov;
}


void
Camera::set_near_plane(const float near_plane)
{
  m_near_plane = near_plane;
}


float
Camera::get_near_plane() const
{
  return m_near_plane;
}


void
Camera::set_far_plane(const float far_plane)
{
  m_far_plane = far_plane;
}


float
Camera::get_far_plane() const
{
  return m_far_plane;
}


void
Camera::set_clear_color(const Core::Color color)
{
  m_clear_color = color.get_color();
}


Color
Camera::get_clear_color() const
{
  return Color();
}


}; // ns