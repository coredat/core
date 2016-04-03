#include <core/material/texture.hpp>
#include <utilities/logging.hpp>
#include <data/resource_data/texture_pool.hpp>
#include <data/resource_data/resource_data.hpp>
#include <SOIL/SOIL.h>


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
  LOG_TODO("Need to check the texture is in the pool first");
  
  // Create texture
  int width, height;
  unsigned char *img = SOIL_load_image(filepath, &width, &height, 0, SOIL_LOAD_RGBA);
  
  auto tex_pool = Resource_data::get_horrible_hack_resouces()->texture_pool;
  m_impl->texture_id = Resource_data::texture_pool_add(tex_pool, img, width, height, 0);
  
  SOIL_free_image_data(img);
  
  if(!m_impl->texture_id)
  {
    LOG_ERROR("Failed to generate a texture");
  }
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