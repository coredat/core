#include <core/material/texture.hpp>
#include <utilities/logging.hpp>
#include <data/global_data/texture_pool.hpp>
#include <data/global_data/resource_data.hpp>


namespace Core {


struct Texture::Impl
{
  uint32_t texture_id;
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
  auto tex_pool = Resource_data::get_resources()->texture_pool;
  m_impl->texture_id = Resource_data::texture_pool_load(tex_pool, filepath);  
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


Texture&
Texture::operator=(const Texture &other)
{
  m_impl.reset(new Impl);
  m_impl->texture_id = other.m_impl->texture_id;
  return *this;
}


Texture&
Texture::operator=(Texture &&other)
{
  m_impl.reset(new Impl);
  m_impl->texture_id = other.m_impl->texture_id;
  return *this;
}


bool
Texture::exists() const
{
  return !!m_impl->texture_id;
}


Texture::operator bool() const
{
  return exists();
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