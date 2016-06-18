#include <core/resources/render_target.hpp>
#include <core/resources/texture.hpp>
#include <graphics_api/pixel_format.cpp>
#include <graphics_api/ogl/ogl_pixel_format.hpp>
#include <graphics_api/ogl/ogl_frame_buffer.hpp>
#include <data/global_data/texture_data.hpp>
#include <data/global_data/resource_data.hpp>


namespace Core {


struct Render_target::Impl
{
  util::generic_id texture_id = 0;
};


Render_target::Render_target()
: m_impl(new Impl)
{
}


Render_target::Render_target(const uint32_t width,
                             const uint32_t height,
                             const Graphics_api::Pixel_format fmt)
: m_impl(new Impl)
{
  auto texture_data = Resource_data::get_resources()->texture_data;
  
  // Create the resources
  Ogl::Frame_buffer fbo;
  Ogl::Texture texture;
  Ogl::Texture depth_buffer;
  Ogl::Texture stencil_buffer;
  {
    Ogl::texture_create_2d(&texture, width, height, Ogl::pixel_format_get_gl_internal_format(fmt), nullptr);
//    Ogl::texture_create_2d(&depth_buffer, width, height, Ogl::pixel_format_get_gl_internal_format(Graphics_api::Pixel_format::rgb8), nullptr);
    Ogl::frame_buffer_create(&fbo, &texture, 1, &depth_buffer, &stencil_buffer);
  }
  
  // Add them to the pool
  {
    Resource_data::data_lock(texture_data);
    
    Resource_data::texture_data_push_back(texture_data, texture_data->size + 1);
    Resource_data::texture_data_set_property_name(texture_data, texture_data->size, "Render Target");
    Resource_data::texture_data_set_property_render_target(texture_data, texture_data->size, fbo);
    Resource_data::texture_data_set_property_texture(texture_data, texture_data->size, texture);
    
    m_impl->texture_id = texture_data->size;
    
    Resource_data::data_unlock(texture_data);
  }
}


Render_target::~Render_target()
{
}


Render_target::Render_target(const Render_target &other)
: m_impl(new Impl)
{
}


Render_target::Render_target(Render_target &&render_target)
: m_impl(new Impl)
{
}

  
Render_target&
Render_target::operator=(const Render_target &render_target)
{
  return *this;
}


Render_target&
Render_target::operator=(Render_target &&render_target)
{
  return *this;
}


Texture
Render_target::get_texture() const
{
  return Texture(m_impl->texture_id);
}


uint32_t
Render_target::get_id() const
{
  return m_impl->texture_id;
}


} // ns