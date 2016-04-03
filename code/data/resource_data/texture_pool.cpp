#include "texture_pool.hpp"
#include <assert.h>
#include <stdatomic.h>
#include <utilities/logging.hpp>


namespace
{
  atomic_uint_least32_t texture_id(0);
}


namespace Resource_data {


void
texture_pool_init(Texture_pool *pool)
{
  memset(pool->id, 0, sizeof(pool->id));
  memset(pool->texture, 0, sizeof(pool->texture));
}


Ogl::Texture
texture_pool_find(const Texture_pool *pool, const uint32_t id)
{
  assert(pool);

  for(uint32_t i = 0; i < pool->capacity; ++i)
  {
    if(pool->id[i] == id)
    {
      return pool->texture[i];
    }
  }
  
    LOG_ERROR("Couldn't find resource for requested id")
  
  return Ogl::Texture();
}


uint32_t
texture_pool_add(Texture_pool *pool,
                 const void *data,
                 const uint32_t width,
                 const uint32_t height,
                 const uint32_t depth)
{
  uint32_t index = 0;

  // Find a free index.
  for(uint32_t i = 0; i < pool->capacity; ++i)
  {
    if(pool->id[i] == 0)
    {
      index = i;
      break;
    }
    
    // Error
    if(i == pool->capacity - 1)
    {
      LOG_ERROR("Failed to find a free slot")
      return 0;
    }
  }

  Ogl::Texture new_texture;
  Ogl::texture_create_2d(&new_texture, width, height, GL_RGBA, data);
  
  if(Ogl::texture_is_valid(&new_texture))
  {
    ++texture_id;
    pool->id[index] = texture_id;
    pool->texture[index] = new_texture;
  }
  else
  {
    LOG_ERROR("Failed to create a texture")
    return 0;
  }
  
  return texture_id;
}


} // ns