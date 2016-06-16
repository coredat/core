#include <core/resources/render_target.hpp>
#include <core/resources/texture.hpp>


namespace Core {


struct Render_target::Impl
{
  int i = 0;
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
  return Texture();
}


} // ns