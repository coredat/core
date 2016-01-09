#include "texture_pool.hpp"
#include <assert.h>


namespace Data {


void
texture_pool_init(Texture_pool *pool)
{
  memset(pool->id, 0, sizeof(pool->id));
  memset(pool->texture, 0, sizeof(pool->texture));
}



renderer::texture*
texture_pool_find(Texture_pool *pool, const std::size_t id)
{
  assert(pool);

  for(std::size_t i = 0; i < pool->size; ++i)
  {
    if(pool->id[i] == id)
    {
      return &pool->texture[i];
    }
  }
  
  return nullptr;
}



} // ns