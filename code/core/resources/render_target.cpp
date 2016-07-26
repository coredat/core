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
    Ogl::texture_create_2d(&texture, width, height, Ogl::pixel_format_get_gl_internal_format(fmt), false, nullptr);
    Ogl::texture_create_2d(&depth_buffer, width, height, GL_DEPTH24_STENCIL8, false, nullptr); // This isn't working.
    Ogl::frame_buffer_create(&fbo, &texture, 1, &depth_buffer, &stencil_buffer);
    
    LOG_TODO_ONCE("Get rid of this error clear");
    Ogl::error_clear(); // need this because this is broken.
  }

  // Add them to the pool
  {
    Resource_data::data_lock(texture_data);
    
    const util::generic_id id = Resource_data::texture_data_push_back(texture_data);
    Resource_data::texture_data_set_property_name(texture_data, id, "Render Target");
    Resource_data::texture_data_set_property_render_target(texture_data, id, fbo);
    Resource_data::texture_data_set_property_texture(texture_data, id, fbo.color_buffer[0]);
    
    m_impl->texture_id = id;
    
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