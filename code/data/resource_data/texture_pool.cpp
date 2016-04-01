#include "texture_pool.hpp"
#include <assert.h>
#include <stdatomic.h>


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
  
  return Ogl::Texture();
}


uint32_t
texture_pool_add(Texture_pool *pool, Ogl::Texture *texture)
{


  return 0;
}


} // ns