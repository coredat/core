#include <core/material/texture.hpp>


namespace Core {


struct Texture::Impl
{
  uint32_t texture_id = 0;
};


Texture::Texture()
: m_impl(new Impl)
{
}


Texture::Texture(const uint32_t id)
: m_impl(new Impl{id})
{
}


Texture::Texture(const char *filepath)
: m_impl(new Impl)
{
}


Texture::~Texture()
{
}


Texture::Texture(const Texture &other)
: m_impl(new Impl{other.m_impl->texture_id})
{
}


Texture::Texture(Texture &&other)
: m_impl(new Impl{other.m_impl->texture_id})
{
}


uint32_t
Texture::get_id() const
{
  if(m_impl)
  {
    return m_impl->texture_id;
  }

  return 0;
}


} // ns